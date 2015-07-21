/*
This example is adapted to receive multiple universes via Artnet and
control a strip of APA102 leds via FastLED library

Original example from https://github.com/natcl/Artnet
This example may be copied under the terms of the MIT license,
see the LICENSE file for details

-----------------------------------------------------

Awakening Circle ~ Interactive Visionary Installation
vulkai studio
https://github.com/sivinjski/AwakeningCircle

This Arduino code should understand packets of values from vvvv's DMX (Network Artnet Sender)
node and send them via Ethernet Shield on Arduino Mega 2560 board and pass them
via DATA Pin A9 (analog), CLOCK Pin A10 (analog) to an array of 360 [APA102] LEDs,
6 strips x 60 pixels.
*/

#include <Artnet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <FastLED.h>

#define NUM_LEDS 360
#define BRIGHTNESS  1 // from 1-31, over 2 makes glitches even though we are using 5V 55A 275W power supply on both sides of the array of 360 LEDs
// #define FRAMES_PER_SECOND 60 ************* testing
#define DATA_PIN A9
#define CLOCK_PIN A10
const int numberOfChannels = NUM_LEDS * 3; // Total number of channels you want to receive (1 led = 3 channels)

// An array of leds
CRGB leds[NUM_LEDS];
bool doSend = false;

// Artnet settings
Artnet artnet; //?/?// Is this line of code allright?
const int startUniverse = 0; // for most software this is 1, some send out artnet first universe as 0.

// Check if we got all universes
const int maxUniverses = numberOfChannels / 180 + ((numberOfChannels % 180) ? 1 : 0); ////////////// / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
int previousDataLength = 0;

// IP and MAC addresses
byte ip[] = {169, 254, 99, 180};
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; 

void setup()
{
  // delay(2000);
  // Serial.begin(115200);
  artnet.begin(mac, ip);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS);
  // initTest();

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  artnet.read();
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  sendFrame = 1;

  // Store which universe has got in
  if (universe < maxUniverses)
    universesReceived[universe] = 1;

  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      // Serial.println("Broke");
      sendFrame = 0;
      break;
    }
  }

  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < NUM_LEDS)
      leds[led].setRGB( data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
       
  }
  previousDataLength = length;     
  
  if (sendFrame)
  {
    FastLED.show();
    // FastLED.delay(1000 / FRAMES_PER_SECOND); ************* testing
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}
