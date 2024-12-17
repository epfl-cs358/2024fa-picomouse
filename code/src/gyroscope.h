#pragma once
#include "utils.h"

void reset_angle(float new_angle);

float get_angle();

RESULT update_gyro();

RESULT init_gyro();

RESULT compute_offset();