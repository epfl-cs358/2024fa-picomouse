#pragma once
#include "utils.h"

#define MAZE_SIZE 8
#define DEBBUG 1
#define DEBBUG_PRINT(SENTENCE) \
    do{\
        if(DEBBUG){\
            SENTENCE;\
        }\
    }while(0)\
    

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

typedef enum WALL_DIR{LEFT, UP, RIGHT, DOWN};


void init_maze(Maze* maze, COORDINATES start, COORDINATES exit);

void run_maze();

void search_maze();

//void display_walls(PATH_STACK* path); // À inclure ici ou dans un éventuel ui.h ? ou nulle part :P

void add_wall(Maze* maze, WALL_DIR wall);
