#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

bool doConnect = false;
bool connected = false;
bool doScan = false;
BLERemoteCharacteristic* pRemoteCharacteristic;
BLEAdvertisedDevice* myDevice;

String latestSlaveData = "Waiting...";

const uint8_t XPOS_PIN = 33;
const uint8_t YPOS_PIN = 25;
const uint8_t PUSH_PIN = 34;

// This runs automatically whenever the peripheral sends data
// Replaces the previous structure where data was read in the loop
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    String value = "";
    for (int i = 0; i < length; i++) {
        value += (char)pData[i];
    }
    latestSlaveData = value;
}

// For peripheral disconnects
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("Disconnected from Slave!");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient* pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remote BLE Server.
    if (!pClient->connect(myDevice)) return false;
    Serial.println(" - Connected to server");

    // Obtain a reference to the service
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) return false;

    // Obtain a reference to the characteristic
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) return false;

    // Read the initial value
    if(pRemoteCharacteristic->canRead()) {
      String value = pRemoteCharacteristic->readValue();
    }

    // REGISTER FOR NOTIFICATIONS
    if(pRemoteCharacteristic->canNotify()) {
      pRemoteCharacteristic->registerForNotify(notifyCallback);
    }

    connected = true;
    return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Client...");
  BLEDevice::init("ESP32-MASTER");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

  pinMode(XPOS_PIN, INPUT);
  pinMode(YPOS_PIN, INPUT);
  pinMode(PUSH_PIN, INPUT);
}

void printJoystickMasterData()
{
  //Serial.print(map(analogRead(XPOS_PIN), 0, 4095, -100, 100));  This is very inaccurate
  //Serial.print((analogRead(XPOS_PIN) - 2048) / 21);             This is just inaccurate
  Serial.print(analogRead(XPOS_PIN));

  Serial.print(" | ");
  //Serial.print(map(analogRead(YPOS_PIN), 0, 4095, -100, 100));  This is very inaccurate
  //Serial.print((analogRead(YPOS_PIN) - 2048) / 21);             This is just inaccurate
  Serial.print(analogRead(YPOS_PIN));

  Serial.print(" | ");
  Serial.print(digitalRead(PUSH_PIN));
}

void loop() {
  // If doConnect is true, we attempt a connection ONCE
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false; // Prevent connecting again in the next loop
  }

  // If we are connected, we print the latest data
  if (connected) {
    Serial.print("Master_Data: ");
    printJoystickMasterData();
    Serial.print(" | ");
    Serial.print("Slave_Data: ");
    Serial.println(latestSlaveData); // This variable is updated by the background callback
    
    delay(500); // Print every 500ms
  }
}
