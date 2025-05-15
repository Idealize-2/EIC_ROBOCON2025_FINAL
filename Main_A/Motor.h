class MotorI2C {       // The class
  private:             // Access specifier
    int driverAddress;
    int motorAddress;
    bool swap_dir = false;
  public:
    MotorI2C( int driverAddress, int motorAddress)
    {
      this->driverAddress = driverAddress;
      this->motorAddress = motorAddress;
    }
    MotorI2C( int driverAddress, int motorAddress , bool swap_dir)
    {
      this->driverAddress = driverAddress;
      this->motorAddress = motorAddress;
      this->swap_dir = swap_dir;
    }

    void run( float pwm )
    {
      Wire.beginTransmission( this->driverAddress );
      Wire.write( this->motorAddress );
      int dir;
      if( swap_dir ) dir = pwm > 0  ? 2 : 1;
      else dir = pwm > 0  ? 1 : 2;

      if (pwm == 0) { dir = 3; }
      Wire.write(dir);
      Wire.write(int(abs(pwm)));
      Wire.endTransmission();
    }

    
    void Swap()
    {
      this->swap_dir = !this->swap_dir;
    }
    void Swap( bool dir )
    {
      this->swap_dir = dir;
    }
    int dir()
    {
      return this->swap_dir;
    }

    

};

class MotorPIN{
    private:
      int in1Pin;
      int in2Pin;
      int pwmPin;
    public:
      MotorPIN( int in1Pin , int in2Pin , int pwmPin )
      {
        this->in1Pin = in1Pin;
        this->in2Pin = in2Pin;
        this->pwmPin = pwmPin;
        pinMode( in1Pin , OUTPUT );
        pinMode( in2Pin , OUTPUT );
        pinMode( pwmPin , OUTPUT );
      }

      void run( float pwm )
      {
        if( pwm < 0)
        {
          digitalWrite( in1Pin , LOW );
          digitalWrite( in2Pin , HIGH );
        }
        else
        {
          digitalWrite( in1Pin , HIGH );
          digitalWrite( in2Pin , LOW );
        }
        
        analogWrite( pwmPin , abs(pwm) );
      }
      void run( float pwm , long time )
      {
        digitalWrite( in1Pin , HIGH );
        digitalWrite( in2Pin , LOW );
        analogWrite( pwmPin , pwm );

        delay( time );
        stop();

      }
      void stop()
      {

        digitalWrite( in1Pin , LOW );
        digitalWrite( in2Pin , LOW );
        analogWrite( pwmPin , 0 );

      }
};

