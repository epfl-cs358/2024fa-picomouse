
#pragma once 
#include "utils.h"
#include "maze.h"
#include "flood_fill.h"




/**
 * @brief Runs the mouse in the maze optimizing the path for the given RUN_POLICY.
 * It optimize the path by removing unnecessary stops in the path (one forward  for mutiple cells).
 * @warning Only the shortest path is implemented for now.
 * @param path_1 The first path of the maze traversal (absolute start_cell to absolute exit_cell)
 * @param path_2 The second path of the maze traversal (absolute exit_cell to absolute start_cell)
 * @return RESULT 
 */
RESULT run(PATH_STACK* path_1, PATH_STACK* path_2, float run_speed);
