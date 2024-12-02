#include "DFRobot_BMI160.h"
#include "calibration.h"

DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x68;
double offset = 0.0;

void setup(){
  Serial.begin(115200);
  delay(100);

  //init the hardware bmin160
  if (bmi160.softReset() != BMI160_OK){
    Serial.println("reset false");
    while(1);
  }

  //set and init the bmi160 i2c address
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK){
    Serial.println("init false");
    while(1);
  }
}

void loop(){
  RESULT rslt_init_pos = NO_ERROR;
  rslt_init_pos = init_pos(&bmi160, &offset);
  Serial.println("hello");
  Serial.println(offset, 5);
  while(1);
}