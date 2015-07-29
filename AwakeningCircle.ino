/*
Awakening Circle ~ Interactive Visionary Installation
https://github.com/sivinjski/AwakeningCircle

vulkai studio
http://www.vulkai.com

Code by:
Ritschard Paul     https://github.com/ritschard
Danijel Sivinjski  https://github.com/sivinjski

-----------------------------------------------------

Setup:
- vvvv (Kinect2 part + triggers are sent to this Arduino Code via RS232 node)
- Arduino Mega 2560 (DATA and CLOCK pin defined below)
- Array of 360 [APA102] LEDs (6 strips x 60 pixels)
*/

#include <FastLED.h>

#define LED_TYPE      APA102
#define DATA_PIN      51
#define CLOCK_PIN     53
#define BRIGHTNESS    31
#define COLOR_ORDER   BGR
#define MAX_INT_VALUE 65536
#define NUM_LEDS      360
CRGB leds[NUM_LEDS];

uint16_t frame        = 0;
uint16_t animateSpeed = 10;
uint8_t  brightness   = 20;
uint16_t fadeAmount   = 9;
uint8_t  animation    = 0;

//// ##############################
//// CHAKRA COLORS
//// ##############################
CRGB RED    = CRGB(255, 0, 0);
CRGB ORANGE = CRGB(255, 127, 0);
CRGB YELLOW = CRGB(255, 255, 0);
CRGB GREEN  = CRGB(0, 255, 0);
CRGB BLUE   = CRGB(0, 0, 255);
CRGB INDIGO = CRGB(75, 0, 130);
CRGB VIOLET = CRGB(143, 0, 255);

//// ##############################
//// CHAKRA POSITIONS (offset)
//// ##############################
#define ROOT    0
#define SACRAL  8
#define PLEXUS 21 
#define HEART  33
#define THROAT 43
#define EYE    50
#define CROWN  55

int chakraPositions[] = {ROOT, SACRAL, PLEXUS, HEART, THROAT, EYE, CROWN};
int chakraLedStripOffsets[] = {0, 60, 120, 180, 240, 300}; 

CRGB getChakraColor(int chakra){
    switch(chakra){
      case ROOT:
        return RED;
      break; 
      case SACRAL:
        return ORANGE;
      break; 
      case PLEXUS:
        return YELLOW;
      break; 
      case HEART:
        return GREEN;
      break; 
      case THROAT:
        return BLUE;
      break;
      case EYE:
        return INDIGO;
      break;
      case CROWN:
        return VIOLET;
      break;  
    }
}

CRGB getColor(int chakra){
    switch(chakra){
      case 0:
        return RED;
      break; 
      case 1:
        return ORANGE;
      break; 
      case 2:
        return YELLOW;
      break; 
      case 3:
        return GREEN;
      break; 
      case 4:
        return BLUE;
      break;
      case 5:
        return INDIGO;
      break;
      case 6:
        return VIOLET;
      break;  
    }
}

//// ##############################
//// Host Sculptures/Strips
//// ##############################
//// 0  =  Root   =  Red
//// 1  =  Sacral =  Orange
//// 2  =  Plexus =  Yellow
//// 3  =  Heart  =  Green
//// 4  =  Throat =  Blue
//// 5  =  Eye    =  Indigo
//// 6  =  Crown  =  Violet

int head  = 0, tail = -4;
int rndval = 0;

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(12)>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  set_max_power_in_milliwatts(275000);
}

int prevColor = -1;
int prevStrip = -1;
int del = 100;

/*
void serialEvent() {
  while (Serial.available()) {
    FastLED.clear();
    animation = Serial.read();
  }
}
*/

void loop() {
  
  while (Serial.available()) // Don't read unless there you know there is data
  {
    FastLED.clear();
    animation = Serial.read();
  }
  
  switch (animation)
  {

//// ChakraRandom
  case 0:
    rndval = random(0,7);
  ChakraRandom(leds, chakraPositions[rndval]);
  break;
  
//// ChakraInAll
  case 1:
  ChakraInAll(ROOT);
  delay(10);
  break;
  
  case 2:
  ChakraInAll(HEART);
  delay(10);
  break;

  case 3:
  ChakraInAll(EYE);
  delay(10);
  break;    
  
  case 4:
  ChakraInAll(PLEXUS);
  delay(10);
  break;
  
  case 5:
  ChakraInAll(CROWN);
  delay(10);
  break;

  case 6:
  ChakraInAll(SACRAL);
  delay(10);
  break;
  
  case 7:
  ChakraInAll(THROAT);
  delay(10);
  break;

//// Snake
  case 8:
    rndval = random(0,7);
    if (rndval != prevColor)
    {
  Snake(leds, getColor(rndval), 60);
    prevColor = rndval;
    }
  break;
  
//// ChakraInHost
  case 9:
  ChakraInHost(leds);
  delay(10);
  break;        

//// RandomDot
  case 10:
    rndval = random(0,7);
    if (rndval != prevColor)
    {
  RandomDot(leds, getColor(rndval));
    prevColor = rndval;
    }
  break;

//// SnakeSpeedUp
  case 11:
    rndval = random(0,7);
    if (rndval != prevColor)
    {
  Snake(leds, getColor(rndval), del);
    del = del / 1.5;
    if (del <= 1)
    del = 60;  
    prevColor = rndval;
    }
  break;

//// ChakraFullRandom
  case 12:
  rndval = random(0,7);
    if (rndval != prevColor)
    {
  ChakraFullRandom(leds, getColor(rndval));
    prevColor = rndval;
  break;

//// ChakraSpectar
  case 13:
  ChakraSpectar(leds);
  break;

//// Awakening A
  case 14:
  Awakening(false, 10);
  break;

//// ##############################

//// TRIGGER #1 ~ ChakraFull
  case 15:
  ChakraFull(leds);
  delay(10);
  break;

//// TRIGGER #2 ~ Awakening B
  case 16:
  Awakening(true, 1);
  break;

//FastLED.show();
}
}
}


void fillChakra(int chakra, byte brightness, int offset){
  CRGB color = getChakraColor(chakra);
  for(int i = 0; i < 5; i++ ) {
    leds[i+chakra+offset] = color;
    leds[i+chakra+offset].nscale8(brightness);
    FastLED.show();
  } 
  //show_at_max_brightness_for_power();
}

void fillChakraFull(int chakra, int chakraLedStripOffsets){
  rndval = random(0,7);
    if (rndval != prevColor)
    {
  CRGB color = getColor(rndval);
  prevColor = rndval;
  for(int i = 0; i < 60; i++ ) {
    leds[i+chakraLedStripOffsets] = color;
    FastLED.show();
  } 
  //show_at_max_brightness_for_power();
}
}


//// ################################################
//// ChakraRandom
//// ################################################
//// Randomly lighting up only one chakra at time, not considersing host sculptures.
void ChakraRandom(CRGB leds[], int chakra) {
    CRGB chakraColor = getChakraColor(chakra);
    int offset = random(0,6);
    FastLED.clear();   
    for(int i = 0; i < 5; i++ ) {
    leds[i+chakra+chakraLedStripOffsets[offset]] = CRGB(chakraColor);;
    FastLED.show();
  }
  delay(1000);  
}




//// ################################################
//// ChakraInAll
//// ################################################
//// Pulsating only one chakra at time in all sculptures.
//// Additional idea: First it pulsates in its host sculpture for 1 minute and then in all
//// other sculptures.
void ChakraInAll(int chakra) {
  for(int j=0; j<6; j++){
    fillChakra(chakra, brightness, chakraLedStripOffsets[j]);
  }
  brightness = brightness + fadeAmount;
  if(brightness == 0 || brightness == 255) {fadeAmount = -fadeAmount ;}
  delay(120);
}




//// ################################################
//// Snake
//// ################################################
//// Only 3 pixels in one color are going like snake from the root of first sculpture
//// till the end of array. After it finishes other 3 pixels start in other chakra color.
void Snake(CRGB leds[], CRGB color, int del) {
  for(int i = 0; i < NUM_LEDS-3; i++ ) {
    memset(leds, 0, i * 3);
    leds[i] = color; 
    leds[i+1] = color; 
    leds[i+2] = color; 
    FastLED.show();
    delay(del);
  }  
  memset(leds, 0, NUM_LEDS * 3);
}




//// ################################################
//// ChakraInHost
//// ################################################
//// Lighting up only one chakra at time. Each host sculpture lights up its chakra.
//// Note: There are 7 chakras but 6 sculptures, so  the CROWN / VIOLET chakra is missing here.
void ChakraInHost(CRGB leds[]) {
  for(int i = 0; i < 5; i++ ) {
    leds[i] = CRGB(RED);
    leds[i].fadeLightBy(brightness);
    leds[i+SACRAL+60] = CRGB(ORANGE);
    leds[i+SACRAL+60].fadeLightBy(brightness);
    leds[i+PLEXUS+60*2] = CRGB(YELLOW);
    leds[i+PLEXUS+60*2].fadeLightBy(brightness);
    leds[i+HEART+60*3] = CRGB(GREEN);
    leds[i+HEART+60*3].fadeLightBy(brightness);
    leds[i+THROAT+60*4] = CRGB(BLUE);
    leds[i+THROAT+60*4].fadeLightBy(brightness);
    leds[i+EYE+60*5] = CRGB(INDIGO);
    leds[i+EYE+60*5].fadeLightBy(brightness);
    FastLED.show();
  }
  brightness = brightness + fadeAmount;
  if(brightness == 0 || brightness == 255) {fadeAmount = -fadeAmount ;}
  delay(140);
}




//// ################################################
//// RandomDot
//// ################################################
void RandomDot(CRGB leds[], CRGB color) {
  memset(leds, 0, NUM_LEDS * 3);
  CRGB chakraColor = getColor(color);

  int randomLED = random(0, NUM_LEDS-1);   
  for(int offset = 0; offset < 25; offset ++) { 
    if(offset % 2 == 0) { 
      leds[randomLED] = CRGB(color);
    }
    else { 
      leds[randomLED] = CRGB(0,0,0);  
    }
    FastLED.show();
    delay(40);
  }
}




//// ################################################
//// ChakraFullRandom
//// ################################################
//// Full strips are pulsating in their host chakra color, randomly one after another.
void ChakraFullRandom(CRGB leds[], CRGB color) {
    CRGB chakraColor = getColor(color);
    FastLED.clear();
    int offset = random(0,6);
    if (offset != prevStrip) {
      for(int i=0; i<1; i++){
      fillChakraFull(color, chakraLedStripOffsets[offset]);
    prevStrip = offset;
    }
    delay(200);
}
}




//// ################################################
//// ChakraSpectar
//// ################################################
//// All chakras are shining one strip at time, and that repeats on all strips.
void ChakraSpectar(CRGB leds[]) {
  int offset = random(0,6);
  FastLED.clear();
  for(int i = 0; i < 5; i++ ) {
    leds[i+chakraLedStripOffsets[offset]] = CRGB(RED);
    leds[i+SACRAL+chakraLedStripOffsets[offset]] = CRGB(ORANGE);
    leds[i+PLEXUS+chakraLedStripOffsets[offset]] = CRGB(YELLOW);
    leds[i+HEART+chakraLedStripOffsets[offset]] = CRGB(GREEN);
    leds[i+THROAT+chakraLedStripOffsets[offset]] = CRGB(BLUE);
    leds[i+EYE+chakraLedStripOffsets[offset]] = CRGB(INDIGO);
    leds[i+CROWN+chakraLedStripOffsets[offset]] = CRGB(INDIGO);
  }
  FastLED.show();
  delay(200);
}




//// ################################################
//// ChakraFull
//// ################################################
//// Full strips are pulsating in their host chakra color, randomly one after another.
void ChakraFull(CRGB leds[]) {
  for(int i = 0; i < 60; i++ ) {
    leds[i] = CRGB(RED);
    leds[i+60] = CRGB(ORANGE);
    leds[i+60*2] = CRGB(YELLOW);
    leds[i+60*3] = CRGB(GREEN);
    leds[i+60*4] = CRGB(BLUE);
    leds[i+60*5] = CRGB(INDIGO);
  }
  FastLED.show();
  delay(70);
}




//// ################################################
//// Awakening
//// ################################################
//// All strips are higlighting chakras from root~red to crown~violet, first pulsating slowly
//// all together and gradually speeding up.
void Awakening(bool clr, int del) {
   //int del = 200;
   for (int k=0; k<7; k++){ 
    FastLED.clear();
     for (int i=0; i<7; i++){
      switch(i){
        case 0:
          for(int j=0; j<6; j++){
            fillChakra(ROOT, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            FastLED.clear();
        break; 
        case 1:
          for(int j=0; j<6; j++){
            fillChakra(SACRAL, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            FastLED.clear();
        break;
        case 2:
          for(int j=0; j<6; j++){
            fillChakra(PLEXUS, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            FastLED.clear();
        break;
        case 3:
          for(int j=0; j<6; j++){
            fillChakra(HEART, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            FastLED.clear();
        break;
        case 4:
          for(int j=0; j<6; j++){
            fillChakra(THROAT, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            FastLED.clear();
        break;
        case 5:
          for(int j=0; j<6; j++){
            fillChakra(EYE, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            FastLED.clear();
        break;
        case 6:
          for(int j=0; j<6; j++){
            fillChakra(CROWN, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            FastLED.clear();
        break; 
      }
     }
     //del = del/2;
   }
   
  brightness = brightness + fadeAmount;
  if(brightness == 0 || brightness == 255) {fadeAmount = -fadeAmount ;}  
}
