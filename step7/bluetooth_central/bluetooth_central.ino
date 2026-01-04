#include <esp_now.h>
#include <WiFi.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

typedef struct struct_message {
  int id;
  int x;
  int y;
  int push;
} struct_message;

struct_message incomingData;

int msgI1 = 0;
int msgX1 = 0;
int msgY1 = 0;
int msgB1 = 0;
int msgI2 = 0;
int msgX2 = 0;
int msgY2 = 0;
int msgB2 = 0;

void OnDataRecv(const esp_now_recv_info_t * info, const uint8_t *incomingDataPtr, int len)
{
  const uint8_t* mac = info->src_addr;
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));

  if (incomingData.id == 1)
  {
    msgX1 = incomingData.x;
    msgY1 = incomingData.y;
    msgB1 = incomingData.push;
  } else
  if (incomingData.id == 2)
  {
    msgX2 = incomingData.x;
    msgY2 = incomingData.y;
    msgB2 = incomingData.push;
  }
}

void setup() {
  SerialBT.begin("ESP32-CENTRAL");

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    SerialBT.println("Error initializing ESP-NOW");
    return;
  }
}
 void loop()
{
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  SerialBT.print(msgX1);
  SerialBT.print(",");
  SerialBT.print(msgY1);
  SerialBT.print(",");
  SerialBT.print(msgB1);
  SerialBT.print(",");
  SerialBT.print(msgX2);
  SerialBT.print(",");
  SerialBT.print(msgY2);
  SerialBT.print(",");
  SerialBT.println(msgB2);
}
