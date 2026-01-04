#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic* pCharacteristic;
bool deviceConnected = false;

const uint8_t XPOS_PIN = 3;
const uint8_t YPOS_PIN = 2;
const uint8_t PUSH_PIN = 1;

#define NAME "ESP32-SLAVE"

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer)
  {
    deviceConnected = false;
    BLEDevice::startAdvertising();
  }
};

void setup()
{
  BLEDevice::init(NAME);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); 
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  pinMode(XPOS_PIN, INPUT);
  pinMode(YPOS_PIN, INPUT);
  pinMode(PUSH_PIN, INPUT);
}

void loop() {
  if (deviceConnected)
  {
    int xPos = analogRead(XPOS_PIN);
    int yPos = analogRead(YPOS_PIN);
    int push = digitalRead(PUSH_PIN);

    String data = String(xPos) + " | " + String(yPos) + " | " + String(push);

    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();
  }
}
