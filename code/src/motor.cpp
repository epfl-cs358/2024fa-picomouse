#include "motors.h"
#include "math.h"
#include "ESP32Encoder.h"

// TODO mettre les bons pins

// Motor pins
#define MOTOR_PIN_R1 5;
#define MOTOR_PIN_R2 18;
#define MOTOR_PIN_L1 19;
#define MOTOR_PIN_L2 21;

// Encoder pins
#define ENCODER_L1 34;
#define ENCODER_L2 35;
#define ENCODER_R1 32;
#define ENCODER_R2 33;

// Max and min PWM values
#define MAX_PWM 255;
#define MIN_PWM 80;

// Direction of the motors
bool motorR_running;
bool motorL_running;

// Encoder object for the two motors
ESP32Encoder encoderL;
ESP32Encoder encoderR;

// =========== Public functions ===========

RESULT motors_init()
{
	// ---- Setup motor -----

	motorR_running = false;
	motorL_running = false;

	// Set the pins as output
	pinMode(MOTOR_PIN_R1, OUTPUT);
	pinMode(MOTOR_PIN_R2, OUTPUT);
	pinMode(MOTOR_PIN_L1, OUTPUT);
	pinMode(MOTOR_PIN_L2, OUTPUT);

	// ----- Setup encoder ----
	// Enable the weak pull up resistors
	ESP32Encoder::useInternalWeakPullResistors = puType::up;

	// Attach the encoders to the pins
	// 4 count per rotation
	encoderL.attachHalfQuad(ENCODER_L1, ENCODER_L2);
	encoderR.attachHalfQuad(ENCODER_R1, ENCODER_R2);

	// Set the initial count to 0
	encoderL.clearCount();
	encoderR.clearCount();

	return NO_ERROR;
}

RESULT stop_motor()
{
	motorR_running = false;
	motorL_running = false;
	run_left_motor(0);
	run_right_motor(0);
	return NO_ERROR;
}

MOTOR_STEPS get_steps_count()
{
	// Counters for steps of the two motors
	MOTOR_STEPS motor_steps;

	motor_steps.left = encoderL.getCount();
	motor_steps.right = encoderR.getCount();

	return motor_steps;
}

RESULT reset_counter()
{
	encoderL.clearCount();
	encoderR.clearCount();
	return NO_ERROR;
}

RESULT run_left_motor(float speed)
{
	if (speed == 0)
	{
		motorL_running = false;
	}
	//define the direction of the motor
	bool forward = (speed >= 0);
	speed = fabs(speed);

	mororL_running = true;
	int PWM = speed_to_pwm(speed);
	run_motor(PWM, false, forward);
	return NO_ERROR;
}

RESULT run_right_motor(float speed)
{
	if (speed == 0)
	{
		motorR_running = false;
	}
	//define the direction of the motor
	bool forward = (speed >= 0);
	speed = fabs(speed);

	mororR_running = true;
	int PWM = speed_to_pwm(speed);
	run_motor(PWM, true, forward);
	return NO_ERROR;
}

// =========== Private functions ===========
/**
 * @brief Run the given motor at a certain speed
 * @param int PWM The speed of the motor
 * @param bool right_motor True if the motor is the right one
 * @return None
 */
void run_motor(int PWM, bool right_motor, bool forward)
{
	int pin1;
	int pin2;

	// Set the pins
	if (right_motor)
	{
		pin1 = MOTOR_PIN_R1;
		pin2 = MOTOR_PIN_R2;
	}
	else
	{
		pin1 = MOTOR_PIN_L1;
		pin2 = MOTOR_PIN_L2;
	}

	// Apply the PWM with the right direction
	if (forward)
	{
		analogWrite(pin1, PWM);
		analogWrite(pin2, LOW);
	}
	else
	{
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
int speed_to_pwm(float speed)
{
	if (fabs(speed) < 0.001)
	{
		return 0;
	}

	// Calculate the PWM
	int PWM = (int)(MIN_PWM + fabs(speed) * (MAX_PWM - MIN_PWM));
	return PWM;
}
