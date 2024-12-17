#pragma once

#include "utils.h"
#include "maze.h"
#include <cstdint>
#include <math.h>


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

typedef enum {CALC_LEFT, CALC_RIGHT, CALC_BOTH}CALC_CHOICE;

void get_side_side_distances(uint16_t* dist);

/**
 * @brief Calculates the position of the mouse to the walls and the deviation
 * @param result pointer to the POSITION_TO_WALL output 
 * @param choice CALC_CHOICE
 * @return RESULT 
 */
RESULT position_to_wall(POSITION_TO_WALL* result, CALC_CHOICE choice);
/**
 * @brief Calculates the position to the front wall.
 * @param result pointer to the POSITION_TO_FRONT output
 * @return RESULT 
 */
RESULT position_to_front(POSITION_TO_FRONT* result);

/**
 * @brief detect the wall(s) in a cell.
 * 
 * @param result buffer of WALL_DIR (assumed to be large enough, size == 4)
 * @param n_walls_found pointer to the result number of walls found
 * @param mouse_direction The current mouse direction
 * @return RESULT 
 */
RESULT detect_walls(WALL_DIR* result, int* n_walls_found, CARDINALS mouse_direction);

RESULT init_TOF();
RESULT update_left();
RESULT update_right();
RESULT update_mid();
RESULT update_all();
