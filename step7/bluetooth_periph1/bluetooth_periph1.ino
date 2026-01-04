#include <esp_now.h>
#include <WiFi.h>

#define BOARD_ID 1 

const uint8_t XPOS_PIN = 3;
const uint8_t YPOS_PIN = 2;
const uint8_t PUSH_PIN = 1;

typedef struct struct_message {
  int id; 
  int x;
  int y;
  int push;
} struct_message;

struct_message myData;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  pinMode(XPOS_PIN, INPUT);
  pinMode(YPOS_PIN, INPUT);
  pinMode(PUSH_PIN, INPUT);
}

void loop() {
  // 1. Read Data
  myData.id = BOARD_ID;
  myData.x = analogRead(XPOS_PIN);
  myData.y = analogRead(YPOS_PIN);
  myData.push = digitalRead(PUSH_PIN);

  // 2. Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}
