#pragma once

#include "utils.h"

/* @brief Calibrates the mouse so it's able to search/run the maze
 * @parm None
 * @return A RESULT value
 */
RESULT init_pos(DFRobot_BMI160* bmi160, double* offset);
