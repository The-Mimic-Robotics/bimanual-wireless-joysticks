#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define MAX_DEVICES 3
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
const String DEVICE_NAME = "ESP32-CENTRAL";

std::vector<BLEAdvertisedDevice*> devicesFound; 
std::vector<String> connectedAddresses; 

bool isScanning = false;

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)
{
  String value = "";
  for (int i = 0; i < length; i++)
  {
    value += (char)pData[i];
  }

  String senderAddress = pBLERemoteCharacteristic->getRemoteService()->getClient()->getPeerAddress().toString().c_str();

  SerialBT.print(senderAddress);
  SerialBT.print(": ");
  SerialBT.println(value);
}

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient* pclient)
  {
    SerialBT.println("Connected to a new client!");
  }

  void onDisconnect(BLEClient* pclient)
  {
    SerialBT.println("Disconnected from a client.");
  }
};

bool connectToServer(BLEAdvertisedDevice* myDevice)
{
  SerialBT.print("Forming a connection to ");
  SerialBT.println(myDevice->getAddress().toString().c_str());

  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  if (!pClient->connect(myDevice))
  {
    SerialBT.println(" - Failed to connect to server");
    return false;
  }
  SerialBT.println(" - Connected to server");

  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    SerialBT.print(" - Failed to find our service UUID: ");
    SerialBT.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }

  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    SerialBT.print(" - Failed to find our characteristic UUID: ");
    SerialBT.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }

  if(pRemoteCharacteristic->canRead())
  {
    String value = pRemoteCharacteristic->readValue();
    SerialBT.print(" - Initial Value: ");
    SerialBT.println(value.c_str());
  }

  if(pRemoteCharacteristic->canNotify())
  {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }

  return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
    {

      String tempAddr = advertisedDevice.getAddress().toString().c_str();

      bool known = false;
      for(int i=0; i < connectedAddresses.size(); i++) {
        if (connectedAddresses[i] == tempAddr)
        {
          known = true;
          break;
        }
      }

      if (!known && connectedAddresses.size() < MAX_DEVICES)
      {
        SerialBT.print("New Device Found: ");
        SerialBT.println(tempAddr);

        connectedAddresses.push_back(tempAddr);

        devicesFound.push_back(new BLEAdvertisedDevice(advertisedDevice));
      }
    }
  }
};

void setup() {
  SerialBT.begin(DEVICE_NAME);

  BLEDevice::init(DEVICE_NAME);

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
}

void loop() {
  while (!devicesFound.empty())
  {
    BLEAdvertisedDevice* device = devicesFound.back();
    devicesFound.pop_back();

    if (connectToServer(device))
    {
      SerialBT.println("Success! Listening for data...");
    } else
    {
      SerialBT.println("Failed to connect.");
    }

    delete device;
  }

  if (connectedAddresses.size() < MAX_DEVICES)
  {
    if (!isScanning)
    {
      BLEDevice::getScan()->start(5, false);
      isScanning = true;
    } else
    {
      isScanning = false;
    }
  }
}
