//-------------------------------------------------- Library Include
#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Mapf.h>
#include <string.h>
#include "Motor.h"


//---------------------------------------- Smile Drive communication
#define MOTOR_SDA 21
#define MOTOR_SCL 22

//---------------------------------------- Upper Shooter pin
#define INA1 26
#define INB1 25
#define PWM1 27

//---------------------------------------- Lower Shooter pin
#define INA2 16
#define INB2 32
#define PWM2 4

//---------------------------------------- DriveBase Address && Define
float LIMIT_SPEED = 0.55;

float smoothedSpeed1 = 0;
float smoothedSpeed2 = 0;
float smoothedSpeed3 = 0;
float smoothedSpeed4 = 0;
unsigned long lastUpdateTime = 0;
const float smoothFactor = 0.2;

#define Left_I2C_ADDRESS 0x50
#define Right_I2C_ADDRESS 0x57
#define Front_I2C_ADDRESS 0x85
#define Back_I2C_ADDRESS 0x86


//---------------------------------------- Feeder, Drawer Address && Define
#define IIOrpm_I2C_ADDRESS 0x51
#define FeederPin 0x85
#define DrawerPin 0x86

#define Relay 33

MotorI2C motor1( Left_I2C_ADDRESS , Front_I2C_ADDRESS ); // Front Left
MotorI2C motor2( Right_I2C_ADDRESS , Front_I2C_ADDRESS ); // Front Right
MotorI2C motor3( Left_I2C_ADDRESS , Back_I2C_ADDRESS ); // Back Left
MotorI2C motor4( Right_I2C_ADDRESS , Back_I2C_ADDRESS ); // Back Right
MotorI2C motor5( IIOrpm_I2C_ADDRESS , FeederPin ); // Feeder
MotorI2C motor6( IIOrpm_I2C_ADDRESS , DrawerPin ); // Drawer



//---------------------------------------- DriveBase Address && Define


//----------------- CONTROLLER ANALOG INIT && CONTROLLER OPTIONS


//----------------- Controller State Button
bool XState = false;
bool YState = false;
int ShootState = 0;
 
bool AState = false;

bool BState = false;
bool AfterBState = false;

unsigned long  AStartTime = 0;
unsigned long  AfterBStartTime = 0;
unsigned long  BStartTime = 0;
unsigned long  YStartTime = 0;


bool L1State = false;
bool L2State = false;
bool R1State = false;
bool R2State = false;

bool Letgooooooo = false;
bool LetgoState = false;

long start_state;


// UNCOMMENT JUST ONE USED PROTOCAL (uncomment in pair) 

#define Bluetooth bool useBluetooth = true; // use esp32_bluepad in board manager

#define with_EspNow

//#define EspNow bool useEspNow = true; // also check Reciever MAC address to send

//#define WebSocket bool useWebSocket = false;

#ifdef Bluetooth 
  #include <Bluepad32.h>
#include <uni.h>
#include "esp_bt.h"
ControllerPtr myControllers[1];

// The address of the gamepad that is allowed to connect.
// You can add up to four entries.
static const char * controller_addr_string = "5C:BA:37:F8:62:00"; //XBOX
//static const char * controller_addr_string = "90:B6:85:9A:50:B0"; //PS5

#endif

#ifdef with_EspNow
  #include <esp_now.h>
  #include <WiFi.h>

#endif

#ifdef EspNow
  #include <esp_now.h>
  #include <WiFi.h>
  // Dont forget to change Reciever MAC address in Sender Code
#endif

#ifdef Wireless
  #include <SPI.h>
  #include <nRF24L01.h>
  #include <RF24.h>

  const uint64_t pipeIn = 0xE9E8F0F0E1LL;
  RF24 radio(2, 4); // ( CE , CSN ) Pin

#endif

float x_ctrl = 0;
float y_ctrl = 0;
float x_turn = 0;
float y_turn = 0;
//----------------- CONTROLLER ANALOG INIT && CONTROLLER OPTIONS


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

// Instance Controller Logic
Controller_Status C_now;
Controller_Status C_past;

// for Radio
Controller_Status data;

///////////////////////////////////////
//    _____        _                 
//   / ____|      | |                
//  | (___    ___ | |_  _   _  _ __  
//   \___ \  / _ \| __|| | | || '_ \ 
//   ____) ||  __/| |_ | |_| || |_) |
//  |_____/  \___| \__| \__,_|| .__/ 
//                            | |    
//                            |_|    
///////////////////////////////////////

void setup() {
  // Somewhere in your "setup" add the following lines:
  

  Serial.begin(115200);
  Wire.begin();
  initiateMotors();
  resetEncoders();

  pinMode(INA1, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  
  pinMode(INA2, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  pinMode(Relay, OUTPUT);

  //Start Communication Protocal
  initProtocal();

  initEspNow();
  

}

/////////////////////////////////////////////////////////////
//   __  __         _          _                          
//  |  \/  |       (_)        | |                         
//  | \  / |  __ _  _  _ __   | |      ___    ___   _ __  
//  | |\/| | / _` || || '_ \  | |     / _ \  / _ \ | '_ \ 
//  | |  | || (_| || || | | | | |____| (_) || (_) || |_) |
//  |_|  |_| \__,_||_||_| |_| |______|\___/  \___/ | .__/ 
//                                                 | |    
//                                                 |_|  
/////////////////////////////////////////////////////////////

void loop() 
{

    if( Letgooooooo )
    {
      if(millis() > start_state + 500 ) Letgooooooo = false;

    }

      //------------------------------------------------ Controller Input && update data
    bool dataUpdated = updateData( C_now );
    //Serial.println("OK"); 
    //Serial.println( dataUpdated ); //for Debug
    if (dataUpdated) 
    {
      //Serial.print("data updated");
      processController();

      //Update C_past for Rising Edge Falling Edge Case
      C_past = C_now;
    }
    


  //------------------------------------------------ Controller Input


  //------------------------------------------------- DriveBase Control
    float direction = atan2(x_ctrl, -y_ctrl);
    float turn = mapf(x_turn, -1, 1, -140, 140);
    float speed = sqrt((pow(x_ctrl, 2) + pow(y_ctrl, 2))) * 255;
    

    movebase(speed, direction, turn);
  //------------------------------------------------- DriveBase Control
    ActionCommand();

}
