#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include <NimBLEDevice.h>
#include <BleGamepad.h>
#include <BleGamepadConfiguration.h>

const int LED_PIN = 2;

BleGamepad bleGamepad("ESP32 Dual Stick", "Maker", 100);

typedef struct struct_message
{
  int id;
  int x;
  int y;
  int push;
} struct_message;

struct_message incomingData;

volatile long msgX1 = 0, msgY1 = 0, msgB1 = 0;
volatile long msgX2 = 0, msgY2 = 0, msgB2 = 0;

unsigned long lastRecvTime1 = 0;
unsigned long lastRecvTime2 = 0;
const unsigned long TIMEOUT_MS = 2000;

TaskHandle_t Task0;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataPtr, int len)
{
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));

  if (incomingData.id == 1)
  {
    msgX1 = incomingData.x;
    msgY1 = incomingData.y;
    msgB1 = incomingData.push;
    lastRecvTime1 = millis();
  } else
  if (incomingData.id == 2)
  {
    msgX2 = incomingData.x;
    msgY2 = incomingData.y;
    msgB2 = incomingData.push;
    lastRecvTime2 = millis();
  }
}

void espNowTask(void * pvParameters)
{
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    vTaskDelete(NULL);
  }

  esp_now_register_recv_cb(OnDataRecv);

  while (1)
  {
    unsigned long currentTime = millis();
    bool connected1 = (currentTime - lastRecvTime1) < TIMEOUT_MS;
    bool connected2 = (currentTime - lastRecvTime2) < TIMEOUT_MS;

    if (connected1 && connected2)
    {
      digitalWrite(LED_PIN, 1);
    } else
    {
      digitalWrite(LED_PIN, 0);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);

  xTaskCreatePinnedToCore(
    espNowTask,   // task function
    "ESPNowTask", // name of task
    10000,        // stack size of task
    NULL,         // parameter of the task
    1,            // priority of the task
    &Task0,       // task handle
    0             // execute on core 0
  );

  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setAutoReport(false);
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepad.begin(&bleGamepadConfig);
}

void loop() {
  //xPortGetCoreID() will return 1 as the loop() always runs on Core 1
  if (bleGamepad.isConnected())
  {
    bleGamepad.setAxes(msgX1, msgY1, msgX2, msgY2, 0, 0, 0, 0);

    if (msgB1 == 0) bleGamepad.press(BUTTON_1);
    else bleGamepad.release(BUTTON_1);

    if (msgB2 == 0) bleGamepad.press(BUTTON_2);
    else bleGamepad.release(BUTTON_2);

    bleGamepad.sendReport();
  }

  //Serial.print(map(msgX1, 0, 4095, -32767, 32767));
  Serial.print(msgX1);
  Serial.print(",");
  //Serial.print(map(msgY1, 0, 4095, -32767, 32767));
  Serial.print(msgY1);
  Serial.print(",");
  //Serial.print(map(msgX2, 0, 4095, -32767, 32767));
  Serial.print(msgX2);
  Serial.print(",");
  //Serial.println(map(msgY2, 0, 4095, -32767, 32767));
  Serial.println(msgY2);

  delay(20);
}