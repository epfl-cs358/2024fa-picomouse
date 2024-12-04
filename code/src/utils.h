#pragma once 

#ifndef UTILS_H
#define UTILS_H

// size_t
#include <cstdlib>

typedef enum {
    NO_ERROR,
    MOUSE_END,
    STACK_OVERFLOW,
    TOF_INNIT_FAIL,
    TOF_READ_FAIL
}RESULT;

typedef struct {
    int x;
    int y;
}COORDINATES;

typedef struct {
    long left_count;
    long right_count;
} MOTOR_STEPS;

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


#endif

