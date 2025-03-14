/**
 * @file gyroscope.cpp
 * @author KACEM Sophia, AUMONT Adrien
 * @copyright Copyright (c) 2024
 */

#include "gyroscope.h"
#include "DFRobot_BMI160.h"
#include "utils.h"
#include <math.h>

#define GYRO_NB_ITERATIONS 1
#define CALIBRATION_OFFSET_NB_ITERATIONS 300

#define SCALE_FACTOR 1.0 / 0.0743

#define OVER_THRESHOLD(angle, threshold)\
  (angle < -threshold || angle > threshold)

float angle = 0.0; // unit : radian
DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x68;
float offset = 0.0;
uint32_t start_time = 0;

void reset_angle(float new_angle) {
  angle = new_angle;
  start_time = 0;
}

float get_angle() { return angle; }

RESULT update_gyro() {
  int16_t gyroData[3];
  uint32_t timestamp;
  // uint32_t start_time = 0;
  float mean = 0;

  for (int i = 0; i < GYRO_NB_ITERATIONS; i++) {
    // Lecture des données du gyroscope avec timestamp
    if (bmi160.getGyroData(gyroData, &timestamp) != BMI160_OK) {
      Serial.println("Error reading gyro");
      return GYRO_ERROR;
    }

    if (start_time == 0) {
      start_time = timestamp;
    }

    // Correction de l'offset
    float angularSpeedZ =
        static_cast<float>(gyroData[2]) - offset; // Convertir LSB en rad/s
    mean += angularSpeedZ;
  }

  long deltaTime = (timestamp - start_time); // Convertir ns en secondes
  DEBBUG_PRINT(Serial.print("deltaTime: "); Serial.println(deltaTime));

  mean /= GYRO_NB_ITERATIONS;
  // Mise à jour de l'angle accumulé
  angle += mean * deltaTime * SCALE_FACTOR / 1e9;

  MODULO(angle);
  start_time = timestamp;

  return NO_ERROR;
}

RESULT init_gyro() {
  // init the hardware bmin160

  CHECK_AND_THROW(bmi160.softReset() != BMI160_OK, GYRO_INIT_FAIL);
  CHECK_AND_THROW(bmi160.I2cInit(i2c_addr) != BMI160_OK, GYRO_INIT_FAIL);

  compute_offset();
  reset_angle(0.0);

  return NO_ERROR;
}

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
    biasZ += gyroData[2]; // Composante Z du gyroscope (rotation autour de l'axe
                          // vertical)
    delay(10);
  }

  biasZ /= CALIBRATION_OFFSET_NB_ITERATIONS; // Moyenne pour obtenir le biais
  offset = biasZ;
  Serial.print(offset, 10);
  Serial.println("   Done");
  return NO_ERROR;
}
