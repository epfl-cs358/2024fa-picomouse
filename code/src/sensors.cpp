#include "sensors.h"
#include "DFRobot_VL53L0X.h"
#include "DFRobot_VL6180X.h"

#define DELAY 10

#define LEFT_PIN 13
#define MID_LEFT_PIN 12
#define MID_RIGHT_PIN 11
#define RIGHT_PIN 10

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

#define INNIT_ONE_TOF(sensor, pin, new_address)\
    do{\
        digitalWrite(pin, HIGH);\
        delay(DELAY);\
        CHECK_AND_THROW(sensor.begin(), TOF_INNIT_FAIL);\
        delay(DELAY);\
        sensor.setIICAddr(new_address);\
    }while(0)

RESULT innit_TOF(){
    pinMode(LEFT_PIN, OUTPUT);
    pinMode(MID_LEFT_PIN, OUTPUT);
    pinMode(MID_RIGHT_PIN, OUTPUT);
    pinMode(RIGHT_PIN, OUTPUT);

    digitalWrite(LEFT_PIN, LOW);
    digitalWrite(MID_LEFT_PIN, LOW);
    digitalWrite(MID_RIGHT_PIN, LOW);
    digitalWrite(RIGHT_PIN, LOW);
    delay(50);

    mid.begin(NEW_ADDRESS_MID);
    mid.setMode(sensor.eContinuous,sensor.eHigh);
    mid.start();


    INNIT_ONE_TOF(left, LEFT_PIN, NEW_ADDRESS_LEFT);
    INNIT_ONE_TOF(mid_left, MID_LEFT_PIN, NEW_ADDRESS_MID_LEFT);
    INNIT_ONE_TOF(mid_right, MID_RIGHT_PIN, NEW_ADDRESS_MID_RIGHT);
    INNIT_ONE_TOF(right, RIGHT_PIN, NEW_ADDRESS_RIGHT);

    return NO_ERROR;
}

RESULT update_left(){
    uint8_t temp_left = left.rangePollMeasurement()
    uint8_t left_status = left.getRangeResult()

    uint8_t temp_mid_left = mid_left.rangePollMeasurement()
    uint8_t mid_left_status = mid_left.getRangeResult()

    CHECK_AND_THROW(left_status || mid_left_status, TOF_READ_FAIL);
    
    distances[0] = temp_left;
    distances[1] =  temp_mid_left;
    size_t current_time = millis();
    update_time[0] = current_time;
    update_time[1] = current_time;
    return NO_ERROR;
}

RESULT update_right(){
    uint8_t temp_right = right.rangePollMeasurement()
    uint8_t right_status = right.getRangeResult()

    uint8_t temp_mid_right = mid_right.rangePollMeasurement()
    uint8_t mid_right_status = mid_right.getRangeResult()

    CHECK_AND_THROW(right_status || mid_status, TOF_READ_FAIL);
    
    distances[2] = temp_mid_right;
    distances[3] =  temp_right;
    size_t current_time = millis();
    update_time[2] = current_time;
    update_time[3] = current_time;
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




void setup() {
  //initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  //join i2c bus (address optional for master)
  Wire.begin();
  //Set I2C sub-device address
  sensor.begin(0x50);
  //Set to Back-to-back mode and high precision mode
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  //Laser rangefinder begins to work
  sensor.start();
}

void loop() 
{
  //Get the distance
  Serial.print("Distance: ");Serial.println(sensor.getDistance());
  //The delay is added to demonstrate the effect, and if you do not add the delay,
  //it will not affect the measurement accuracy
  delay(500);
}