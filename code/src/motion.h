#pragma once

#ifndef MOTION_H
#define MOTION_H

#include "utils.h"
#include "motors.h"

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
#define KP 0.05

// Speed of the motors when a turning function is called (INPLACE mode)
#define ROTATION_SPEED 0.1

// Defined speeds for both run and search mode
#define SPEED_SEARCH 0.2
#define SPEED_RUN 1

#define DELTA_TIME 50 // ms, the time between two measures of the counter

// Breaking power [0;1]
#define BREAKING_POWER 0.5 

// Enum for different modes of turns
// TODO implement smooth turn
typedef enum
{
    INPLACE,
    SMOOTH
} MODE;

/* @brief Stops the two motors
 * @param None
 * @return A RESULT value
 */
RESULT stop();

/* @brief Active breaking.
 * @param The breaking power [0;1]
 * @return A RESULT value
 */
RESULT break_wheels();

/* @brief Runs the two motors at a certain speed [-1;1]
 * CALL IT EVEN IF THE SPEED DOESN'T CHANGE --> it addapt if one wheel is too slow each DELTA_TIME
 * Never stop running --> give a next instruction to stop
 * @param The desired speed
 * @return A RESULT value
 */
RESULT forward(float speed);

/* @brief Makes the mouse turn right. NEVER STOP ROTATING --> give a next instruction to stop
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
