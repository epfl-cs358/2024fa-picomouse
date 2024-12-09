#include <cstring>
#include "utils.h"
#include "flood_fill.h"
#include "maze.h"


#define WEIGHT_AT(weight_matrix, coord) weight_matrix[coord.x][coord.y]
#define WALLS_AT(wall_matrix, coord) wall_matrix[coord.x][coord.y]


RESULT init_maze(Maze* maze, COORDINATES start, COORDINATES exit){

    CHECK_AND_THROW(!IN_BOUNDARIES(start), MAZE_INIT_FAIL);
    CHECK_AND_THROW(!IN_BOUNDARIES(exit), MAZE_INIT_FAIL);

    maze->start = start;
    maze->exit = exit;
    maze->mouse_pos = start;

    std::memset(maze->walls, 0, MAZE_SIZE*MAZE_SIZE);
    std::memset(maze->matrix, MAX_UNSIGNED_BYTE, MAZE_SIZE*MAZE_SIZE);

    return NO_ERROR;    
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

/**
 * @brief Pop n values on the stack, those values are not used (neither returned).
 * 
 * @param n Number of element to pop
 * @param stack 
 * @return RESULT STACK_OVERFLOW (underflow) / NO_ERROR
 */
RESULT pop_n(size_t n, PATH_STACK* stack){
    CHECK_AND_THROW(n > stack->end, STACK_OVERFLOW);
    stack->end -= n;
}

/**
 * @brief Push the coordinate to the stack and also removes loops in the path.
 * 
 * @param new_coord Coordinate to add on the stack 
 * @param stack 
 * @return RESULT STACK_OVERFLOW / NO_ERROR
 */
RESULT push(COORDINATES new_coord, PATH_STACK* stack){
    CHECK_AND_THROW(stack->end >= stack->max_size, STACK_OVERFLOW);

    for (size_t i = 0; i < stack->end; i++){
        if(EQUAL_COORD(stack->stack[i], new_coord)){
            RESULT err = pop_n(stack->end-i-1, stack);
            PROPAGATE_ERROR(err);
            return NO_ERROR;
        }
    }
    stack->stack[stack->end] = new_coord;
    stack->end++;

    return NO_ERROR;
}


RESULT add_wall(Maze* maze, WALL_DIR wall){
    CHECK_AND_THROW(!maze, NULL_PTR);
    size_t wall_index = static_cast<size_t>(wall);
    // add the wall to the current cell
    WALLS_AT(maze->walls, maze->mouse_pos) |= WALLS[wall_index];

    COORDINATES adjacent_cell = maze->mouse_pos;
    ADD_COORD(adjacent_cell, dx[wall_index], dy[wall_index]);

    if(IN_BOUNDARIES(adjacent_cell)){
        // adjacent wall offset index  = (wall_index+2)%4
        WALLS_AT(maze->walls, adjacent_cell) |= WALLS[(wall_index+2)&3];
    }

    return NO_ERROR;
}


RESULT one_iteration_flood_fill(Maze* maze, PATH_STACK* path_stack, CARDINALS* next_direction){
    CHECK_AND_THROW(!path_stack, NULL_PTR);
    CHECK_AND_THROW(!maze, NULL_PTR);
    RESULT err = push(maze->mouse_pos, path_stack);
    PROPAGATE_ERROR(err);
    
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

    int delta_x = dx[min_dir];
    int delta_y = dy[min_dir];

    ADD_COORD(maze->mouse_pos, delta_x, delta_y);  

    if (delta_x == 0) {
        if (delta_y == -1) {
            *next_direction = WEST;
        }
        else if (delta_y == 1) {
            *next_direction = EAST;
        }
    }
    else if (delta_y == 0) {
        if (delta_x == -1) {
            *next_direction = NORTH;
        }
        else if (delta_x == 1) {
            *next_direction = SOUTH;
        }
    }
    else {
        return NO_SOLUTION;
    }

    return NO_ERROR;
}

RESULT init_stack(PATH_STACK* stack){
    CHECK_AND_THROW(!stack, NULL_PTR);
    stack->end = 0;
    stack->max_size = MAZE_SIZE*MAZE_SIZE;

    return NO_ERROR;
}