#pragma once
#include "utils.h"

#define MAZE_SIZE 6
#define CELL_SIZE 180
#define START_ORIENTATION EAST

#define LeftWall 1
#define UpWall 2
#define RightWall 4
#define DownWall 8


typedef struct{
    unsigned char matrix[MAZE_SIZE][MAZE_SIZE];
    unsigned char walls[MAZE_SIZE][MAZE_SIZE];
    COORDINATES mouse_pos;
    COORDINATES exit;
    COORDINATES start;
}Maze;

typedef enum {LEFT, UP, RIGHT, DOWN}WALL_DIR;


