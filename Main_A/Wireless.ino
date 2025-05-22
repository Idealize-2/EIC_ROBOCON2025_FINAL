#ifdef Wireless

// for DEbug
// void initProtocal(){}
// void updateData(){}

void ResetData()
{
// Define the inicial value of each data input. | Veri girişlerinin başlangıç değerleri
// The middle position for Potenciometers. (254/2=127) | Potansiyometreler için orta konum
  data.x = 0;
  data.y = 0;
}

void initProtocal()
{ 
   ResetData();
   //Configure the NRF24 module
   radio.begin();
   radio.setChannel(5);
   radio.openReadingPipe(1,pipeIn);
  
   radio.startListening(); //start the radio comunication for receiver | Alıcı olarak sinyal iletişimi başlatılıyor
}

bool updateData( Controller_Status& C_state ){
//   Serial.print("OK"); 
  if ( radio.available() ) {
    Serial.println("Let Gooooooooooooooooooooooooooooooooooooooooooooooooooooo"); 
    radio.read(&data, sizeof(Controller_Status));
    Serial.print( data.x );
    Serial.print( " " );
    Serial.println( data.y );
    C_now = data;
    return true;
  }
//   delay( 50 );
  return false;
}

#endif