// 4 Channel Transmitter | 4 Kanal Verici

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE9E8F0F0E1LL;   //IMPORTANT: The same as in the receiver 0xE9E8F0F0E1LL | Bu adres alıcı ile aynı olmalı
RF24 radio(2, 4); // select CE,CSN pin | CE ve CSN pinlerin seçimi

typedef struct Controller_Status {

    // RIGHT BUTTON Ex.(Right Down = rd, Right Left = rl)
    bool rt , rl , rr , rd;

    // LEFT BUTTON Ex.(Left Top = lt,Left Left = ll)
    bool lt , ll , lr , ld;

    // LEFT ANALOG
    float x , y ;
    bool l3 ;

    // RIGHT ANALOG
    float rx , ry ;
    bool r3 ;

    // R2 R1 L2 L1
    bool r2 , r1 ;
    bool l2 , l1 ;

    // SELECT HOME
    bool select , home ;

} Controller_Status ;

Controller_Status data;

void ResetData() 
{
 data.x = 0;
 data.y = 0;
}

void setup()
{
//Start everything up
    Serial.begin( 112500 );
    radio.begin();
    radio.openWritingPipe(pipeOut);
    radio.setChannel(5);
    radio.stopListening(); //start the radio comunication for Transmitter | Verici olarak sinyal iletişimi başlatılıyor
    ResetData();
}

// Joystick center and its borders | Joystick merkez ve sınırları


void loop()
{
    // Control Stick Calibration | Kumanda Kol Kalibrasyonları
    // Setting may be required for the correct values of the control levers. | Kolların doğru değerleri için ayar gerekebilir.
    data.x +=1;
    data.y +=2;

    if( radio.write(&data, sizeof(Controller_Status)) )
    {
        Serial.println("nigga");
    }
    else
    {
        Serial.println("I ken");
    }
    //Serial.println("nigger");
    delay(100);

    
}