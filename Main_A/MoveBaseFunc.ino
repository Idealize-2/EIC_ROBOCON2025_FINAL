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

  // Run DriveBase
  motor1.run(speed1 * LIMIT_SPEED);
  motor2.run(-speed2 * LIMIT_SPEED);
  motor3.run(-speed3 * LIMIT_SPEED);
  motor4.run(speed4 * LIMIT_SPEED);

}