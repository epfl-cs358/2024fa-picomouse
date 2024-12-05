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

// Enum for the mode of turning
typedef enum
{
    INPLACE,
    SMOOTH
} MODE;

// Enum for the correction direction, used in the forward function
typedef enum
{
    CORR_RIGHT,
    CORR_LEFT, 
    NO_CORR
} EXT_CORRECTION;

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
 * CALL IT EVEN IF THE SPEED DOESN'T CHANGE --> it addapt if one wheel is too slow each DELTA_TIME
 * Never stop running --> give a next instruction to stop
 * @param The desired speed
 * @param The correction factor if need to turn a bit in a direction (if not alligned between the two walls)
 * @return A RESULT value
 */

//TODO IMPLEMENT THE CORRECTION FACTOR -->
RESULT forward(float speed, EXT_CORRECTION ext_corr = NO_CORR);

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
