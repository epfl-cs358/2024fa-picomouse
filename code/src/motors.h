#pragma once
#ifndef MOTORS_H
#define MOTORS_H

#include "utils.h"

// TODO mettre les bons pins
// Motor pins
#define MOTOR_PIN_R1 27
#define MOTOR_PIN_R2 26
#define MOTOR_PIN_L1 14
#define MOTOR_PIN_L2 12

// Encoder pins
#define ENCODER_R1 23
#define ENCODER_R2 19
#define ENCODER_L1 15
#define ENCODER_L2 2

// Max and min PWM values
#define MAX_PWM 255
#define MIN_PWM 40

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

/* SHOULD NEVER BE USED
 * @brief Resets the two counters
 * @param None
 * @return A RESULT value
 */
RESULT reset_encoder_counter();

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


/* @brief Active breaking on the left motor
 * @param The breaking power [0;1]
 * @return A RESULT value
 */
RESULT break_left(float breaking_power);

/* @brief Active breaking on the right motor
 * @param The breaking power [0;1]
 * @return A RESULT value
 */
RESULT break_right(float breaking_power);

#endif
