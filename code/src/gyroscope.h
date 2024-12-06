#pragma once
#include "utils.h"

void reset_angle();

double get_angle();

RESULT update_gyro(double offset);

RESULT setup_gyro();

double compute_offset();