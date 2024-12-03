#pragma once

#include "utils.h"

// Array of distances detected by sensors 
// in order LEFT MID_LEFT MID_RIGHT RIGHT
uint_8 side_distances[4];
size_t side_update_time[4];

double mid_distance = 0;
size_t mid_update_time = 0;


RESULT innit_TOF();

RESULT update_left();
RESULT update_right();
RESULT update_mid();
RESULT update_all();
