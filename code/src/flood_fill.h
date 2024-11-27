#pragma once 

#define MAZE_SIZE 8


#define LeftWall 1
#define UpWall 2
#define RightWall 4
#define DownWall 8

#define INIT_STACK(stack) stack.end = 0;

typedef struct {
    COORDINATES stack[MAZE_SIZE*MAZE_SIZE];
    size_t end;
    size_t max_size;
}PATH_STACK;


#define IN_BOUNDARIES(coord) (0 <= coord.x && coord.x < MAZE_SIZE && 0 <= coord.y && coord.y < MAZE_SIZE)

typedef enum WALLS_DIR{LEFT, UP, RIGHT, DOWN};Maze maze;
int dx[] = {0, -1, 0, 1};
int dy[] = {-1, 0, 1, 0};
int WALLS[] = {LeftWall, UpWall, RightWall, DownWall};