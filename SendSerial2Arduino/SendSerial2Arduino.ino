// Understanding values from vvvv [RS232 (Devices String Legacy) node] and
// sending them to the array of 360 APA102 pixels (serial), 6 strips x 60 pixels

// Arduino Mega 2560, DATA pin A9 (analog), CLOCK pin A10 (analog)
// *analogWrite() works on pins 2 - 13 and 44 - 46.

int dataPin = A9; // DATA Pin
int clockPin = A10;  // CLOCK Pin

char packetBuffer[6];

void setup()
{
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  // pinMode(12, OUTPUT);  // sets the digital pin 12 as output
  Serial.begin(9600);
}

void loop() {
  
if (Serial.available() > 0) 
{
 Serial.readBytes(packetBuffer,6); // 6 is the packetsize
 if( packetBuffer[0]=='<' && packetBuffer[6-1]=='>')
  {
  unsigned char CleanBuffer [6-2];
  memcpy(CleanBuffer,packetBuffer+1,6-2);
  // Now we have a clean input buffer of 4 bytes to use.
   
  analogWrite(dataPin,CleanBuffer[0]); 
  analogWrite(clockPin,CleanBuffer[1]); 
  // analogWrite(9,CleanBuffer[0]); 
  // analogWrite(10,CleanBuffer[1]); 
  // analogWrite(11,CleanBuffer[2]); 
  // digitalWrite(12,CleanBuffer[3]); 
  }
}
delay(5); // wait 5 milliseconds before the next loop
}
