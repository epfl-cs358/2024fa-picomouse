#include "DFRobot_BMI160.h"
#include "motion.h"
#include "navigation.h"
#include "gyroscope.h"
#include "utils.h"


void setup() {
  Serial.begin(115200);
  delay(100);
  // put your setup code here, to run once:
  motors_init();
  stop();
  setup_gyro();
}

void loop() {
  Serial.println("Enter loop");
  
  RESULT rlt = turn(0.5, INPLACE);
  rlt = turn(-0.5, INPLACE);
  while(1);

}
