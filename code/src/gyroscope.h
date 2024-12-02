#pragma once


void reset_angle();

double get_angle(int16_t angular_speed, double delta_time);

RESULT update_gyro(double threshold);

RESULT setup_gyro();