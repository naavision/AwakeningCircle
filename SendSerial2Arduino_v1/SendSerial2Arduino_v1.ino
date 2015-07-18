// Awakening Circle ~ Interactive Visionary Installation
// vulkai studio
// https://github.com/sivinjski/AwakeningCircle

// This Arduino code should understand packets of values from vvvv serial port and send them
// via Arduino Mega 2560 board, DATA Pin A9 (analog), CLOCK Pin A10 (analog)
// to an array of 360 [APA102] LEDs, 6 strips x 60 pixels.

#include <FastLED.h>

#define NUM_LEDS 360
#define DATA_PIN A9    // Analog 9
#define CLOCK_PIN A10  // Analog 10
CRGB leds[NUM_LEDS];   // Array for RGB LEDs
int LEDValues[NUM_LEDS][3]; // 2D array (360,3) to store RGB values for every LED
char packetBuffer[1080]; // Buffer to store incoming string from vvvv

void setup()
{
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS); // initiate RGB LED strip
  Serial.begin(9600); // start Serial Communication
}
void loop() {
  if (Serial.available() > 0) // Check if data is coming in through serial
  {
    Serial.readBytes(packetBuffer,1080);// 1080 is the packetsize = 1080 values + '<' and '>'
    if( packetBuffer[0]=='<' && packetBuffer[1080-1]=='>') // if first and last chars of string are '<' and '>' we know the whole package has arrived
    {
      unsigned char CleanBuffer [1080-2]; // new Array to store string without starting and closing tag
      memcpy(CleanBuffer,packetBuffer+1,1080-2); // copies the content from packetBuffer to CleanBuffer excluding the first and last element
      
      int index=0; // helper to point to a specific data point
      
      for(int i=0; i<NUM_LEDS; i++) // nested for loop to run through two dimensional array
      {
        for(int j=0; j<3; j++)
        {
          LEDValues[i][j] = CleanBuffer[index]; // store values of CleanBuffer in 2D Array
          index++; // increase index to get next data point in next run through the loop
        }
      }
    }
  }
  
  for(int i=0; i<NUM_LEDS; i++) // for loop to push values to LEDs
  {
    leds[i] = CRGB(LEDValues[i][0],LEDValues[i][1],LEDValues[i][2]);
  }
  
  FastLED.show(); // updated RGB LED strip
  delay(5);
}

