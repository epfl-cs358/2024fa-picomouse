/**
 * @file navigation.cpp
 * @author PEREGO Zacharie, KNABENHANS Felix, AUMONT Adrien, KACEM Sophia
 * @copyright Copyright (c) 2024
 */

#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"
#include "sensors.h"
#include "utils.h"
#include <Arduino.h>
#include <math.h>

#define POSITION_STOP_THRESHOLD 2  // mm

#define MIN_RPS 0.01  // The minimum rps to consider the mouse stopped

// The maxdifference of distance made by the wheels
#define MAX_DELTA_DISTANCE 1.2

float mouse_absolute_angle = 0;

// --------- Rotation ---------
#define BASE_SPEED 0.06             // Vitesse de rotation de base changed from 0.06 to 0.08
#define MIN_SPEED 0.02              // Vitesse minimale pour corriger vitesse
#define STOP_THRESHOLD 1.0 / 120.0  // Angle threshold to stop the rotation

#define NB_BURST 5

// --------- Correction speed forward---------
#define CORRECTION_SPEED 0.03
#define HALF_CELL 80  // mm
#define NB_ITTERATION_WITHOUT_TOF 0
#define MAX_DIFF_BETWEEN_SIDE_TOF 7
#define MIDDLE_TOF_DIST_TO_WALL 130

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

RESULT compute_traveled_distance(int *tof_mode, float *dist_left,
                                 float *abs_dist_left, float distance);

RESULT compute_encoder_distance(float *dist_left, float *abs_dist_left,
                                float distance);

// ========== Public functions ===========
RESULT turn_with_angle(float angle, MODE mode, int correction) {
  RESULT rslt = update_gyro();
  PROPAGATE_ERROR(rslt);
  float target;

  if (!correction) {
    //only apply if turn  is not in correction mode
    mouse_absolute_angle = mouse_absolute_angle + angle;
    MODULO(mouse_absolute_angle);
    target = mouse_absolute_angle;
  } else {
    target = mouse_absolute_angle + angle;
    MODULO(angle);
  }

  DEBBUG_PRINT(Serial.print("Goal: ");
               Serial.println(mouse_absolute_angle, 5););

  bool is_close_enough = false;

  float error;
  float abs_error;
  float speed = BASE_SPEED;
  int i = 0;

  while (!is_close_enough) {
    // Burst mode
    ++i;
    

    rslt = update_gyro();
    PROPAGATE_ERROR(rslt);

    float new_angle = get_angle();

    error = target - new_angle;
    MODULO(error);
    abs_error = fabs(error);

    if (i > NB_BURST) {
      speed = MIN_SPEED;
    }

    DEBBUG_PRINT(Serial.print("Current error: ");
                 Serial.println(abs_error, 10););

    if (abs_error > STOP_THRESHOLD) {
      if (error > 0) {
        turn_left(mode, speed);
      } else {
        turn_right(mode, speed);
      }
    } else {
      break_wheels();  // Stop the robot
      for (int j = 0; j < 10; j++) {
        update_gyro();
      }
      new_angle = get_angle();
      error = target - new_angle;
      MODULO(error);  // Gère les valeurs circulaires
      abs_error = fabs(error);

      if (abs_error < STOP_THRESHOLD) {
        // Rotation is fine --> we stop
        is_close_enough = true;
      }
    }
  }
  return NO_ERROR;
}

RESULT turn(ROTATION rotation, MODE mode) {
  return turn_with_angle(rotation_to_angle(rotation), mode, 0);
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

RESULT navigation_forward(float distance, float max_speed, int only_encoder) {

  int correcting_mode = 0;
  int tof_mode = 0;
  float speed = max_speed;

  reset_traveled_distance();

  // TODO use physic bcs breaking dst arn't linear
  float slow_dist = 0.40 * distance;
  float very_slow_motor_dist = 0.30 * distance;
  float breaking_dist = 0.20 * distance;  // went from 10 percent to 15

  float dist_left;
  float abs_dist_left;

  while (1) {

    RESULT rslt = update_gyro();
    PROPAGATE_ERROR(rslt);

    // --------- COMPUTE DISTANCE LEFT ---------
    if (only_encoder) {
      // Correction mode --> we only use the encoders
      rslt = compute_encoder_distance(&dist_left, &abs_dist_left, distance);
      // We don't want to use the TOF
      tof_mode = 0;
      PROPAGATE_ERROR(rslt);

    } else {
      // Classic forward
      rslt = compute_traveled_distance(&tof_mode, &dist_left, &abs_dist_left,
                                       distance);
      PROPAGATE_ERROR(rslt);
    }

    if (abs_dist_left > 1.1 * breaking_dist) {
      correcting_mode = 0;
    }

    // --------- CORRECTING MODE (little adjustments when arrived) ---------
    if (correcting_mode) {
      // When the mouse has breaked, we check if we are at the right place
      if (abs_dist_left < POSITION_STOP_THRESHOLD) {

        if (tof_mode) {
          // Recalibrate the gyro
          POSITION_TO_FRONT tof_dist;
          RESULT rslt = position_to_front(&tof_dist);
          PROPAGATE_ERROR(rslt);

          float diff =
            tof_dist.front_distance_left - tof_dist.front_distance_right;

          while (fabs(diff) >= MAX_DIFF_BETWEEN_SIDE_TOF - 5) {
            // We are not centered
            if (diff > 0) {
              // We turn to the right
              turn_right(INPLACE, MIN_SPEED);
            } else {
              // We turn to the left
              turn_left(INPLACE, MIN_SPEED);
            }
            rslt = position_to_front(&tof_dist);
            PROPAGATE_ERROR(rslt);

            diff = tof_dist.front_distance_left - tof_dist.front_distance_right;
          }
          reset_angle(mouse_absolute_angle);
          update_gyro();
        }
        // We are at the right distance
        forward(0);
        break_wheels();
        update_gyro();
        delay(3);
        update_gyro();
        return NO_ERROR;

      } else {
        speed = dist_left >= 0 ? CORRECTION_SPEED : -CORRECTION_SPEED;
        forward(speed);
        continue;
      }
    }

    // If the mouse is too far. True for small distances
    if (dist_left < 0) {
      correcting_mode = 1;
    }

    // --------- Change speed depending on the distance left ---------
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
      correcting_mode = 1;
      continue;
    } else if (abs_dist_left <= very_slow_motor_dist) {
      // Slow down a lot
      speed = CORRECTION_SPEED;
    } else if (abs_dist_left <= slow_dist) {
      // start slowing down
      speed = max_speed / 2;
    } else {
      speed = max_speed;
    }

    forward(speed);
  }
  return NO_ERROR;
}

RESULT compute_traveled_distance(int *tof_mode, float *dist_left,
                                 float *abs_dist_left, float distance) {
  POSITION_TO_FRONT tof_dist;
  RESULT rslt = position_to_front(&tof_dist);
  PROPAGATE_ERROR(rslt);

  if (*tof_mode) {
    float side_tof_mean_dist =
      (tof_dist.front_distance_left + tof_dist.front_distance_right) / 2;

    float diff =
      fabs(tof_dist.front_distance_left - tof_dist.front_distance_right);

    if (diff <= MAX_DIFF_BETWEEN_SIDE_TOF && side_tof_mean_dist < HALF_CELL + 15) {
      // We use the two side sesors
      *dist_left = side_tof_mean_dist - HALF_CELL;
      *abs_dist_left = fabs(*dist_left);

    } else if (tof_dist.front_distance_mid > 1.1 * MIDDLE_TOF_DIST_TO_WALL) {
      // Finally there is no wall in front of the mouse
      *tof_mode = 0;
      compute_encoder_distance(dist_left, abs_dist_left, distance);
    } else {
      // We use the front sensor
      *dist_left = tof_dist.front_distance_mid - HALF_CELL;
      *abs_dist_left = fabs(*dist_left);
    }
  } else {
    //double check
    update_gyro();
    delay(2);
    POSITION_TO_FRONT tof_dist_2;
    RESULT rslt = position_to_front(&tof_dist_2);
    PROPAGATE_ERROR(rslt);
    if (tof_dist.front_distance_mid < MIDDLE_TOF_DIST_TO_WALL && tof_dist_2.front_distance_mid < MIDDLE_TOF_DIST_TO_WALL) {
      // There is a front wall in the cell --> we trust the TOFs to center the
      // mouse in the cell
      *tof_mode = 1;
    }
    // We trust encoders
    compute_encoder_distance(dist_left, abs_dist_left, distance);
  }
  return NO_ERROR;
}

RESULT compute_encoder_distance(float *dist_left, float *abs_dist_left,
                                float distance) {
  WHEELS_DISTANCES dist = get_traveled_distance();
  float mean_dist = (dist.left_distance + dist.right_distance) / 2;
  float abs_mean_dist = fabs(mean_dist);

  *dist_left = distance - abs_mean_dist;
  *abs_dist_left = fabs(*dist_left);
  return NO_ERROR;
}
