#include <SoftwareSerial.h>
#include <Mapf.h>
#include <string.h>
SoftwareSerial Conn(4, 5);

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
Controller_Status C_data;
Controller_Status C_msg;

String msg;

void setup()
{
  Serial.begin(9600);
  while (!Serial) ;
  Conn.begin(9600);
}
 
void loop()
{
  
  ReadController(  );
  delay(50);
}

void ReadController( )
{
  
    msg = Conn.readStringUntil('\n');
    //Serial.println(msg);
    for(int i = 0; i < msg.length() ; i++)
    {
//JOY STRICK
///LEFT
      if( msg[i] == 'W' )
      {
        i++;
        String ka = "";
        while( isdigit(msg[i]) && i != msg.length() )
        {
          ka += msg[i];
          i++;
        }
        C_msg.y = mapf(ka.toInt(),0,255,1,-1);
        i--;
      }
      if( msg[i] == 'P'){
        i++;
        String ka = "";
        while( isdigit(msg[i]) && i != msg.length() )
        {
          ka += msg[i];
          i++;
        }
        C_msg.x = mapf(ka.toInt(),0,255,-1,1);
        i--;
      }
////RIGHT
      if( msg[i] == 'Q'){
        i++;
        String ka = "";
        while( isdigit(msg[i]) && i != msg.length() )
        {
          ka += msg[i];
          i++;
        }
        C_msg.ry = mapf(ka.toInt(),0,255,1,-1);
        i--;
      }
      if( msg[i] == 'S'){
        i++;
        String ka = "";
        while( isdigit(msg[i]) && i != msg.length() )
        {
          ka += msg[i];
          i++;
        }
        C_msg.rx = mapf(ka.toInt(),0,255,-1,1);
        i--;
      }
///////// RIGHT BUTTON ///////////
      if( msg[i] == 'I')C_msg.rt = true;


      if( msg[i] == 'L')C_msg.rr = true;


      if( msg[i] == 'K')C_msg.rl = true;


      if( msg[i] == 'J')C_msg.rd = true;

///////// LEFT PAD ///////////
      if( msg[i] == 'A')C_msg.lt = true;


      if( msg[i] == 'C')C_msg.ll = true;


      if( msg[i] == 'B')C_msg.ld = true;


      if( msg[i] == 'D')C_msg.lr = true;

////// R1 R2
      if( msg[i] =='M')C_msg.r1 = true;


      if( msg[i] == 'N')C_msg.r2 = true;

///////L1 L2
      if( msg[i] == 'E')C_msg.l1 = true;


      if( msg[i] == 'F')C_msg.l2 = true;


/////// Start Select
      if( msg[i] == 'G')C_msg.select = true;


      if( msg[i] == 'H')C_msg.home = true;
      //Serial.println(i);

    }
    C_data = C_msg;
    C_msg = {};
}