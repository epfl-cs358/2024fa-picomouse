#pragma once
#ifndef MOTORS_H
#define MOTORS_H

#include "utils.h"

/* @brief Initialize the motor and the encoders. Set the count to 0 and set the last last toogle time to millis()
* @param None
* @return a RESULT value 
*/
RESULT motors_init();


/* @brief Retrieves steps counts and the time of the last toogle
 * @param None
 * @return a struct containing the steps count and the time of the last toogle
 */
MOTOR_STEPS get_steps_count();

/* @brief Calculate speed of motors
 * @param None
 * @return A double value corresponding to the speed
 */
double get_speed(); //TODO je pense pas l'implementer

/* @brief Resets the two counters
 * @param None
 * @return A RESULT value
 */
RESULT reset_counter();

/* @brief Sets left motor's speed, value between [-1;1]
 * @param The desired speed
 * @return A RESULT value
 */
RESULT run_left_motor(float speed);

/* @brief Sets right motor's speed, value between [-1;1]
 * @param The desired speed
 * @return A RESULT value
 */
RESULT run_right_motor(float speed);

#endif
