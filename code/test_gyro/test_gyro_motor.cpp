
#include "DFRobot_BMI160.h"
#include "gyroscope.h"
#include "motion.h"


#define CORRECTION_ANGLE 1.0/16.0



const int8_t i2c_addr = 0x68;
uint32_t value = 0;
double offset = 0.0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/


#define NB_MEASURES 4
#define NS_TO_S 1000000
double mouse_absolute_angle = 0;
double target_angle = 0;



void setup() {
  Serial.begin(115200);
  delay(100);

  motors_init();
  stop();
  setup_gyro();
}

void loop(){

  // Get both accel and gyro data from BMI160
  // parameter accelGyro is the pointer to store the data
  update_gyro(0.00150);

  double current_angle = get_angle();
  Serial.println(current_angle, 5);
  if (current_angle > CORRECTION_ANGLE) {
    turn_right(INPLACE);
  } else if (current_angle < -CORRECTION_ANGLE ) {
    turn_left(INPLACE);
  } else {
    break_wheels();
  }
}