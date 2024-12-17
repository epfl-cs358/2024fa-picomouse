#pragma once 

#ifndef UTILS_H
#define UTILS_H

#define DEBBUG 0
// size_t
#include <cstdlib>

#define MAX_LEN_STRING 30
typedef enum {
    NO_ERROR,
    MOUSE_END,
    STACK_OVERFLOW,
    TOF_INIT_FAIL,
    GYRO_INIT_FAIL,
    GYRO_ERROR,
    TOF_READ_FAIL,
    MAZE_INIT_FAIL,
    NULL_PTR,
    NO_SOLUTION,
    OUT_OF_BOUND, 
    CANNOT_CORRECT
}RESULT;

static char error_table_translation[][MAX_LEN_STRING] = {
    "NO_ERROR",
    "MOUSE_END",
    "STACK_OVERFLOW",
    "TOF_INIT_FAIL",
    "GYRO_INIT_FAIL",
    "GYRO_ERROR",
    "TOF_READ_FAIL",
    "MAZE_INIT_FAIL",
    "NULL_PTR",
    "NO_SOLUTION",
    "OUT_OF_BOUND", 
    "CANNOT_CORRECT"
};

typedef struct {
    int x;
    int y;
}COORDINATES;

typedef struct {
    long left_count;
    long right_count;
} MOTOR_STEPS;

typedef struct {
    float left_distance;
    float right_distance;
} WHEELS_DISTANCES;

// Enum for the mode of turning
typedef enum
{
    INPLACE,
    SMOOTH
} MODE;

// Enum for the correction direction, used in the forward function
typedef enum
{
    CORR_RIGHT,
    CORR_LEFT, 
    NO_CORR
} EXT_CORRECTION;

typedef enum {
    LEFT_TURN, 
    RIGHT_TURN, 
    HALF_TURN,
    NO_TURN
} ROTATION;

typedef enum{
    NORTH,
    EAST,
    SOUTH,
    WEST
} CARDINALS;

static ROTATION direction_to_rotation[5] = {HALF_TURN, LEFT_TURN, NO_TURN, RIGHT_TURN, HALF_TURN};

static ROTATION calculate_turn(CARDINALS curr, CARDINALS target){
    int diff = target - curr;

    if(diff > 2) diff -= 4;
    if (diff < -2) diff += 4;

    diff += 2;

    return direction_to_rotation[diff];
}

#define EQUAL_COORD(coord_1, coord_2) (coord_1.x == coord_2.x && coord_1.y == coord_2.y)
#define ADD_COORD(coord, dX, dY) coord.x += dX; coord.y += dY;


#define MAX_UNSIGNED_BYTE 255

// helper macro to throw an given error if the condition is satisfied 
#define CHECK_AND_THROW(condition, error) \
    do{\
        if(condition){return error;}\
    }while(0)


/// Helper macro to propagate an error returned by a called function
#define PROPAGATE_ERROR(error)\
    do{\
        if(error){return error;}\
    }while(0)

#define DEBBUG_PRINT(SENTENCE) \
    do{\
        if(DEBBUG){\
            SENTENCE;\
        }\
    }while(0)\

//#define MODULO(angle) (((fmod((angle) + 1, 2)) <= 0 ? (fmod((angle) + 1, 2)) + 2 : fmod((angle) + 1, 2)) - 1)

#define MODULO(angle) \
    do{\
        float module = angle > 0 ? -2.0 : 2.0;\
        while(!(-1.0 < angle && angle <= 1.0)){\
            angle += module;\
        }\
    }while(0)


#endif

