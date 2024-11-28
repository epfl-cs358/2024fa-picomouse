#include "motion.h"
#include <math.h>

float speed_goal = 0; // [-1;1]

float left_speed = 0;  // [-1;1]
float right_speed = 0; // [-1;1]

// radian per second
float left_rps = 0;
float right_rps = 0;

unsigned long last_time = 0;
unsigned long current_time = 0;

MOTOR_STEPS old_steps_count = {0, 0};
MOTOR_STEPS new_steps_count = {0, 0};

RESULT stop()
{
    stop_motor();
    return NO_ERROR;
}

RESULT forward(float speed)
{
    speed = cap_speed(speed);
    // If the speed is different from the current speed, set the new speed
    if (speed_goal != speed)
    {
        set_new_speed_forward(speed);
        last_time = millis();
        current_time = millis();
    }
    // If the speed is the same, check if the time has passed
    else
    {
        current_time = millis();
        unsigned long delta_time = current_time - last_time;

        // If the time has passed, compute the new speed
        if (delta_time >= DELTA_TIME)
        {
            last_time = current_time;
            new_steps_count = get_steps_count();

            left_rps = (new_steps_count.left_count - old_steps_count.left_count) / delta_time;
            right_rps = (new_steps_count.right_count - old_steps_count.right_count) / delta_time;

            // Syncronize the two speeds
            update_speed();

            old_steps_count = new_steps_count;
            return NO_ERROR;
        }
    }
}

RESULT turn_right(MODE mode)
{
    if (last_time == 0)
    {
        motors_init();
    }
    last_time = millis();
    //TODO peut etre inverser
    run_right_motor(- ROTATION_SPEED);
    run_left_motor(ROTATION_SPEED);
}

RESULT turn_left(MODE mode)
{
    if (last_time == 0)
    {
        motors_init();
    }
    last_time = millis();
}

// ========== Private functions ===========
void update_speed()
{
    float error = left_rps - right_rps;
    float correction = KP * error;

    if (error > 0) // Left is faster
    {
        if (fabs(right_speed) < 1)
        { // We can increase right speed
            right_speed = right_speed > 0 ? cap_speed(right_speed + correction) : cap_speed(right_speed - correction);
            run_right_motor(right_speed);
        }
        else
        { // We decrease left speed
            left_speed = left_speed > 0 ? cap_speed(left_speed - correction) : cap_speed(left_speed + correction);
            run_left_motor(left_speed);
        }
    }
    else if (error < 0) // Right is faster
    {
        if (fabs(left_speed) < 1)
        { // We can increase left speed
            left_speed = left_speed > 0 ? cap_speed(left_speed + correction) : cap_speed(left_speed - correction);
            run_left_motor(left_speed);
        }
        else
        { // We decrease right speed
            right_speed = right_speed > 0 ? cap_speed(right_speed - correction) : cap_speed(right_speed + correction);
            run_right_motor(right_speed);
        }
    }
}
/**
 * @brief set the new speed of the two motors --> used to go forward
 */
void set_new_speed_forward(float speed)
{
    speed_goal = speed;
    left_speed = speed;
    right_speed = speed;

    // If the motors are not initialized, do it
    if (last_time == 0)
    {
        motors_init();
    }
    last_time = millis();

    reset_counter();
    old_steps_count = {0, 0};
    new_steps_count = {0, 0};

    // Set the new speed
    run_left_motor(speed);
    run_right_motor(speed);
}

float cap_speed(float speed)
{
    speed = speed > 1 ? 1 : speed;
    speed = speed < -1 ? -1 : speed;
    return speed;
}