void processController()
{
  if( C_now.rt && !C_past.rt  ){
    //Serial.println("Y button pressed");
    YState = true;

  }
  if( C_now.rl && !C_past.rl ){
    //Serial.println("X button pressed");
    XState != XState;
    
  if( C_now.rr && !C_past.rr ){
    //Serial.println("B button pressed");
    BState = true;
    BStartTime = millis();

  }
  if( C_now.rd && !C_past.rd ){
    //Serial.println("A button pressed");
    AState = true;
    AStartTime = millis();
  }

  // Serial.print(C_now.x);
  // Serial.print(" ");
  // Serial.println(C_now.y);

  // Serial.print(C_now.rx);
  // Serial.print(" ");
  // Serial.println(C_now.ry);
}

void ActionCommand(){
  if (AState == true){
    ActionTime(AStartTime, 3000, 'A');    
  }
  if (BState == true){
    ActionTime(BStartTime, 1000, 'B');
  }
  if (AfterBState == true){
    ActionTime(AfterBStartTime, 1200, 'C');
  }
  if (XState == true){
    digitalWrite(INA1, HIGH);
    digitalWrite(INB1, LOW);
    analogWrite(PWM1, 100);

    digitalWrite(INA2, HIGH);
    digitalWrite(INB2, LOW);
    analogWrite(PWM2, 250);
    Serial.println("Shoot");
  } 
  if (XState == false){
    digitalWrite(INA1, HIGH);
    digitalWrite(INB1, LOW);
    analogWrite(PWM1, 0);

    digitalWrite(INA2, HIGH);
    digitalWrite(INB2, LOW);
    analogWrite(PWM2, 0);
    Serial.println("Stop");
  } 

}

void ActionTime(unsigned long start_time ,unsigned long time, char label){
  if( millis() - start_time >= time  )
      {
        //End process
        //Serial.println("End");
        if (label == 'A'){
          AState = false;
          

        }
        if (label == 'B'){
          motor5.run(0);
          BState = false;
          AfterBState = true;
          AfterBStartTime = millis();
          Serial.println("EndB");
        }
        if (label == 'C'){
          motor5.run(0);
          AfterBState = false;
          Serial.println("EndBDown");
        }
      }
  else{
    if (label == 'A'){
      Serial.println("Drawer_On");
    }   
    if (label == 'B'){
      Serial.println("Feeder_Up");
      motor5.run(255);
    }
    if (label == 'C'){
      Serial.println("Feeder_Down");
      motor5.run(-255);
    }    
  }     

}