#pragma once
#include "utils.h"

#define GYRO_CALIBRATION_NB_ITERATIONS 3

void reset_angle();

double get_angle();

RESULT update_gyro(double threshold);

RESULT setup_gyro();