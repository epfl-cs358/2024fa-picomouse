#pragma once

#include utils.h

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

/* @brief Adjusts the total angle of the mouse by using the gyroscope
 * @param None
 * @return A RESULT value
 */
RESULT adjust_angle();
