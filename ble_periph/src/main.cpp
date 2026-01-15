#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

const int BOARD_ID = 2;
#define LED_PIN 8

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
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

volatile bool deliverySuccess = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  if (status == ESP_NOW_SEND_SUCCESS)
  {
    deliverySuccess = true;
    digitalWrite(LED_PIN, 0);
  } else
  {
    deliverySuccess = false;
    digitalWrite(LED_PIN, 1);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

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
  myData.id = BOARD_ID;
  myData.x = map(analogRead(XPOS_PIN), 0, 4095, 0, 32767);
  myData.y = map(analogRead(YPOS_PIN), 0, 4095, 0, 32767);
  //myData.y = map(analogRead(YPOS_PIN), 0, 4095, 32767, -32767); for right hand micro-controller
  myData.push = digitalRead(PUSH_PIN);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}