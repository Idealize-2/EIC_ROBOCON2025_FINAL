void processController()
{
  if( C_now.rt && !C_past.rt  ){
    //Serial.println("Y button pressed");
    YState = !YState;
    YStartTime = millis();

    // if (ShootState == 0){
    //   ShootState = 2;
    // }
    // else{
    //   ShootState = 0;
    // }
    //Serial.println(YState);

  }
  if( C_now.rl && !C_past.rl ){
    //Serial.println("X button pressed");
    //XState = !XState;
    if (ShootState == 0){
      ShootState = 1;
    }
    else{
      ShootState = 0;
    }

  }  
  if( C_now.rr && !C_past.rr ){
    //Serial.println("B button pressed");
    BState = true;
    BStartTime = millis();

  }
  // if( C_now.rd && !C_past.rd ){
  if( C_now.rd ) {  
    //Serial.println("A button pressed");
    AState = true;
    AStartTime = millis();
  }
  else{
    AState = false;
  }

  if (C_now.r1 && !C_past.r1 ) {
    if (ShootState == 0){
      ShootState = 2;
    }
    else{
      ShootState = 0;
    }
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
    Serial.println("Drawer_On");
    motor6.run(-200);
    //ActionTime(AStartTime, 3000, 'A');    
  }
  if (AState == false){
    Serial.println("Drawer_Off");
    motor6.run(0);
  }
  if (BState == true){
    ActionTime(BStartTime, 800, 'B');
  }
  if (AfterBState == true){
    ActionTime(AfterBStartTime, 800, 'C');
  }
  if (ShootState == 1){
    digitalWrite(INA1, HIGH);
    digitalWrite(INB1, LOW);
    analogWrite(PWM1, 100);

    digitalWrite(INA2, HIGH);
    digitalWrite(INB2, LOW);
    analogWrite(PWM2, 250);
    Serial.println("Dribble");
  } 
  if (ShootState == 2){
    digitalWrite(INA1, LOW);
    digitalWrite(INB1, HIGH);
    analogWrite(PWM1, 250);

    digitalWrite(INA2, HIGH);
    digitalWrite(INB2, LOW);
    analogWrite(PWM2, 250);
    Serial.println("Shoot");
  } 
  if (ShootState == 0){
    digitalWrite(INA1, LOW);
    digitalWrite(INB1, HIGH);
    analogWrite(PWM1, 0);

    digitalWrite(INA2, HIGH);
    digitalWrite(INB2, LOW);
    analogWrite(PWM2, 0);
    //Serial.println("Stop");
  } 
  if (YState == true){
    ActionTime(YStartTime, 800, 'Y');
  }


}

void ActionTime(unsigned long start_time ,unsigned long time, char label){
  if( millis() - start_time >= time  )
      {
        //End process
        //Serial.println("End");
        // if (label == 'A'){
        //   motor6.run(0);
        //   AState = false;
        //   Serial.println("EndA");
          

        // }
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
          ShootState = 0;
          Serial.println("EndBDown");
        }
        if (label == 'Y'){
          motor5.run(0);
          YState = false;
          AfterBState = true;
          AfterBStartTime = millis();
          Serial.println("EndY");
        }
      }
  else{
    // if (label == 'A'){
    //   Serial.println("Drawer_On");
    //   motor6.run(120);
    // }   
    if (label == 'B'){
      Serial.println("Feeder_Up");
      motor5.run(255);
      if (millis() - start_time >= 450){
        ShootState = 1;
      }
    }
    if (label == 'C'){
      Serial.println("Feeder_Down");
      motor5.run(-255);
    }
    if (label == 'Y'){
      Serial.println("Feeder_Up");
      motor5.run(255);
      }
    }    
  }     

