#pragma once
#include "utils.h"

void reset_angle();

double get_angle();

RESULT update_gyro(double threshold);

RESULT setup_gyro();