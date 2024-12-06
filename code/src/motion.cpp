#include "motion.h"
#include "motors.h"
#include <Arduino.h>
#include <math.h>

// ========== DEBUG MODE ===========================
// UNCOMMENT THIS LINE TO DISSABLE DEBUG MODE
// #define DEBUG_MODE
//====================================================

// Correction factor for the speed
#define KP 0.1

// Speed of the motors when a turning function is called (INPLACE mode)
#define ROTATION_SPEED 0.001

// Defined speeds for both run and search mode
#define SPEED_SEARCH 0.2
#define SPEED_RUN 1

#define DELTA_TIME 50 // ms, the time between two measures of the counter

// Breaking power [0;1]
#define BREAKING_POWER 1

// Enum for different modes of turns
// TODO implement smooth turn

// Correction angle
#define CORRECTION_ANGLE 1.0 / 16.0

// Correction factor if there is a CORR_RIGHT or CORR_LEFT
#define EXT_CORR_FACTOR 0.01
#define DELTA_CORR 5.0

float left_speed_goal = 0;  // [-1;1]
float right_speed_goal = 0; // [-1;1]

float left_speed = 0;  // [-1;1]
float right_speed = 0; // [-1;1]

// radian per second
float left_rps = 0;
float right_rps = 0;

unsigned int correction_count = 1;

unsigned long last_time = 0;
unsigned long current_time = 0;

MOTOR_STEPS old_steps_count = {0, 0};
MOTOR_STEPS new_steps_count = {0, 0};

EXT_CORRECTION previous_correction = NO_CORR;

// ========== Private functions ===========
void set_delta_steps_count_to_zero() {
    
  old_steps_count = get_steps_count();
  new_steps_count = old_steps_count;
}

float cap_speed(float speed) {
  speed = speed > 1 ? 1 : speed;
  speed = speed < -1 ? -1 : speed;
  return speed;
}

// Turn the mouse in place. The speed is the same for both motors, right = true
// means the mouse will turn right
void turn_inplace(float roation_speed, bool right) {
  roation_speed = fabs(cap_speed(roation_speed));

  if (last_time == 0) {
    motors_init();
  }
  last_time = millis();

  if (right) {
    right_speed_goal = -roation_speed;
    left_speed_goal = roation_speed;
  } else {
    right_speed_goal = roation_speed;
    left_speed_goal = -roation_speed;
  }
  left_speed = left_speed_goal;
  right_speed = right_speed_goal;

  run_right_motor(right_speed_goal);
  run_left_motor(left_speed_goal);
}

void update_speed_go_left(float correction) {
  if (fabs(right_speed) < 1) { // We can increase right speed
    right_speed = right_speed > 0 ? cap_speed(right_speed + correction)
                                  : cap_speed(right_speed - correction);
    run_right_motor(right_speed);
  } else { // We decrease left speed
    left_speed = left_speed > 0 ? cap_speed(left_speed - correction)
                                : cap_speed(left_speed + correction);
    run_left_motor(left_speed);
  }
}

void update_speed_go_right(float correction) {
  if (fabs(left_speed) < 1) { // We can increase left speed
    left_speed = left_speed > 0 ? cap_speed(left_speed + correction)
                                : cap_speed(left_speed - correction);
    run_left_motor(left_speed);
  } else { // We decrease right speed
    right_speed = right_speed > 0 ? cap_speed(right_speed - correction)
                                  : cap_speed(right_speed + correction);
    run_right_motor(right_speed);
  }
}

void update_speed() {
  float error = left_rps - right_rps;
  float correction = fabs(KP * error);

#ifdef DEBUG_MODE
  Serial.print("error: ");
  Serial.print(error);
  Serial.print("   corr: ");
  Serial.println(correction);
  Serial.print("left before correction: ");
  Serial.print(left_speed);
  Serial.print("   right before correction: ");
  Serial.println(right_speed);
#endif

  if (error > 0) // Left is faster --> mouse going to the right
  {
    update_speed_go_left(correction);
  } else if (error < 0) // Right is faster --> mouse going to the left
  {
    update_speed_go_right(correction);
  }

#ifdef DEBUG_MODE
  Serial.print("left after correction: ");
  Serial.print(left_speed);
  Serial.print("   right after correction: ");
  Serial.println(right_speed);
#endif
}

void uptade_speed_external_correction(EXT_CORRECTION ext_corr) {
  correction_count += DELTA_CORR;
  if (ext_corr == CORR_RIGHT) {
    update_speed_go_right(EXT_CORR_FACTOR / (float)correction_count);
  } else if (ext_corr == CORR_LEFT) {
    update_speed_go_left(EXT_CORR_FACTOR / (float)correction_count);
  }
}

void reset_speeds() {
  left_speed_goal = 0;
  right_speed_goal = 0;
  left_speed = 0;
  right_speed = 0;
}

/**
 * @brief set the new speed of the two motors --> used to go forward
 */
void set_new_speed_forward(float speed) {
  left_speed_goal = speed;
  right_speed_goal = speed;
  left_speed = speed;
  right_speed = speed;

  // If the motors are not initialized, do it
  if (last_time == 0) {
    motors_init();
  }
  last_time = millis();

  set_delta_steps_count_to_zero();

  // Set the new speed
  run_left_motor(speed);
  run_right_motor(speed);
}

// ========== Public functions ===========
RESULT break_wheels() {
  break_left(BREAKING_POWER);
  break_right(BREAKING_POWER);

  reset_speeds();
  set_delta_steps_count_to_zero();
  return NO_ERROR;
}

RESULT turn_off_motors() {
  run_left_motor(0);
  run_right_motor(0);
  // freiner bref
  reset_speeds();
  set_delta_steps_count_to_zero();
  return NO_ERROR;
}

RESULT forward(float speed, EXT_CORRECTION ext_corr) {
  speed = cap_speed(speed);
  // If the speed is different from the current speed, set the new speed
  // If the correction is different from the previous one, set the new speed
  if (left_speed_goal != speed || right_speed_goal != speed ||
      previous_correction != ext_corr) {
    correction_count = 1; // reset the correction count
    set_new_speed_forward(speed);
    last_time = millis();
    current_time = millis();
  }
  // If the speed is the same, check if the time has passed
  else {
    current_time = millis();
    unsigned long delta_time = current_time - last_time;

    // If the delta time has passed, compute the new speed
    if (delta_time >= DELTA_TIME) {
      last_time = current_time;
      new_steps_count = get_steps_count();

      left_rps = (float)(abs(new_steps_count.left_count) -
                         abs(old_steps_count.left_count)) /
                 delta_time;
      right_rps = (float)(abs(new_steps_count.right_count) -
                          abs(old_steps_count.right_count)) /
                  delta_time;

#ifdef DEBUG_MODE
      Serial.print("left");
      Serial.print(left_rps);
      Serial.print("    right");
      Serial.println(right_rps);
#endif
      if (ext_corr != NO_CORR) {
        uptade_speed_external_correction(ext_corr);
      } else {
        update_speed();
      }

      old_steps_count = new_steps_count;
    }
  }
  previous_correction = ext_corr;
  return NO_ERROR;
}

void turn_right(MODE mode, float rotation_speed) {
  if (last_time == 0) {
    motors_init();
  }
  last_time = millis();

  switch (mode) {
  case INPLACE: {
    turn_inplace(rotation_speed, true);
    break;
  }
  case SMOOTH: {
    // TODO
    break;
  }

  default:
    break;
  }
}

void turn_left(MODE mode, float rotation_speed) {
     if (last_time == 0) {
    motors_init();
  }
  last_time = millis();

  switch (mode) {
  case INPLACE: {
    turn_inplace(rotation_speed, false);
    break;
  }
  case SMOOTH: {
    // TODO
    break;
  }

  default:
    break;
  }
}
