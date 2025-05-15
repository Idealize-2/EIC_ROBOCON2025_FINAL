//-------------------------------------------------- Library Include
#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Mapf.h>
#include <string.h>
#include "Motor.h"



//-------------------------------------------------- Library Include




//---------------------------------------- Smile Drive communication
#define MOTOR_SDA 21
#define MOTOR_SCL 22
//---------------------------------------- Smile Drive communication


//---------------------------------------- DriveBase Address && Define
#define Left_I2C_ADDRESS 0x50
#define Right_I2C_ADDRESS 0x57
#define Front_I2C_ADDRESS 0x85
#define Back_I2C_ADDRESS 0x86

MotorI2C motor1( Left_I2C_ADDRESS , Front_I2C_ADDRESS ); // Front Left
MotorI2C motor2( Right_I2C_ADDRESS , Front_I2C_ADDRESS ); // Front Right
MotorI2C motor3( Left_I2C_ADDRESS , Back_I2C_ADDRESS ); // Back Left
MotorI2C motor4( Right_I2C_ADDRESS , Back_I2C_ADDRESS ); // Back Right

//---------------------------------------- DriveBase Address && Define


//----------------- CONTROLLER ANALOG INIT && CONTROLLER OPTIONS

// UNCOMMENT JUST ONE USED PROTOCAL (uncomment in pair) 

#define Bluetooth bool useBluetooth = true; // use esp32_bluepad in board manager

//#define EspNow bool useEspNow = false;

//#define Wireless bool useWireless = false;

//#define WebSocket bool useWebSocket = false;

#ifdef Bluetooth 
  #include <Bluepad32.h>
  ControllerPtr myControllers[ 1 ];
#endif

#ifdef EspNow
  #include <esp_now.h>
  #include <WiFi.h>
  // Dont forget to change Reciever MAC address in Sender Code
#endif

float x_ctrl = 0;
float y_ctrl = 0;
float x_turn = 0;
float y_turn = 0;
//----------------- CONTROLLER ANALOG INIT && CONTROLLER OPTIONS


struct Controller_Status {

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

};

// Instance Controller Logic
Controller_Status C_now;
Controller_Status C_past;

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
  Serial.begin(115200);
  initiateMotors();
  resetEncoders();

  //Start Communication Protocal
  initProtocal();

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
//------------------------------------------------ Controller Input && update data
  bool dataUpdated = updateData( C_now );
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
  float turn = mapf(x_turn, -1, 1, -180, 180);
  float speed = sqrt((pow(x_ctrl, 2) + pow(y_ctrl, 2))) * 255;
  movebase(speed, direction, turn);
//------------------------------------------------- DriveBase Control

}
