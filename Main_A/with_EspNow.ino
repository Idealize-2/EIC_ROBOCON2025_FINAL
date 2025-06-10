#ifdef with_EspNow

void initEspNow()
{
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // เมื่อรับข้อมูลมา ให้ทำในฟังก์ชั่น OnDataRecv ที่เราสร้างไว้
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

char msg;



void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&msg, incomingData, sizeof(msg));
  if( !Letgooooooo && !YState)
  {
    if( msg == 'R' )
    {
      Serial.println("LEt gooooooooooooooooooooooooooooooo!!!!!!!!!!!!!!!!!!!");
      // YState = !YState;
      // YStartTime = millis();
      // Letgooooooo = true;
      // start_state = millis();
    }
  }
  
  
}

#endif