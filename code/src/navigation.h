#pragma once

#include "utils.h"
#include "motion.h"
#include "gyroscope.h"


typedef enum {
    LEFT, 
    RIGHT, 
    HALF_TURN,
    NO_TURN
} ROTATION;

/* @brief Adjusts the distance of the mouse with the front wall
 * @param None
 * @return A RESULT value
 */
RESULT adjust_front_distance();

/* @brief Adjusts the distance of the mouse with the side walls
 * @param None
 * @return A RESULT value
 */
RESULT adjust_sides_distance();

/* @brief Assures that the two motors run at the same speed
 * @param None
 * @return A RESULT value
 */
RESULT alignement();

RESULT turn(ROTATION rotation, MODE mode);

RESULT init_all_sensors();