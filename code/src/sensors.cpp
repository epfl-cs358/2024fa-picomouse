#include "sensors.h"
#include "DFRobot_VL53L0X.h"
#include "DFRobot_VL6180X.h"

#define DELAY 10

#define LEFT_PIN 4
#define MID_LEFT_PIN 17
#define MID_RIGHT_PIN 16
#define RIGHT_PIN 5

#define NEW_ADDRESS_LEFT 0x40
#define NEW_ADDRESS_MID_LEFT 0x42
#define NEW_ADDRESS_MID 0x44
#define NEW_ADDRESS_MID_RIGHT 0x46
#define NEW_ADDRESS_RIGHT 0x48

DFRobot_VL6180X left;
DFRobot_VL6180X mid_left;
DFRobot_VL53L0X mid;
DFRobot_VL6180X mid_right;
DFRobot_VL6180X right;

float alphas[4] = {0.7669, 0.384, 0.384, 0.7669};
float cs[4] = {57.007, 62.533, 62.533, 57.007};
float betas[4] = {0.5426, 0.4021, 0.4021, 0.5426};


float d_calc[4];
float teta_calc[4];
// Array of side_distances detected by sensors 
// in order LEFT MID_LEFT MID_RIGHT RIGHT
uint16_t side_distances[4];
size_t side_update_time[4];

float mid_distance = 0;
size_t mid_update_time = 0;

void get_side_side_distances(uint16_t* dist){
    for(int i = 0; i < 4; i++){
        dist[i] = side_distances[i];
    }
}


#define INIT_ONE_TOF(sensor, pin, new_address)\
    do{\
        digitalWrite(pin, HIGH);\
        delay(DELAY);\
        sensor.begin();\
        delay(DELAY);\
        sensor.setIICAddr(new_address);\
    }while(0)

RESULT init_TOF(){
    pinMode(LEFT_PIN, OUTPUT);
    pinMode(MID_LEFT_PIN, OUTPUT);
    pinMode(MID_RIGHT_PIN, OUTPUT);
    pinMode(RIGHT_PIN, OUTPUT);

    digitalWrite(LEFT_PIN, LOW);
    digitalWrite(MID_LEFT_PIN, LOW);
    digitalWrite(MID_RIGHT_PIN, LOW);
    digitalWrite(RIGHT_PIN, LOW);
    delay(50);

    Serial.println("start:");

    mid.begin(NEW_ADDRESS_MID);
    mid.setMode(mid.eContinuous,mid.eHigh);
    mid.start();

    Serial.println("1 success");


    INIT_ONE_TOF(left, LEFT_PIN, NEW_ADDRESS_LEFT);
     Serial.println("2 success");
    INIT_ONE_TOF(mid_left, MID_LEFT_PIN, NEW_ADDRESS_MID_LEFT);
     Serial.println("3 success");
    INIT_ONE_TOF(mid_right, MID_RIGHT_PIN, NEW_ADDRESS_MID_RIGHT);
     Serial.println("4 success");
    INIT_ONE_TOF(right, RIGHT_PIN, NEW_ADDRESS_RIGHT);
     Serial.println("5 success");

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
    
    side_distances[0] = temp_left;
    side_distances[1] =  temp_mid_left;
    size_t current_time = millis();
    side_update_time[0] = current_time;
    side_update_time[1] = current_time;
    return NO_ERROR;
}

RESULT update_right(){
    uint8_t temp_right = right.rangePollMeasurement();
    uint8_t right_status = right.getRangeResult();

    uint8_t temp_mid_right = mid_right.rangePollMeasurement();
    uint8_t mid_right_status = mid_right.getRangeResult();

    //CHECK_AND_THROW(right_status || mid_right_status, TOF_READ_FAIL);
    
    side_distances[2] = temp_mid_right;
    side_distances[3] =  temp_right;
    size_t current_time = millis();
    side_update_time[2] = current_time;
    side_update_time[3] = current_time;
    return NO_ERROR;
}   

RESULT update_mid(){
    mid_distance= static_cast<double>(mid.getDistance());
    return NO_ERROR;
}

RESULT update_all(){
    PROPAGATE_ERROR(update_left());
    PROPAGATE_ERROR(update_mid());
    PROPAGATE_ERROR(update_right());
    return NO_ERROR;
}


