#pragma once

#include "utils.h"
#include "motors.h"

// Defined speeds for both run and search mode
#define SPEED_SEARCH 0.3
#define SPEED_RUN 1

// Enum for different modes of turns
typedef enum MODE{INPLACE, SMOOTH};

/* @brief Stops the two motors
 * @param None
 * @return A RESULT value
 */
RESULT stop();

/* @brief Runs the two motors at a certain speed
 * @param The desired speed
 * @return A RESULT value
 */
RESULT forward(int speed);

/* @brief Makes the mouse turn right
 * @param A mode of turning
 * @return A RESULT value
 */
RESULT turn_right(MODE mode);

/* @brief Makes the mouse turn turn left
 * @param A mode of turning
 * @return A RESULT value
 */
RESULT turn_left(MODE mode);

