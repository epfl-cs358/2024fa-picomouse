#pragma once

#include "utils.h"
#include "motion.h"
#include "gyroscope.h"


/* @brief Adjusts the distance of the mouse with the front wall
 * @param None
 * @return A RESULT value
 */
RESULT adjust_front_distance();


 /**
  * @brief 
  * 
  * @param distance in mm
  * @return RESULT 
  */
RESULT nav_forward(float distance);

/* @brief Assures that the two motors run at the same speed
 * @param None
 * @return A RESULT value
 */
RESULT alignement();

/**
 * @brief 
 * 
 * @param rotation 
 * @param mode 
 * @return RESULT 
 */
RESULT turn(ROTATION rotation, MODE mode);

RESULT init_all_sensors();