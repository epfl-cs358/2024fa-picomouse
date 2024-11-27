#pragma once

#include "utils.h"

// Array of all distances detected by sensors 
double distances[5];

/* @brief Enables sensors
 * @param None
 * @return A RESULT value
 */
RESULT enable();

/* @brief Disables sensors
 * @param None
 * @return A RESULT value
 */
RESULT disable();

/* @brief Updates value of 'distances[5]'
 * @details Calculate an average of 15 values per sensor
 * @param None
 * @return A RESULT value
 */
RESULT get_distances();
