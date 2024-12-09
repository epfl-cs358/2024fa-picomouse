#pragma once 

#ifndef UTILS_H
#define UTILS_H

#define DEBBUG 1
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
    NO_SOLUTION
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
    "NO_SOLUTION"
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
    DEBBUG_PRINT(Serial.printf("Before modulo: % 0.6f", angle));\
    float module = angle > 0 ? -2.0 : 2.0;\
    while(!(-1.0 < angle && angle <= 1.0)){\
        angle += module;\
    }\
    DEBBUG_PRINT(Serial.printf("After Modulo: % 0.6f", angle));\
    }while(0)


#endif

