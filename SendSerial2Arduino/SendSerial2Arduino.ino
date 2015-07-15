// Understanding values from vvvv [RS232 (Devices String Legacy) node] and
// sending them to the array of 360 APA102 pixels (serial), 6 strips x 60 pixels

// Arduino Mega 2560
// DATA pin A9 (analog)
// CLOCK pin A10 (analog) 

char packetBuffer[6];
void setup()
{
  // Output pins
  pinMode(9, OUTPUT); // DATA
  pinMode(10, OUTPUT); // CLOCK
  Serial.begin(9600);
}

void loop() {
  
if (Serial.available() > 0) 
{
 Serial.readBytes(packetBuffer,8); // 6 is the packetsize
 if( packetBuffer[0]=='<' && packetBuffer[6-1]=='>')
  {
  unsigned char CleanBuffer [6-2];
  memcpy(CleanBuffer,packetBuffer+1,6-2);
  // Now we have a clean input buffer of 4 bytes to use.
   
  analogWrite(9,CleanBuffer[0]); 
  analogWrite(10,CleanBuffer[1]); 
  analogWrite(11,CleanBuffer[2]); 
  analogWrite(12,CleanBuffer[3]); // (test) changed from digitalWrite to analogWrite
  }
}
delay(5);
}
