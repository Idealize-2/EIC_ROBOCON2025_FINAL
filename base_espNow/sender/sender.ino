/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
//EC:E3:34:DA:C2:FC
uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0xDA, 0xC2, 0xFC};

// Structure example to send data
// Must match the receiver structure

char msg = 'R';


esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(32,INPUT_PULLUP);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

bool state;
 
void loop() {
  
  // Send message via ESP-NOW
  if( digitalRead(32) == 1 && !state){
    for(int i = 0 ; i < 10 ; i++)
    {
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &msg, sizeof(msg));
    
      if (result == ESP_OK) {
        Serial.println("Sent with success");
      }
      else {
        Serial.println("Error sending the data");
      }
      delay(100);
    }
    
  }
  state = digitalRead(32) == 1;

    // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &msh, sizeof(msg));
    
    // if (result == ESP_OK) {
    //   Serial.println("Sent with success");
    // }
    // else {
    //   Serial.println("Error sending the data");
    // }
    // delay(100);
}