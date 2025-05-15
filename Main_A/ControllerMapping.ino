void processController()
{
  if( C_now.rt && !C_past.rt  ){
    Serial.println("Right Top pressed");
  }
  if( C_now.rl && !C_past.rl ){
    Serial.println("Right Left pressed");
  }
  if( C_now.rr && !C_past.rr ){
    Serial.println("Right Right pressed");
  }
  if( C_now.rd && !C_past.rd ){
    Serial.println("RIght Down pressed");
  }

  Serial.print(C_now.x);
  Serial.print(" ");
  Serial.println(C_now.y);

  Serial.print(C_now.rx);
  Serial.print(" ");
  Serial.println(C_now.ry);
}