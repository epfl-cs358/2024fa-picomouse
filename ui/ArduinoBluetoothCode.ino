
#include <SoftwareSerial.h>
//uint8_t address[6]  = {0xA0, 0xA3, 0xB3, 0x28, 0xED, 0x76}; MAC address of the ESP32, from attempting to connect to it
SoftwareSerial BTSerial(2, 3); 
void setup() {

Serial.begin(9600);
Serial.println("Initializing Bluetooth Module in Data Mode\n");

BTSerial.begin(9600);
//SerialBT.connect(address); ESP32 connection attempt
Serial.print("Bluetooth Module is Successfully Initialized in Data Mode\n");
}
void loop() {

if (Serial.available()) {
BTSerial.write(Serial.read());
}

if (BTSerial.available()) {
Serial.write(BTSerial.read());
}
}
