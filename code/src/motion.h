#pragma once

#ifndef MOTION_H
#define MOTION_H

#include "utils.h"
#include "motors.h"

// Correction factor for the speed
#define KP 0.05

#define ROTATION_SPEED 0.1



// Defined speeds for both run and search mode
#define SPEED_SEARCH 0.2
#define SPEED_RUN 1

#define DELTA_TIME 100 //ms, the time between two measures of the counter

// Enum for different modes of turns
//TODO implement smooth turn
typedef enum {INPLACE, SMOOTH}MODE;

/* @brief Stops the two motors
 * @param None
 * @return A RESULT value
 */
RESULT stop();
void set_new_speed_forward(float speed);

/* @brief Runs the two motors at a certain speed [-1;1]
 * CALL IT EVEN IF THE SPEED IS THE SAME --> it addapt if one wheel is too slow
 * @param The desired speed
 * @return A RESULT value
 */
RESULT forward(float speed);

/* @brief Makes the mouse turn right. NEVER STOP ROTATING --> --> give a next instruction to stop
 * @param A mode of turning
 * @return A RESULT value
 */
void turn_right(MODE mode);

/* @brief Makes the mouse turn turn left. NEVER STOP ROTATING --> give a next instruction to stop
 * @param A mode of turning
 * @return A RESULT value
 */
void turn_left(MODE mode);

#endif
