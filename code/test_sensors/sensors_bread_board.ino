#include <DFRobot_VL6180X.h>

//Modify I2C address, invalid after power off
//DFRobot_VL6180X VL6180X(/* iicAddr */0x29,/* TwoWire * */&Wire);
DFRobot_VL6180X s1;
DFRobot_VL6180X s2;
DFRobot_VL6180X s3;
DFRobot_VL6180X s4;

#define pin1 5
#define pin2 17 
#define pin3 16
#define pin4 4

uint8_t new_add1 = 0x40;
uint8_t new_add2 = 0x42;
uint8_t new_add3 = 0x44;
uint8_t new_add4 = 0x46;

void setup() {
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);

  digitalWrite(pin1,HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3,LOW);
  digitalWrite(pin4,LOW);
  delay(100);

  Serial.begin(9600);
  while(!(s1.begin())){
    Serial.println("Please check that the IIC device is properly connected! 1");
    delay(1000);
  }
  s1.setIICAddr(new_add1);
  delay(100);

  digitalWrite(pin2, HIGH);
  delay(100);
  while(!(s2.begin())){
    Serial.println("Please check that the IIC device is properly connected! 2");
    delay(1000);
  }
  s2.setIICAddr(new_add2);
  delay(100);
  //
  digitalWrite(pin3, HIGH);
  delay(100);
  while(!(s3.begin())){
    Serial.println("Please check that the IIC device is properly connected! 3");
    delay(1000);
  }
  s3.setIICAddr(new_add3);
  delay(100);
///
  digitalWrite(pin4, HIGH);
  delay(100);
  while(!(s4.begin())){
    Serial.println("Please check that the IIC device is properly connected! 4");
    delay(1000);
  }
  s4.setIICAddr(new_add4);
  delay(100);

  Serial.println("I2C Scanner");
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at address: 0x");
      if (i < 16) {
        Serial.print("0");
      }
      Serial.println(i, HEX);
    }
  }
}

#define MACRO_QUI_TUE(sensor)\
    do{\
      range = sensor.rangePollMeasurement();\
      status = sensor.getRangeResult();\
      str1 = "Range: "+String(range) + " mm";\
      Serial.println(str1); \
      delay(100);\
    }while(0)

void loop() {
  delay(3000);
  uint8_t range;
  uint8_t status;
  String str1;

  Serial.println("temp : 1");
  MACRO_QUI_TUE(s1);
  Serial.println("temp : 2");
  MACRO_QUI_TUE(s2);
  Serial.println("temp : 3");
  MACRO_QUI_TUE(s3);
  Serial.println("temp : 4");
  MACRO_QUI_TUE(s4);
}