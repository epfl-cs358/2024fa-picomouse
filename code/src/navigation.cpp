#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"
#include "sensors.h"
#include "utils.h"
#include <Arduino.h>
#include <math.h>

#define POSITION_STOP_THRESHOLD 2 // mm

#define MIN_RPS 0.01 // The minimum rps to consider the mouse stopped

// The maxdifference of distance made by the wheels
#define MAX_DELTA_DISTANCE 1.2

float mouse_absolute_angle = 0;

// --------- Rotation speed ---------
#define BASE_SPEED 0.2 // Vitesse de rotation de base changed from 0.06 to 0.08
#define MIN_SPEED 0.05 // Vitesse minimale pour corriger vitesse
#define STOP_THRESHOLD 1.0 / 120.0 // Angle threshold to stop the rotation

// --------- Correction speed forward---------
#define CORRECTION_SPEED 0.05
#define HALF_CELL 80 // mm

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

  mouse_absolute_angle = mouse_absolute_angle + rotation_to_angle(rotation);
  MODULO(mouse_absolute_angle);

  DEBBUG_PRINT(Serial.print("Goal: ");
               Serial.println(mouse_absolute_angle, 5););

  bool is_close_enough = false;

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

    error = mouse_absolute_angle - new_angle;
    MODULO(error); // Gère les valeurs circulaires
    abs_error = fabs(error);

    DEBBUG_PRINT(Serial.print("Current error: ");
                 Serial.println(abs_error, 10););

    if (abs_error > STOP_THRESHOLD) {

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
        // Rotation is fine --> we stop
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

  bool correcting_mode = false;
  int tof_mode = 0;
  float speed = max_speed;
  WHEELS_DISTANCES dist = {0, 0};
  reset_traveled_distance();

  // TODO use physic bcs breaking dst arn't linear
  float slow_dist = 0.50 * distance;
  float very_slow_motor_dist = 0.40 * distance;
  float breaking_dist = 0.30 * distance; // went from 10 percent to 15

  float mean_dist;
  float abs_mean_dist;

  float dist_left;
  float abs_dist_left;

  int i = 0;
  while (true) {
    i++;
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
    // FRONT_DISTANCE_MID > (7 + 3)

    POSITION_TO_FRONT tof_dist;

    position_to_front(&tof_dist);

    if (tof_mode && i > 10) {
      float side_tof_mean_dist =
          (tof_dist.front_distance_left + tof_dist.front_distance_right) / 2;
      float diff =
          fabs(tof_dist.front_distance_left - tof_dist.front_distance_right);
      // TODO vérifier que la mouse est droite =============
      if (diff <= 6 && side_tof_mean_dist < HALF_CELL) {
        // We use the two side sesors
        dist_left = side_tof_mean_dist - HALF_CELL;
        abs_dist_left = fabs(dist_left);
      } else {
        // We use the front sensor
        dist_left = tof_dist.front_distance_mid - HALF_CELL;
        abs_dist_left = fabs(dist_left);
      }
    } else if (tof_dist.front_distance_mid < 140 && i > 10) {
      // There is a front wall in the cell --> we trust the TOFs to cetner the
      // mouse in the cell
      // 180 mm distance from wall
      tof_mode = 1;
    } else {
      // We trust encoders
      dist = get_traveled_distance();
      float mean_dist = (dist.left_distance + dist.right_distance) / 2;
      float abs_mean_dist = fabs(mean_dist);

      dist_left = distance - abs_mean_dist;
      abs_dist_left = fabs(dist_left);
    }

    if (correcting_mode) {
      // When the mouse has breaked, we check if we are at the right place
      if (abs_dist_left < POSITION_STOP_THRESHOLD) {
        // We are at the right distance
        forward(0);
        break_wheels();
        update_gyro();
        delay(5);
        return NO_ERROR;
      } else {
        speed = dist_left >= 0 ? CORRECTION_SPEED : -CORRECTION_SPEED;
        forward(speed);
        continue;
      }
    }

    // If the mouse is too far. True for small distances
    if (dist_left < 0) {
      correcting_mode = true;
    }

    if (abs_dist_left <= breaking_dist) {
      // start_breaking
      speed = 0;
      while (get_rps() > MIN_RPS) {
        // Wait for the mouse to stop
        forward(0);
        break_wheels();
        for (int j = 0; j < 3; j++) {
          update_gyro();
          delay(5);
        }
      }
      correcting_mode = true;
      continue;
    } else if (abs_dist_left <= very_slow_motor_dist) {
      // Slow down a lot
      speed = CORRECTION_SPEED;
    } else if (abs_dist_left <= slow_dist) {
      // start slowing down
      speed = max_speed / 2;
    }

    forward(speed);
  }
  return NO_ERROR;
}

