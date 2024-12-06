#include "DFRobot_BMI160.h"
#include "gyroscope.h"
#include "utils.h"
#include <math.h>

#define GYRO_NB_ITERATIONS 2
#define CALIBRATION_OFFSET_NB_ITERATIONS 300

#define SCALE_FACTOR 0.5 / 0.65
#define OVER_THRESHOLD(angle, threshold) (angle < -threshold || angle > threshold)



float angle = 0.0;  //unit : radian
DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x68;
float offset = 0.0;

void reset_angle() {
  angle = 0.0;
}

float get_angle() {
  return angle;
}
/**
RESULT update_gyro() {
  float mean_val = 0;
  int8_t rslt;
  int16_t angularSpeeds[6] = { 0 };
  uint32_t time_stamp[2] = { 0 };
  uint32_t start_time = 0;
  uint32_t end_time = 0;

  for (int i = 0; i < GYRO_NB_ITERATIONS; i++) {

    rslt = bmi160.getAccelGyroData(angularSpeeds, time_stamp);

    if (rslt != 0) {
      Serial.println("I fucking died mate");
      while (1)
        ;
    }
    if (i == 0) {
      start_time = time_stamp[0];
    }
    float angular_speed = static_cast<float>(angularSpeeds[2]);
    if(OVER_THRESHOLD(angular_speed, 0.00001)){
      mean_val += angular_speed - offset; 
    }
  } 

  end_time = time_stamp[0];
  float elapsed_time_ns = (end_time - start_time);
  mean_val /= GYRO_NB_ITERATIONS;
  mean_val *= 3.1415 / 180.0;

  //if (OVER_THRESHOLD(((mean_val * elapsed_time_ns) / NS_TO_S), THRESHOLD)) {
    angle += (mean_val * elapsed_time_ns) / NS_TO_S;
  
  angle = MODULO_PI(angle);
  
  return NO_ERROR;
}*/

RESULT update_gyro() {
  int16_t gyroData[3];
  uint32_t timestamp[2];
  uint32_t start_time = 0;
  float mean = 0;

  for (int i = 0; i < GYRO_NB_ITERATIONS; i++) {
    // Lecture des données du gyroscope avec timestamp
    if (bmi160.getGyroData(gyroData, timestamp) != BMI160_OK) {
      Serial.println("Error reading gyro");
      return GYRO_ERROR;
    }
    if (i == 0) {
      start_time = timestamp[0];
    }

    // Correction de l'offset
    float angularSpeedZ = static_cast<float>(gyroData[2]) - offset;  // Convertir LSB en rad/s
    mean += angularSpeedZ * SCALE_FACTOR;
  }

  float deltaTime = (timestamp[0] - start_time) / 1e6;  // Convertir ns en secondes

  // Mise à jour de l'angle accumulé
  angle += mean * deltaTime  * (3.1415 / 180.0);

  angle = MODULO_PI(angle);
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

  compute_offset();

  return NO_ERROR;
}
/** 
RESULT compute_offset(){
  Serial.print("Computing offset...");
  float mean_val = 0;
  int8_t rslt;
  int16_t angularSpeeds[6] = { 0 };
  uint32_t time_stamp[2] = { 0 };
  uint32_t start_time = 0;
  uint32_t end_time = 0;

  for (int i = 0; i < CALIBRATION_OFFSET_NB_ITERATIONS; i++) {

    rslt = bmi160.getAccelGyroData(angularSpeeds, time_stamp);

    if (rslt != 0) {
      Serial.println("I fucking died mate");
      while (1)
        ;
    }
    if (i == 0) {
      start_time = time_stamp[0];
    }
    mean_val += static_cast<float>(angularSpeeds[2]);
  }
  end_time = time_stamp[0];

  mean_val /= CALIBRATION_OFFSET_NB_ITERATIONS; 


  offset = mean_val;
  Serial.print(offset, 10);
  Serial.println("Done");

  return NO_ERROR;
}*/

RESULT compute_offset() {
  Serial.print("Computing offset...");
  int16_t gyroData[3];
  float biasZ = 0;

  for (int i = 0; i < CALIBRATION_OFFSET_NB_ITERATIONS; i++) {
    if (bmi160.getGyroData(gyroData) != BMI160_OK) {
      Serial.println("Error reading gyro during calibration");
      while (1)
        ;
    }
    biasZ += gyroData[2];  // Composante Z du gyroscope (rotation autour de l'axe vertical)
    delay(10);
  }

  biasZ /= CALIBRATION_OFFSET_NB_ITERATIONS;  // Moyenne pour obtenir le biais
  offset = biasZ;
  Serial.print(offset, 10);
  Serial.println("   Done");
  return NO_ERROR;
}
