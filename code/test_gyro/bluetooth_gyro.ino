#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "DFRobot_BMI160.h"
#include "calibration.h"



DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x68;
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
double offset = 0.0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define NB_MEASURES 4
#define NS_TO_S 1000000
double current_angle = 0;
double target_angle = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


void setup() {
  Serial.begin(9600);
  delay(100);

  //init the hardware bmin160
  if (bmi160.softReset() != BMI160_OK){
    Serial.println("reset false");
    while(1);
  }

  //set and init the bmi160 i2c address
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK){
    Serial.println("init false");
    while(1);
  }
  // Create the BLE Device
  BLEDevice::init("ESP_32_PICOMOUSE");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID, //                      BLECharacteristic::PROPERTY_READ   |BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_INDICATE
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
      RESULT rslt_init_pos = NO_ERROR;
      rslt_init_pos = init_pos(&bmi160, &offset);

      
          // notify changed value
        if (deviceConnected) {
        int i = 0;
        int rslt;
        int16_t accelGyro[6] = {0};
        uint32_t time_stamp[2]= {0};
        uint32_t start_time = 0;
        uint32_t end_time = 0;
        float value = 0;

        // Get both accel and gyro data from BMI160
        // parameter accelGyro is the pointer to store the data
        for (int i = 0; i < NB_MEASURES; i++) {
          rslt = bmi160.getAccelGyroData(accelGyro, time_stamp);
          if (rslt != 0) {
            Serial.println("I fucking died mate");
            while (1);
          }
          if (i == 0){
            start_time = time_stamp[0];
          }
          value += static_cast<float>(accelGyro[2]);
        }
        end_time = time_stamp[0];
        float elapsed_time_ns = (end_time - start_time);
        value /= NB_MEASURES;
        value *= 3.14 / 180.0;
        current_angle += (value * elapsed_time_ns) / NS_TO_S;
        if((current_angle - target_angle) >= offset){
          current_angle -= offset;
        }
        if((current_angle - target_angle) <= -offset){
          current_angle += offset;
        }
    
          char buffer [200] = {0};
          sprintf(buffer, "vz : %f",current_angle);
          pCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));
          pCharacteristic->notify();
          value++;
          delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
        }
        // disconnecting
        if (!deviceConnected && oldDeviceConnected) {
            delay(500); // give the bluetooth stack the chance to get things ready
            pServer->startAdvertising(); // restart advertising
            Serial.println("start advertising");
            oldDeviceConnected = deviceConnected;
        }
        // connecting
        if (deviceConnected && !oldDeviceConnected) {
            // do stuff here on connecting
            oldDeviceConnected = deviceConnected;
        }
      
      
}