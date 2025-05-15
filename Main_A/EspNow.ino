#ifdef EspNow

void initProtocal(){
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

// typedef struct struct_message {
//     char a[32];
//     int b;
//     float c;
//     bool d;
// } struct_message;

// struct_message C_Data;

Controller_Status C_Data;

// ToDo When Recieve Data
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&C_Data, incomingData, sizeof(C_Data));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(C_Data.x);
  Serial.print("Y: ");
  Serial.println(C_Data.y);
  Serial.print("RR: ");
  Serial.println(C_Data.rr);
  Serial.print("LL: ");
  Serial.println(C_Data.ll);
  Serial.println();
}
bool updateData( Controller_Status& C_state ){

}

#endif