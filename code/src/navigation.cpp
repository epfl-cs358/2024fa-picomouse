#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"
#include "utils.h"
#include <Arduino.h>

#define STOP_THRESHOLD 1.0 / 120.0

float mouse_absolute_angle = 0;

// ========== Private functions ===========
// Convert a cardinal to an angle, return the angle in radian ]-1;1]
float rotation_to_angle(ROTATION rotation) {
  switch (rotation) {
    case LEFT:
      return 0.5;
    case RIGHT:
      return -0.5;
    case HALF_TURN:
      return 1;
    case NO_TURN:
      return 0;
    default:
      return 0;
  }
}


// ========== Public functions ===========
RESULT adjust_front_distance() {
  return NO_ERROR;
}

RESULT adjust_sides_distance() {
  return NO_ERROR;
}

RESULT alignement() {
  return NO_ERROR;
}

/**
RESULT turn(float angle, MODE mode) {
  update_gyro();
  float curr_angle = get_angle() + angle;
  float rotation_speed = 0.01;
  Serial.print("goal:  ");
  Serial.println(curr_angle, 5);
  curr_angle = MODULO(curr_angle);
  bool is_close_enough = false;

  int burst_speed_count = 0;

  const int nbr_burst_speed = 2;
  float speed = 0;

  while (!is_close_enough) {
    speed = burst_speed_count < nbr_burst_speed ? 0.1 : 0.015;
    update_gyro();
    float new_angle = get_angle();
    Serial.println(new_angle, 10);

    if (new_angle > curr_angle + INTERVAL_STOP_SPEED) {
      turn_right(mode, speed);
    } else if (new_angle < curr_angle - INTERVAL_STOP_SPEED) {
      turn_left(mode, speed);
    } else {
      break_wheels();
      is_close_enough = true;
    }
    burst_speed_count++;
  }

  return NO_ERROR;
}*/
RESULT turn(ROTATION rotation, MODE mode) {
  update_gyro();
  if(rotation == NO_TURN) return NO_ERROR;
  mouse_absolute_angle = mouse_absolute_angle + rotation_to_angle(rotation);
  MODULO(mouse_absolute_angle);
  Serial.print("Goal: ");
  Serial.println(mouse_absolute_angle, 5);
  bool is_close_enough = false;

  const float BASE_SPEED = 0.06;  // Vitesse de rotation de base
  const float MIN_SPEED = 0.01;   // Vitesse minimale pour corriger
  //const float KP = 0.5;             // Gain proportionnel pour ajuster la vitesse
  float error;
  float abs_error;
  float speed = BASE_SPEED;
  int i = 0;
  while (!is_close_enough) {
    if (++i > 5) {
      speed = MIN_SPEED;
    }
    update_gyro();
    float new_angle = get_angle();
    //Serial.print("Current angle: ");
    //Serial.println(new_angle, 10);

    error = mouse_absolute_angle - new_angle;
    MODULO(error);  // Gère les valeurs circulaires
    abs_error = fabs(error);
    Serial.print("Current error: ");
    Serial.println(abs_error, 10);

    if (abs_error > STOP_THRESHOLD) {
      //float speed = KP * abs_error;  // Vitesse proportionnelle à l'erreur
      //speed = constrain(speed, MIN_SPEED, BASE_SPEED);

      if (error > 0) {
        turn_left(mode, speed);
      } else {
        turn_right(mode, speed);
      }
    } else {
      break_wheels();  // Stop le robot
      for (int j = 0; j < 2; j++) {
        update_gyro();
      }
      new_angle = get_angle();
      error = mouse_absolute_angle - new_angle;
      MODULO(error);  // Gère les valeurs circulaires
      abs_error = fabs(error);
      if (abs_error < STOP_THRESHOLD) {
        is_close_enough = true;
      }
    }
  }


  return NO_ERROR;
}


RESULT init_all_sensors() {
  RESULT rslt = init_gyro();
  PROPAGATE_ERROR(rslt);

  
  // init ToF sensors here
  return NO_ERROR;
}
/**
  * @brief 
  * 
  * @param distance in mm
  * @return RESULT 
  */
RESULT navigation_forward(float distance){

}
