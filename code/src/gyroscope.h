/**
 * @file gyroscope.h
 * @author KACEM Sophia, AUMONT Adrien
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "utils.h"

void reset_angle(float new_angle);

float get_angle();

RESULT update_gyro();

RESULT init_gyro();

RESULT compute_offset();