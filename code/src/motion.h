#pragma once

#ifndef MOTION_H
#define MOTION_H

#include "motors.h"
#include "utils.h"

/** This library control the two motors of the mouse
 * It uses a special encoder library wich uses the Serial with 115200 bauds
 * Exemple of a simple use of forward
 *
 * void setup(){ Serial.begin(115200); }
 *
 * int 1 = 0;
 * void loop(){
 * forward(0.2);
 * i++;
 * delay(50);
 * if(i == 100){
 *   i = 0;
 *   stop();
 *   delay(1000);
 * }
 * }
 */

// Correction factor for the speed
#define KP 0.1

#define DELTA_TIME 50 // ms, the time between two measures of the counter

// Breaking power [0;1]
#define BREAKING_POWER 1

// Correction angle
#define CORRECTION_ANGLE 1.0 / 16.0

// Correction factor if there is a CORR_RIGHT or CORR_LEFT
#define EXT_CORR_FACTOR 1.5
#define EXT_CORR_MULT 10

// Wheels characteristics
#define WHEEL_DIAMETER 38.0                    // mm
#define WHEEL_PERMIMER WHEEL_DIAMETER * 3.1415 // mm
#define ENCODER_RESOLUTION 408.0               // 408 pulse per revolution

/*
 * @brief Initialize the two motors and the two encoders
 */
RESULT init_motors();

/* @brief reset to 0 the traveled distance
 * @param None
 * @return a RESULT value
 */
void reset_traveled_distance();

/* @brief get the traveled distance in mm
 * @param None
 * @return the traveled distance
 */
WHEELS_DISTANCES get_traveled_distance();

/* @brief Stops the two motors
 * @param None
 * @return A RESULT value
 */
RESULT turn_off_motors();

/* @brief Acti
#define CORRECTION_ANGLE 1.0/16.0ve breaking.
 * @param The breaking power [0;1]
 * @return A RESULT value
 */
RESULT break_wheels();

/* @brief Runs the two motors at a certain speed [-1;1]
 * CALL IT EVEN IF THE SPEED DOESN'T CHANGE --> it addapt if one wheel is too
 * slow each DELTA_TIME Never stop running --> give a next instruction to stop
 * @param The desired speed
 * @param The correction factor if need to turn a bit in a direction (if not
 * alligned between the two walls) ATTENTION; ITS FAST, CALL IT A VERY SHORT
 * TIME
 * @return A RESULT value
 */

// TODO IMPLEMENT THE CORRECTION FACTOR -->
RESULT forward(float speed, EXT_CORRECTION ext_corr = NO_CORR);

/* @brief Makes the mouse turn right. NEVER STOP ROTATING --> give a next
 * instruction to stop
 * @param A mode of turning
 * @param The speed of the rotation [0,1]
 * @return A RESULT value
 */
void turn_right(MODE mode, float rotation_speed);

/* @brief Makes the mouse turn turn left. NEVER STOP ROTATING --> give a next
 * instruction to stop
 * @param A mode of turning
 * @param The speed of the rotation [0,1]
 * @return A RESULT value
 */
void turn_left(MODE mode, float rotation_speed);

#endif
