#include "DFRobot_BMI160.h"

double angle = 0.0;
DFRobot_BMI160 bmi160; 

void reset_angle(){
    angle = 0.0;
}

double get_angle(int16_t angular_speed, double delta_time){
   return angle;
}

RESULT update_gyro(, double threshold){
    double mean_val = 0;
    int N = 10;
    int t = 10;
    int8_t rslt;
    int16_t angularSpeeds[6]={0};
    uint32_t time_stamp[2]= {0};
    uint32_t start_time = 0;
    uint32_t end_time = 0;

    for(int i=0; i<N; i++){

        rslt = bmi160.getAccelGyroData(angularSpeeds, time_stamp);

        if (rslt != 0) {
          Serial.println("I fucking died mate");
          while (1);
        }
        if (i == 0){
          start_time = time_stamp[0];
        }
        mean_val += static_cast<double>(angularSpeeds[2]);
      }
      end_time = time_stamp[0];
      double elapsed_time_ns = (end_time - start_time);
      mean_val /= N;
      mean_val *= 3.14 / 180.0;

      if(((mean_val * elapsed_time_ns)/NS_TO_S > threshold) || ((mean_val * elapsed_time_ns)/NS_TO_S < -threshold)){
        angle += (mean_val * elapsed_time_ns)/NS_TO_S;
      }
}

RESULT setup_gyro(){
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