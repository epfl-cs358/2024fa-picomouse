#include "DFRobot_BMI160.h"
#include "utils.h"

#define NS_TO_S 1000000


RESULT init_pos(DFRobot_BMI160* bmi160, double* offset){
    int sumZ = 0;
    int N = 10;
    int t = 10;
    int8_t rslt;
    int16_t angularSpeeds[6]={0};
    uint32_t time_stamp[2]= {0};
    uint32_t start_time = 0;
    uint32_t end_time = 0;

    for(int i=0; i<N; i++){

        rslt = bmi160->getAccelGyroData(angularSpeeds, time_stamp);

        if (rslt != 0) {
          Serial.println("I fucking died mate");
          while (1);
        }
        if (i == 0){
          start_time = time_stamp[0];
        }
        *offset += static_cast<double>(angularSpeeds[2]);
      }
      end_time = time_stamp[0];
      double elapsed_time_ns = (end_time - start_time);
      *offset /= N;
      *offset *= 3.14 / 180.0;
      *offset = (*offset * elapsed_time_ns)/NS_TO_S;
      return NO_ERROR;
}