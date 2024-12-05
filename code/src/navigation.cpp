#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"
#include "utils.h"
#include <Arduino.h>

#define INTERVAL_STOP_SPEED 1.0 / 80.0

#define DEBBUG 1

RESULT adjust_front_distance() { return NO_ERROR; }

RESULT adjust_sides_distance() { return NO_ERROR; }

RESULT alignement() { return NO_ERROR; }

RESULT turn(double angle, MODE mode) {
  update_gyro(0.00150);
  double curr_angle = get_angle() + angle;
  float rotation_speed = 0.01;
  Serial.print("goal:  ");
  Serial.println(curr_angle, 5);
  curr_angle = MODULO_PI(curr_angle);
  bool is_close_enough = false;

  while (!is_close_enough) {
    update_gyro(0.00150);
    double new_angle = get_angle();
    if (new_angle > curr_angle + INTERVAL_STOP_SPEED) {
      turn_right(mode);
    } else if (new_angle < curr_angle - INTERVAL_STOP_SPEED) {
      turn_left(mode);
    } else {
      break_wheels();
      is_close_enough = true;
    }
  }

  return NO_ERROR;
}

RESULT init_all_sensors() {
  setup_gyro();
  // init ToF sensors here
  return NO_ERROR;
}