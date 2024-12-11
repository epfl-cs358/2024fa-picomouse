#include "sensors.h"
#include "DFRobot_VL53L0X.h"
#include "DFRobot_VL6180X.h"


#define DELAY 10

#define LEFT_PIN 4
#define MID_LEFT_PIN 17
#define MID_PIN 13
#define MID_RIGHT_PIN 16
#define RIGHT_PIN 5

#define NEW_ADDRESS_LEFT 0x40
#define NEW_ADDRESS_MID_LEFT 0x42
#define NEW_ADDRESS_MID 0x44
#define NEW_ADDRESS_MID_RIGHT 0x46
#define NEW_ADDRESS_RIGHT 0x48

#define SLOPE_LEFT 0.95
#define SLOPE_MID_LEFT 1
#define SLOPE_MID 1.03
#define SLOPE_MID_RIGHT 1
#define SLOPE_RIGHT 0.96

#define OFFSET_LEFT -6.3
#define OFFSET_MID_LEFT -0.19
#define OFFSET_MID 40.0
#define OFFSET_MID_RIGHT -4.41
#define OFFSET_RIGHT -1.18

#define MID_WALL_THRESHOLD 80
#define SIDE_WALL_THRESHOLD 80
typedef enum {LEFT_D, MID_LEFT_D, MID_RIGHT_D, RIGHT_D}SIDE_SENSOR_ID;
void calc_d(SIDE_SENSOR_ID id);

DFRobot_VL6180X left;
DFRobot_VL6180X mid_left;
DFRobot_VL6180X mid;
DFRobot_VL6180X mid_right;
DFRobot_VL6180X right;

float alphas[4] = {0.7669, 0.384, 0.384, 0.7669};
float cs[4] = {57.007, 62.533, 62.533, 57.007};
float betas[4] = {0.5426, 0.4021, 0.4021, 0.5426};


float d_calc[4];
float teta_calc[4];
// Array of side_distances detected by sensors 
// in order LEFT MID_LEFT MID MID_RIGHT RIGHT
uint16_t side_distances[5];
size_t side_update_time[5];

float mid_distance = 0;
size_t mid_update_time = 0;

void get_side_side_distances(uint16_t* dist){
    for(int i = 0; i < 4; i++){
        dist[i] = side_distances[i];
    }
}

#define INNIT_ONE_TOF(sensor, pin, new_address)\
    do{\
        digitalWrite(pin, HIGH);\
        delay(DELAY);\
        CHECK_AND_THROW(!sensor.begin(), TOF_INIT_FAIL);\
        delay(DELAY);\
        sensor.setIICAddr(new_address);\
    }while(0)

    
#define CALC_D6(d1, d2, teta1, teta2, d6, orientation)\
    do{\
        float d1_squared = d1 * d1;\
        float d2_squared = d2 * d2;\
        float d3_squared = d1_squared + d2_squared - 2 * d1 * d2 * cos(teta1 - teta2);\
        float d5_squared = (d2_squared + d3_squared - d1_squared);\
        d5_squared *= d5_squared;\
        d5_squared /= (4 * d3_squared);\
        d6 = sqrt(d2_squared - d5_squared);\
        }while(0)

        // float teta3 = acos(d2/d6);
        // orientation = teta2 - teta3;
      


#define CALIB_SENSOR(measured_value, slope, offset) (measured_value - offset)/slope

RESULT init_TOF(){
    pinMode(LEFT_PIN, OUTPUT);
    pinMode(MID_LEFT_PIN, OUTPUT);
    pinMode(MID_PIN, OUTPUT);
    pinMode(MID_RIGHT_PIN, OUTPUT);
    pinMode(RIGHT_PIN, OUTPUT);

    digitalWrite(LEFT_PIN, LOW);
    digitalWrite(MID_LEFT_PIN, LOW);
    digitalWrite(MID_PIN, LOW);
    digitalWrite(MID_RIGHT_PIN, LOW);
    digitalWrite(RIGHT_PIN, LOW);
    delay(50);


    INNIT_ONE_TOF(left, LEFT_PIN, NEW_ADDRESS_LEFT);
    Serial.println("0 short success");
    INNIT_ONE_TOF(mid_left, MID_LEFT_PIN, NEW_ADDRESS_MID_LEFT);
    Serial.println("1 short success");
    INNIT_ONE_TOF(mid, MID_PIN, NEW_ADDRESS_MID);
    Serial.println("2 short success");
    INNIT_ONE_TOF(mid_right, MID_RIGHT_PIN, NEW_ADDRESS_MID_RIGHT);
    Serial.println("3 short success");
    INNIT_ONE_TOF(right, RIGHT_PIN, NEW_ADDRESS_RIGHT);
    Serial.println("4 short success");

    Serial.println("I2C Scanner");
    for (byte i = 8; i < 120; i++) {
      Wire.beginTransmission(i);
      if (Wire.endTransmission() == 0) {
        Serial.print("Found device at address: 0x");
        if (i < 16) {
          Serial.print("0");
        }
        Serial.println(i, HEX);
      }
    }
    Serial.println("4 short success");
    return NO_ERROR;
}

RESULT update_left(){
    uint8_t temp_left = left.rangePollMeasurement();
    uint8_t left_status = left.getRangeResult();

    uint8_t temp_mid_left = mid_left.rangePollMeasurement();
    uint8_t mid_left_status = mid_left.getRangeResult();

    //CHECK_AND_THROW(left_status || mid_left_status, TOF_READ_FAIL);
    
    side_distances[0] = CALIB_SENSOR(temp_left, SLOPE_LEFT, OFFSET_LEFT);
    side_distances[1] =  CALIB_SENSOR(temp_mid_left, SLOPE_MID_LEFT, OFFSET_MID_LEFT);
    size_t current_time = millis();
    side_update_time[0] = current_time;
    side_update_time[1] = current_time;
    return NO_ERROR;
}

RESULT update_right(){
    uint8_t temp_right = right.rangePollMeasurement();
    uint8_t right_status = right.getRangeResult();
    delay(100);
    uint8_t temp_mid_right = mid_right.rangePollMeasurement();
    uint8_t mid_right_status = mid_right.getRangeResult();

    //CHECK_AND_THROW(right_status || mid_right_status, TOF_READ_FAIL);
    
    side_distances[3] = CALIB_SENSOR(temp_mid_right, SLOPE_MID_RIGHT, OFFSET_MID_RIGHT);
    side_distances[4] =  CALIB_SENSOR(temp_right, SLOPE_RIGHT, OFFSET_RIGHT);
    size_t current_time = millis();
    side_update_time[3] = current_time;
    side_update_time[4] = current_time;
    return NO_ERROR;
}   

RESULT update_mid(){
    uint8_t temp_mid = mid.rangePollMeasurement();
    uint8_t right_status = mid.getRangeResult();
    side_distances[2] = temp_mid;
     size_t current_time = millis();
    side_update_time[2] = current_time;
    return NO_ERROR;
}

RESULT update_all(){
    PROPAGATE_ERROR(update_left());
    PROPAGATE_ERROR(update_mid());
    PROPAGATE_ERROR(update_right());
    Serial.printf("mid sensor = %hu \n", side_distances[2]);
    return NO_ERROR;
}

RESULT position_to_wall (POSITION_TO_WALL* result, CALC_CHOICE choice) {
  POSITION_TO_WALL pos = {0};
  switch (choice){
    case CALC_LEFT :
      calc_d(LEFT_D);
      calc_d(MID_LEFT_D);
      CALC_D6(d_calc[MID_LEFT_D], d_calc[LEFT_D], teta_calc[MID_LEFT_D], teta_calc[LEFT_D], pos.distance_left, pos.deviation_left);
      *result = pos;
      break;
    case CALC_RIGHT : 
      calc_d(RIGHT_D);
      calc_d(MID_RIGHT_D);
      CALC_D6(d_calc[MID_RIGHT_D], d_calc[RIGHT_D], teta_calc[MID_RIGHT_D], teta_calc[RIGHT_D],  pos.distance_right, pos.deviation_right);
      *result = pos;
      break;
    case CALC_BOTH :
      calc_d(LEFT_D);
      calc_d(MID_LEFT_D);
      CALC_D6(d_calc[MID_LEFT_D], d_calc[LEFT_D], teta_calc[MID_LEFT_D], teta_calc[LEFT_D],  pos.distance_left, pos.deviation_left);
      calc_d(RIGHT_D);
      calc_d(MID_RIGHT_D);  
      CALC_D6(d_calc[MID_RIGHT_D], d_calc[RIGHT_D], teta_calc[MID_RIGHT_D], teta_calc[RIGHT_D],  pos.distance_right, pos.deviation_right);
      *result = pos;
      break;
  }
  return NO_ERROR;
}

void calc_d(SIDE_SENSOR_ID id) {
  int index = static_cast<int>(id);
  float alpha = alphas[index];
  float beta = betas[index];
  float c = cs[index];
  float c_squared = c*c;
  if(index > 1){
    float r = static_cast<float>(side_distances[index+1]);
  }
  float r = static_cast<float>(side_distances[index]); 
  float r_squared = r*r;
  float d_squared = r_squared + c_squared - 2*r*c*cos(M_PI - alpha);
  float d = sqrt(d_squared);
  d_calc[index] = d;
  teta_calc[index] = alpha + acos((d_squared + c_squared - r_squared) / (2 * d * c));
}

RESULT detect_walls (WALL_DIR* result, int* n_walls_found, CARDINALS mouse_direction){
  float sum[4] = {0}; 
  float mid_sum = 0;
  for(int i = 0 ; i < 5 ; i++){
    PROPAGATE_ERROR(update_all());
    for (int j = 0 ; j < 4 ; j++){
      sum[j]+= side_distances[j];
    }
  }
  int index = 0;
  if ((sum[2]/5) < MID_WALL_THRESHOLD){
    // + 1 MOD 4 CARDINALS to WALL_DIR
    int wall_dir_index = (static_cast<int>(mouse_direction) + 1) & 0b11;
    result[index] = static_cast<WALL_DIR>(wall_dir_index);
    index++;
  }
  if ((sum[0]/5) < SIDE_WALL_THRESHOLD){
    // + 3 MOD 4 CARDINALS left side of the mouse to WALL_DIR
    int wall_dir_index = static_cast<int>(mouse_direction) & 0b11;
    result[index] = static_cast<WALL_DIR>(wall_dir_index);
    index ++;
  }
  if((sum[3]/5) < SIDE_WALL_THRESHOLD){
    // + 1  CARDINALS right side of the mouse to WALL_DIR
    int wall_dir_index = (static_cast<int>(mouse_direction) + 2) & 0b11;
    result[index] = static_cast<WALL_DIR>(wall_dir_index);
    index ++;
  }
  *n_walls_found = index;

  return NO_ERROR;
}
