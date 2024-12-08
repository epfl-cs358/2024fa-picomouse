#pragma once 

#include "maze.h"

#define MAZE_SIZE 8


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


int dx[] = {0, -1, 0, 1};
int dy[] = {-1, 0, 1, 0};
int WALLS[] = {LeftWall, UpWall, RightWall, DownWall};

RESULT init_maze(Maze* maze, COORDINATES start, COORDINATES exit);
void add_wall(Maze* maze, WALL_DIR wall);
RESULT one_iteration_flood_fill(Maze* maze, PATH_STACK* path_stack, CARDINALS* next_direction);
RESULT init_stack(PATH_STACK* stack);