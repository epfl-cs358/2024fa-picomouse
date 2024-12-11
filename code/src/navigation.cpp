#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"
#include "sensors.h"
#include "utils.h"
#include <Arduino.h>
#include <math.h>

#define STOP_THRESHOLD 1.0 / 120.0

#define POSITION_STOP_THRESHOLD 2 // mm

#define MIN_RPS 0.01 // The minimum rps to consider the mouse stopped

// The maxdifference of distance made by the wheels
#define MAX_DELTA_DISTANCE 1.2

float mouse_absolute_angle = 0;

// ========== Private functions ===========
// Convert a cardinal to an angle, return the angle in radian ]-1;1]
float rotation_to_angle(ROTATION rotation) {
  switch (rotation) {
  case LEFT_TURN:
    return 0.5;
  case RIGHT_TURN:
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
RESULT adjust_front_distance() { return NO_ERROR; }

RESULT adjust_sides_distance() { return NO_ERROR; }

RESULT alignement() { return NO_ERROR; }

RESULT turn(ROTATION rotation, MODE mode) {
  update_gyro();
  // if (rotation == NO_TURN) {
  //   //correct angle here
  //   if()
  //   return NO_ERROR;
  // }
  mouse_absolute_angle = mouse_absolute_angle + rotation_to_angle(rotation);
  MODULO(mouse_absolute_angle);

  DEBBUG_PRINT(Serial.print("Goal: ");
               Serial.println(mouse_absolute_angle, 5););

  bool is_close_enough = false;

  const float BASE_SPEED = 0.07; // Vitesse de rotation de base changed from 0.06 to 0.08
  const float MIN_SPEED = 0.01;  // Vitesse minimale pour corriger
  // const float KP = 0.5;             // Gain proportionnel pour ajuster la
  // vitesse
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
    // Serial.print("Current angle: ");
    // Serial.println(new_angle, 10);

    error = mouse_absolute_angle - new_angle;
    MODULO(error); // Gère les valeurs circulaires
    abs_error = fabs(error);

    DEBBUG_PRINT(Serial.print("Current error: ");
                 Serial.println(abs_error, 10););

    if (abs_error > STOP_THRESHOLD) {
      // float speed = KP * abs_error;  // Vitesse proportionnelle à l'erreur
      // speed = constrain(speed, MIN_SPEED, BASE_SPEED);

      if (error > 0) {
        turn_left(mode, speed);
      } else {
        turn_right(mode, speed);
      }
    } else {
      break_wheels(); // Stop the robot
      for (int j = 0; j < 2; j++) {
        update_gyro();
      }
      new_angle = get_angle();
      error = mouse_absolute_angle - new_angle;
      MODULO(error); // Gère les valeurs circulaires
      abs_error = fabs(error);

      if (abs_error < STOP_THRESHOLD) {
        is_close_enough = true;
      }
    }
  }

  return NO_ERROR;
}

RESULT init_all_sensors() {
  RESULT rslt = init_motors();
  PROPAGATE_ERROR(rslt);
  rslt = init_gyro();
  PROPAGATE_ERROR(rslt);
  rslt = init_TOF();
  PROPAGATE_ERROR(rslt);
  return NO_ERROR;
}

RESULT navigation_forward(float distance, float max_speed) {
  const float correction_speed = 0.05;
  bool correcting_mode = false;
  float speed = max_speed;
  WHEELS_DISTANCES dist = {0, 0};
  reset_traveled_distance();

  // TODO use physic bcs breaking dst arn't linear
  float slow_dist = 0.30 * distance;
  float very_slow_motor_dist = 0.20 * distance;
  float breaking_dist = 0.15 * distance; // went from 10 percent to 15

  
  while (true) {
    update_gyro();
    // --------- THRESHOLD GYRO CHECK ---------
    // TODO implémenter plus tard
    // if threashold dépassé, correction burst --> faudrait faire une fonction
    // pour check ça if trop grand stopper la mouse et faire une rotation
    // INPLACE

    // --------- DISTANCE CHECK ---------------
    // TODO faire plus tard
    // if distance trop proche d'un mur, STOP, (return error ou justr s'arrêter)
    // Si la place pour faire demi tour sauvegarder traveled dist, faire demi
    // tour, aller au centre faire quart de tour, recommencer à compter la
    // distance

    // update_gypro(); SPAM un max pour la précision
    dist = get_traveled_distance();
    float mean_dist = (dist.left_distance + dist.right_distance) / 2;
    float abs_mean_dist = fabs(mean_dist);

    float dist_left = distance - abs_mean_dist;
    float abs_dist_left = fabs(dist_left);
    // peut etre faire une condition qui vérifie que les deux distances ne
    // diffèrent pas trop

    if (correcting_mode) {
      //When the mouse has breaked, we check if we are at the right place
      if (abs_dist_left < POSITION_STOP_THRESHOLD) {
        // We are at the right distance
        forward(0);
        break_wheels();
        update_gyro();
        delay(5);
        return NO_ERROR;
      } else {
        speed = dist_left >= 0 ? correction_speed : -correction_speed;
        forward(speed);
        continue;
      }
    }

    if (abs_dist_left <= breaking_dist) {
      // start_breaking
      speed = 0;
      while (get_rps() > MIN_RPS) {
        // Wait for the mouse to stop
        forward(0);
        break_wheels();
        update_gyro();
        delay(5);
      }
      correcting_mode = true;
      continue;
    } else if (abs_dist_left <= very_slow_motor_dist) {
      // Slow down a lot
      speed = 0.05;
    } else if (abs_dist_left <= slow_dist) {
      // start slowing down
      speed = max_speed / 2;
    }

    forward(speed);
  }
  return NO_ERROR;
}

// void break_and_correct_distance() {

//   while (true) {
//     update_gyro();

//     WHEELS_DISTANCES dist = get_traveled_distance();

//     float mean_dist = (dist.left_distance + dist.right_distance) / 2;
//     float abs_mean_dist = fabs(mean_dist);

//     float dist_left = distance - abs_mean_dist;
//     float abs_dist_left = fabs(dist_left);

//     if (fabs(dist_left) > POSITION_STOP_THRESHOLD) {
//       // We have break --> but we are too fare or too early
//       // we are going foward or backward
//       float speed = dist_left >= 0 ? correction_speed : -correction_speed;
//       forward(correction);
//       if (abs_dist_left <= POSITION_STOP_THRESHOLD) {
//       }
//     }
//   }
