void initiateMotors() {
  // Motors
  Wire.begin(MOTOR_SDA, MOTOR_SCL);

  //------------------------ Example Init Encoder
  //                pinMode( teleEncoderPinA , INPUT );
  //                 pinMode( teleEncoderPinB , INPUT );

  //                 // Enable the weak pull up resistors
  //                 ESP32Encoder::useInternalWeakPullResistors = puType::up;
  //                 teleEncoder.attachFullQuad( teleEncoderPinA , teleEncoderPinB ); 
}

void resetEncoders() {
  //------------------------ Example Reste Encoder
  //                  //--------recently added-------//
  //                  teleEncoder.setCount( 0 );
  //                  teleEncoder.setFilter( 10 ); 
  //                  //-----------------------------//
}

void movebase(int rpm, float theta, int turn) {
  
  float sin_theta = sin(theta - M_PI / 4);
  float cos_theta = cos(theta - M_PI / 4);
  float max_value = max(abs(sin_theta), abs(cos_theta));
  int speed1 , speed2 , speed3 , speed4;

  speed1 = rpm * cos_theta / max_value + turn;
  speed2 = rpm * sin_theta / max_value + turn;
  speed3 = rpm * sin_theta / max_value - turn;
  speed4 = rpm * cos_theta / max_value - turn;

  unsigned long now = millis();

  if (now - lastUpdateTime >= 50) {  // Update every 50 ms (~20 Hz)
    lastUpdateTime = now;

    // Exponential Moving Average
    smoothedSpeed1 = (smoothFactor * speed1) + ((1 - smoothFactor) * smoothedSpeed1);
    smoothedSpeed2 = (smoothFactor * speed2) + ((1 - smoothFactor) * smoothedSpeed2);
    smoothedSpeed3 = (smoothFactor * speed3) + ((1 - smoothFactor) * smoothedSpeed3);
    smoothedSpeed4 = (smoothFactor * speed4) + ((1 - smoothFactor) * smoothedSpeed4);

    // Serial.println(smoothedSpeed);
  }

  // Run DriveBase
  motor1.run(smoothedSpeed1 * LIMIT_SPEED);
  motor2.run(-smoothedSpeed2 * LIMIT_SPEED);
  motor3.run(-smoothedSpeed3 * LIMIT_SPEED);
  motor4.run(smoothedSpeed4 * LIMIT_SPEED);

}