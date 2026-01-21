#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

const uint8_t XPOS_PIN = 3;
const uint8_t YPOS_PIN = 0;
const uint8_t PUSH_PIN = 1;

typedef struct struct_message
{
  int id;
  int x;
  int y;
  int push;
} struct_message;

struct_message myData;
uint8_t broadcastAddress[] = {0x14, 0x33, 0x5C, 0x0F, 0xB8, 0x16}; // if not work, change back to {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

volatile bool deliverySuccess = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  if (status == ESP_NOW_SEND_SUCCESS)
  {
    deliverySuccess = true;
    digitalWrite(8, 0); //LED_PIN
  } else
  {
    deliverySuccess = false;
    digitalWrite(8, 1); //LED_PIN
  }
}

void setup() {
  pinMode(8, OUTPUT); //LED_PIN

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) return;

  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) return;

  analogReadResolution(12);

  pinMode(XPOS_PIN, INPUT);
  pinMode(YPOS_PIN, INPUT);
  pinMode(PUSH_PIN, INPUT_PULLUP);
}

void loop() {
  // right hand
  /*myData.id = 1;
  myData.x = map(analogRead(XPOS_PIN), 0, 4095, 0, 32767);
  myData.y = map(analogRead(YPOS_PIN), 0, 4095, 0, 32767);
  */
 
  // left hand
  myData.id = 2;
  myData.x = map(analogRead(XPOS_PIN), 0, 4095, 32767, 0);
  myData.y = map(analogRead(YPOS_PIN), 0, 4095, 32767, 0);

  myData.push = digitalRead(PUSH_PIN);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(5);
}