#pragma once

#include "utils.h"
#include <cstdint>
#include <math.h>

typedef enum {CALC_LEFT, CALC_RIGHT, CALC_BOTH}CALC_CHOICE;

void get_side_side_distances(uint16_t* dist);



typedef struct {
  float distance_right;
  float distance_left;
  float deviation_left;
  float deviation_right;
}POSITION_TO_WALL;

RESULT position_to_wall (POSITION_TO_WALL* result, CALC_CHOICE choice);

RESULT init_TOF();

RESULT detect_walls (WALL_DIR* result, int* n_walls_found, CARDINALS mouse_direction)

RESULT update_left();
RESULT update_right();
RESULT update_mid();
RESULT update_all();
