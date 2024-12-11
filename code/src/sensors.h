#pragma once

#include "utils.h"
#include "maze.h"
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

typedef struct {
  float front_distance_mid;
  float front_distance_left;
  float front_distance_right;
}POSITION_TO_FRONT;

RESULT position_to_wall(POSITION_TO_WALL* result, CALC_CHOICE choice);
RESULT position_to_front(POSITION_TO_FRONT* result);

RESULT init_TOF();

RESULT detect_walls(WALL_DIR* result, int* n_walls_found, CARDINALS mouse_direction);

RESULT update_left();
RESULT update_right();
RESULT update_mid();
RESULT update_all();
