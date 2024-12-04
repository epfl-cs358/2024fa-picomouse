#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"

#define ERROR 1.0/16.0

RESULT adjust_front_distance(){

}

RESULT adjust_sides_distance(){

}

RESULT alignement(){

}

RESULT turn(double angle, MODE mode){
    update_gyro(0.00150);
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
}

RESULT init_all_sensors(){
    setup_gyro();
    //init ToF sensors here
}