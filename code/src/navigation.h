#pragma once

#include "utils.h"
#include "motion.h"
#include "gyroscope.h"


/* @brief Adjusts the distance of the mouse with the front wall
 * @param None
 * @return A RESULT value
 */
RESULT adjust_front_distance();


 


/* @brief Assures that the two motors run at the same speed
 * @param None
 * @return A RESULT value
 */
RESULT alignement();

RESULT init_all_sensors();

RESULT navigation_forward(float distance);

RESULT turn(ROTATION rotation, MODE mode);