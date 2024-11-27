#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;


#define MAZE_SIZE 8


#define LeftWall 1
#define UpWall 2
#define RightWall 4
#define DownWall 8

#define IN_BOUNDARIES(X,Y) (0 <= X && X < MAZE_SIZE && 0 <= Y && Y < MAZE_SIZE)

char matrix[MAZE_SIZE][MAZE_SIZE];
typedef struct {
    int x;
    int y;
}COORDINATES;

typedef enum WALLS_DIR{LEFT, UP, RIGHT, DOWN};

typedef struct{
    unsigned char matrix[MAZE_SIZE][MAZE_SIZE];
    unsigned char walls[MAZE_SIZE][MAZE_SIZE];
    COORDINATES mouse_pos;
    COORDINATES exit;
    COORDINATES start;
}Maze;

Maze maze;
int dx[] = {0, -1, 0, 1};
int dy[] = {-1, 0, 1, 0};
int WALLS[] = {LeftWall, UpWall, RightWall, DownWall};


void innit_maze(COORDINATES start, COORDINATES exit){
    maze.start = start;
    maze.exit = exit;
    maze.mouse_pos = start;
    std::memset(maze.walls, 0, MAZE_SIZE*MAZE_SIZE);
    std::memset(maze.matrix, 255, MAZE_SIZE*MAZE_SIZE);
}

void weighted_BFS(Maze* maze, COORDINATES current_cell, unsigned char current_distance) {
    // If this cell's distance is already smaller, stop recursion
    if (WEIGHT_AT(maze->matrix, current_cell) <= current_distance) {
        return;
    }
    // Update the cell's distance
    WEIGHT_AT(maze->matrix, current_cell) = current_distance;

    // Explore all four directions
    for (int dir = 0; dir < 4; ++dir) {
        // Check if there's a wall blocking this direction
        if (WALLS_AT(maze->walls, current_cell) & WALLS[dir]){
            continue;
        }
        COORDINATES new_pos = current_cell;
        ADD_COORD(new_pos, dx[dir], dy[dir]);
        
        if(IN_BOUNDARIES(new_pos)){
            weighted_BFS(maze, new_pos, current_distance+1);
        }
    }
}


void pop_n(size_t n, PATH_STACK* stack){
    stack->end -= n;
}

RESULT push(COORDINATES new_coord, PATH_STACK* stack){
    if(stack->end >= stack->max_size){
        return STACK_OVERFLOW;
    }
    for (size_t i = 0; i < stack->end; i++){
        if(EQUAL_COORD(stack->stack[i], new_coord)){
            pop_n(stack->end-i-1, stack);
            return NO_ERROR;
        }
    }
    stack->stack[stack->end] = new_coord;
    stack->end++;

    return NO_ERROR;
}


void add_wall(Maze* maze, WALLS_DIR wall){
    size_t wall_index = static_cast<size_t>(wall);
    // add the wall to the current cell
    WALLS_AT(maze->walls, maze->mouse_pos) |= WALLS[wall_index];

    COORDINATES adjacent_cell = maze->mouse_pos;
    ADD_COORD(adjacent_cell, dx[wall_index], dy[wall_index]);

    if(IN_BOUNDARIES(adjacent_cell)){
        // adjacent wall offset index  = (wall_index+2)%4
        WALLS_AT(maze->walls, adjacent_cell) |= WALLS[(wall_index+2)&3];
    }
}



RESULT one_iteration_flood_fill(Maze* maze, PATH_STACK* path_stack){
    push(maze->mouse_pos, path_stack);
    if(EQUAL_COORD(maze->mouse_pos, maze->exit)){
        return MOUSE_END;
    }
        
    // reset the weight matrix
    std::memset(maze->matrix, MAX_UNSIGNED_BYTE, MAZE_SIZE*MAZE_SIZE);
    weighted_BFS(maze, maze->exit, 0);
    
    unsigned char current_min = MAX_UNSIGNED_BYTE;
    int min_dir = 0;
    for (size_t i = 0; i < 4; i++){
        if(WALLS[i] & WALLS_AT(maze->walls, maze->mouse_pos)){
            continue;
        }
        if(current_min > maze->matrix[maze->mouse_pos.x+dx[i]][maze->mouse_pos.y+dy[i]]){
            current_min = maze->matrix[maze->mouse_pos.x+dx[i]][maze->mouse_pos.y+dy[i]];
            min_dir = i;
        }
    }
    ADD_COORD(maze->mouse_pos, dx[min_dir], dy[min_dir]);   

    return NO_ERROR;
}






