#pragma once

#include utils.h

// Counters for steps of the two motors
int l_motor_steps;
int r_motor_steps;

/* @brief Retrieves steps counts
 * @param None
 * @return An array constituted of l_motor_steps and r_motor_steps
 */
int* get_steps_count();

/* @brief Calculate speed of motors
 * @param None
 * @return A double value corresponding to the speed
 */
double get_speed();

/* @brief Resets the two counters
 * @param None
 * @return A RESULT value
 */
RESULT reset_counter();

/* @brief Sets left motor's speed
 * @param The desired speed
 * @return A RESULT value
 */
RESULT run_left_motor(int speed);

/* @brief Sets right motor's speed
 * @param The desired speed
 * @return A RESULT value
 */
RESULT run_right_motor(int speed);
