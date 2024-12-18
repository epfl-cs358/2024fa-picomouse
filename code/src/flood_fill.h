/**
 * @file flood_fill.h
 * @author AUMONT Adrien, KNABENHANS Felix
 * @copyright Copyright (c) 2024
 */

#pragma once 

#include "maze.h"
#include "utils.h"

// binary encoding of the walls 
#define LeftWall 1
#define UpWall 2
#define RightWall 4
#define DownWall 8


typedef struct {
    COORDINATES stack[MAZE_SIZE*MAZE_SIZE];
    size_t end;
    size_t max_size;
}PATH_STACK;


#define IN_BOUNDARIES(coord) (0 <= coord.x && coord.x < MAZE_SIZE && 0 <= coord.y && coord.y < MAZE_SIZE)


static int dx[] = {0, -1, 0, 1};
static int dy[] = {-1, 0, 1, 0};
static int WALLS[] = {LeftWall, UpWall, RightWall, DownWall};


RESULT init_maze(Maze* maze, COORDINATES start, COORDINATES exit);
RESULT init_stack(PATH_STACK* stack);

/**
 * @brief Add a given wall to the maze in the current cell.
 * @param maze 
 * @param wall wall to add
 * @return RESULT 
 */
RESULT add_wall(Maze* maze, WALL_DIR wall);

/**
 * @brief Run the flood fill algorithm on the maze to get the next cell and adds it to the given path
 * @param maze 
 * @param path_stack 
 * @param next_direction returned value 
 * @return RESULT 
 */
RESULT one_iteration_flood_fill(Maze* maze, PATH_STACK* path_stack, CARDINALS* next_direction);


