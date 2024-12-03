#include "DFRobot_BMI160.h"
#include "gyroscope.h"
#include <math.h>

#define NS_TO_S 1000000
#define OVER_THRESHOLD(angle, threshold) (angle < -threshold || angle > threshold)


double angle = 0.0;
DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x68;

void reset_angle() {
  angle = 0.0;
}

double get_angle() {
  return angle;
}

RESULT update_gyro(double threshold) {
  double mean_val = 0;
  int N = 10;
  int t = 10;
  int8_t rslt;
  int16_t angularSpeeds[6] = { 0 };
  uint32_t time_stamp[2] = { 0 };
  uint32_t start_time = 0;
  uint32_t end_time = 0;

  for (int i = 0; i < N; i++) {

    rslt = bmi160.getAccelGyroData(angularSpeeds, time_stamp);

    if (rslt != 0) {
      Serial.println("I fucking died mate");
      while (1)
        ;
    }
    if (i == 0) {
      start_time = time_stamp[0];
    }
    mean_val += static_cast<double>(angularSpeeds[2]);
  }
  end_time = time_stamp[0];
  double elapsed_time_ns = (end_time - start_time);
  mean_val /= N;
  mean_val *= 3.14 / 180.0;

  if (OVER_THRESHOLD(((mean_val * elapsed_time_ns) / NS_TO_S), threshold)) {
    angle += (mean_val * elapsed_time_ns) / NS_TO_S;
  }

  angle = fmod(angle + 1, 2);  // Shift by 1, then take modulo 2
  if (angle < 0) {
    angle += 2;  // Ensure positive result
  }
  angle -= 1;  // Shift back to [-1, 1]
  return NO_ERROR;
}

RESULT setup_gyro() {
  //init the hardware bmin160
  if (bmi160.softReset() != BMI160_OK) {
    Serial.println("reset false");
    while (1)
      ;
  }

  //set and init the bmi160 i2c address
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK) {
    Serial.println("init false");
    while (1)
      ;
  }
  return NO_ERROR;
}