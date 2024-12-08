#pragma once

#include "utils.h"
#include <cstdint>
#include <math.h>
typedef enum {CALC_LEFT, CALC_RIGHT, CALC_BOTH}CALC_CHOICE;
float d_calc[4];
float teta_calc[4];
// Array of distances detected by sensors 
// in order LEFT MID_LEFT MID_RIGHT RIGHT
uint16_t side_distances[4];
size_t side_update_time[4];
float alphas[4] = {0.7669, 0.384, 0.384, 0.7669};
float cs[4] = {57.007, 62.533, 62.533, 57.007};
float betas[4] = {0.5426, 0.4021, 0.4021, 0.5426};

float mid_distance = 0;
size_t mid_update_time = 0;

typedef struct {
  float distance_right;
  float distance_left;
  float deviation_left;
  float deviation_right;
}POSITION_TO_WALL;
RESULT innit_TOF();

RESULT update_left();
RESULT update_right();
RESULT update_mid();
RESULT update_all();
