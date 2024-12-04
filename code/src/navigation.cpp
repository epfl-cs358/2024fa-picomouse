#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"

#define ERROR 1.0/16.0

RESULT adjust_front_distance(){
    return NO_ERROR;
}

RESULT adjust_sides_distance(){
    return NO_ERROR;
}

RESULT alignement(){
    return NO_ERROR;
}

RESULT turn(double angle, MODE mode){
    update_gyro(0.00150);
    double current_angle = get_angle();
    double target_angle = current_angle + angle;
    if (get_angle() > angle + ERROR) {
        turn_right(mode);
    }
    else if (get_angle() < angle - ERROR) {
        turn_left(mode);
    }else {
        if (mode == INPLACE) {
            break_wheels();
        }
    }
    return NO_ERROR;
}

RESULT init_all_sensors(){
    setup_gyro();
    //init ToF sensors here
    return NO_ERROR;
}