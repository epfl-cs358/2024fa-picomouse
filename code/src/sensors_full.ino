#include "sensors.h"
#include <Wire.h>

void setup(){
  Serial.begin(115200);
  delay(100);
  Wire.begin();
  delay(100);
  Serial.println("ff irl ");
  RESULT res = init_TOF();
  Serial.println(res);


}
uint16_t distances[4];

void loop(){
  RESULT res = update_all();
  Serial.println(res);
  ///CHECK_AND_THROW(!sensor.begin(), TOF_INIT_FAIL);
  get_side_side_distances(distances);
  Serial.printf("left: %hu mid_left: %hu  mid_right: %hu  right: %hu \n", distances[0],  distances[1], distances[2],  distances[3]);
  delay(500);
}