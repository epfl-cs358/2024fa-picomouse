#include "motor.h"
#include "math.h"
#include "ESP32Encoder.h"

//TODO mettre les bons pins

// Motor pins
#define MOTOR_PIN_R1 5;
#define MOTOR_PIN_R2 18;
#define MOTOR_PIN_L1 19;
#define MOTOR_PIN_L2 21;

//Encoder pins
#define ENCODER_L1 34;
#define ENCODER_L2 35;
#define ENCODER_R1 32;
#define ENCODER_R2 33;

// Max and min PWM values
#define MAX_PWM 255;
#define MIN_PWM 80;

/**
// PWM frequency
int pwm_L1;
int pwm_L2;
int pwm_R1;
int pwm_R2; */

// Direction of the motors
bool forward;
bool motorR_running;
bool motorL_running;

// Encoder object for the two motors
ESP32Encoder encoderL;
ESP32Encoder encoderR;


// =========== Public functions ===========


RESULT motors_init(){
	// ---- Setup motor -----

	motorR_running = false;
	motorL_running = false;

	// Set the pins as output
	pinMode(MOTOR_PIN_R1, OUTPUT);
	pinMode(MOTOR_PIN_R2, OUTPUT);
	pinMode(MOTOR_PIN_L1, OUTPUT);
	pinMode(MOTOR_PIN_L2, OUTPUT);

	/**
	// PWM frequency
	int pwm_L1 = 0;
	int pwm_L2 = 0;
	int pwm_R1 = 0;
	int pwm_R2 = 0; */
	forward = true;

	// ----- Setup encoder ----
	// Enable the weak pull up resistors
	ESP32Encoder::useInternalWeakPullResistors = puType::up;

	// Attach the encoders to the pins
    //4 count per rotation
	encoderL.attachHalfQuad(ENCODER_L1, ENCODER_L2);
	encoderR.attachHalfQuad(ENCODER_R1, ENCODER_R2);

	// Set the initial count to 0
	encoderL.clearCount();
	encoderR.clearCount();

	return NO_ERROR;
}

RESULT stop_motor(){
	motorR_running = false;
	motorL_running = false;
	run_left_motor(0);
	run_right_motor(0);	
	return NO_ERROR;
}

MOTOR_STEPS get_steps_count(){
	// Counters for steps of the two motors
	MOTOR_STEPS motor_steps;

	motor_steps.left = encoderL.getCount();
	motor_steps.last_toogle = millis();
	motor_steps.right = encoderR.getCount();

	return motor_steps;
}

RESULT reset_counter(){
    encoderL.clearCount();
    encoderR.clearCount();
	return NO_ERROR;
}


RESULT run_left_motor(float speed){
	if(speed == 0){
		motorL_running = false;
	}
    int PWM = speed_to_pwm(speed);
	run_motor(PWM, false);
	return NO_ERROR;
}

RESULT run_right_motor(float speed){
	int PWM = speed_to_pwm(speed);
	run_motor(PWM, true);
	return NO_ERROR;
}






// =========== Private functions ===========
/**
 * @brief Run the given motor at a certain speed
 * @param int PWM The speed of the motor
 * @param bool right_motor True if the motor is the right one
 * @return None
 */
void run_motor(int PWM, bool right_motor){
	int pin1; 
	int pin2;

	// Set the pins
	if(right_motor){
		pin1 = MOTOR_PIN_R1;
		pin2 = MOTOR_PIN_R2;
	}
	else{
		pin1 = MOTOR_PIN_L1;
		pin2 = MOTOR_PIN_L2;
	}

	// Apply the PWM with the right direction
	if (direction){
		analogWrite(pin1, PWM);
		analogWrite(pin2, LOW);
	} else {
		analogWrite(pin1, LOW);
		analogWrite(pin2, PWM);
	}
}


/**
	* @brief take the desired speed and limits the speed (PWM) to the maximum values allowed ([-255;255])
	*
	* @param float speed The speed about to be scaled
	* @return A capped speed
	* @retval int in range [-255, 255]
*/
int speed_to_pwm(float speed){
	// Set the direction
	forward = (speed >= 0);
	if (speed == 0){
		return 0;
	}

	// Cap the speed
	speed = speed > 1 ? 1 : speed;
	speed = speed < -1 ? -1 : speed;

	// Calculate the PWM
	int PWM = (int) (MIN_PWM + abs(speed) * (MAX_PWM - MIN_PWM));
	return PWM;
}





