//Understanding values from vvvv and sending them to the LED strips.
char packetBuffer[6];
void setup()   
{
  pinMode(9, OUTPUT);   // sets the digital pin # as output
  Serial.begin(9600); 
}
void loop() {
  
if (Serial.available() > 0) 
{
 Serial.readBytes(packetBuffer,6);    // 6 is the packetsize
 if( packetBuffer[0]=='<' && packetBuffer[6-1]=='>')
 {
 unsigned char CleanBuffer [6-2];
 memcpy(CleanBuffer,packetBuffer+1,6-2);
// Now we have a clean input buffer of 4 bytes to use.

 analogWrite(9,CleanBuffer[0]); 
 analogWrite(10,CleanBuffer[1]); 
 analogWrite(11,CleanBuffer[2]); 
 digitalWrite(12,CleanBuffer[3]);
 }
  
}
delay(1);
}
