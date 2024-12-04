#include "navigation.h"
#include "gyroscope.h"
#include "motion.h"
#include "utils.h"
#include <Arduino.h>

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
    double curr_angle = get_angle() + angle;
    Serial.println(curr_angle);
    MODULO_PI(curr_angle);
    bool is_close = false;
    while (!is_close) {
        if (get_angle() > curr_angle + ERROR) {
            turn_right(mode);
            Serial.println("too big");
        }
        else if (get_angle() < curr_angle - ERROR) {
            turn_left(mode);
            Serial.println("too small");
        }else {
            if (mode == INPLACE) {
                break_wheels();
                Serial.println("finished");
                is_close = true;
            }
        }
    }

    return NO_ERROR;
}

RESULT init_all_sensors(){
    setup_gyro();
    //init ToF sensors here
    return NO_ERROR;
}