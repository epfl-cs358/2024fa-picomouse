#include "motor.h"
#include "ESP32Encoder.h"

ESP32Encoder encoderL;
ESP32Encoder encoderR;

unsigned long encoderL_last_Toogle = 0;
unsigned long encoderR_last_Toogle = 0;

// =========== Public functions ===========


void motors_init(){
	// Enable the weak pull up resistors
	ESP32Encoder::useInternalWeakPullResistors = puType::up;

	// Attach the encoders to the pins
    // One edge --> +1 each rotation of the motor 
    //One can use attachHalfQuad instead of attachSingleEdge --> use 2 pin, 4 count per rotation
	encoderL.attachSingleEdge(ENCODER_L, true);
	encoderR.attachSingleEdge(ENCODER_R, true);

	// Set the initial count to 0
	encoderL.clearCount();
	encoderR.clearCount();

	// Set the last toogle time to the current time
	encoderL_last_Toogle = millis();
	encoderR_last_Toogle = millis();
}

void reset_counter(){
    encoderL.clearCount();
    encoderR.clearCount();
}

void stop_robot(){
    run_left_motor(0);
    run_right_motor(0);
}

void run_left_motor(int speed){
    speed = limit_speed(speed);
    //TODO faire tourner le moteur
}

void run_rig





// =========== Private functions ===========
/**
	* @brief Limits the speed (PWM) to the maximum values allowed ([-255;255])
	*
	* @param (int speed) The speed about to be checked and capped 
	* @return A capped speed
	* @retval int in range [-255, 255]
*/
int limit_speed(int speed){
	if(speed > 255){
		speed = 255;
	}else if(speed < -255){
		speed = -255;
	}
	return speed;
}





