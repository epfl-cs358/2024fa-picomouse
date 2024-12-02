#include "DFRobot_BMI160.h"

double angle = 0.0;

void reset_angle(){
    angle = 0.0;
}

double get_angle(int16_t angular_speed, double delta_time){
    angle += angular_speed * delta_time;
    return angle;
}