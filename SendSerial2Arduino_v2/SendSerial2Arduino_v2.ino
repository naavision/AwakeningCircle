// Awakening Circle ~ Interactive Visionary Installation
// vulkai studio
// https://github.com/sivinjski/AwakeningCircle

// This Arduino code should understand packets of values from vvvv serial port and send them
// via Arduino Mega 2560 board, DATA Pin A9 (analog), CLOCK Pin A10 (analog)
// to an array of 360 [APA102] LEDs, 6 strips x 60 pixels.

// In this second version (SendSerial2Arduno_v2) there could be some mess between
// FastLED.h and Adafrit_DotStar.h libraries: such as NUM_LEDS vs. NUMPIXELS, etc. etc.

#include <FastLED.h>
#include <SPI.h>
#include <Adafruit_DotStar.h> // Library for APA102, labeled by Adafruit as DotStar 
                              // https://github.com/adafruit/Adafruit_DotStar

#define NUM_LEDS 360
//FastLED// #define DATA_PIN A9
//FastLED// #define CLOCK_PIN A10

#define NUMPIXELS 360
#define DATAPIN  A9
#define CLOCKPIN A10
  
CRGB leds[NUM_LEDS]; // Array for RGB LEDs
int LEDValues[NUM_LEDS][3]; // 2D array (360,3) to store RGB values for every LED
char packetBuffer[1080]; // Buffer to store incoming string from vvvv

void setup()
{
  Adafruit_DotStar strip = Adafruit_DotStar(NUM_LEDS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
  //// FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS); // initiate RGB LED strip
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

