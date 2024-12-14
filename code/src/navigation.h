#pragma once

#include "gyroscope.h"
#include "motion.h"
#include "utils.h"

/**
 * @brief Initialize all the sensors
 * @param None
 * @return A RESULT value
 */
RESULT init_all_sensors();

/**
 * @brief forward for a given distance.
 * @param distance in mm
 * @param max_speed [0;1]
 * @return RESULT
 */
RESULT navigation_forward(float distance, float max_speed);

/**
 * @brief Turn the mouse
 * @param rotation the rotation to do
 * @param mode the mode of the turn INPLACE or SMOOTH
 * @return A RESULT value
 */
RESULT turn(ROTATION rotation, MODE mode);