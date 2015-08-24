/*
Awakening Circle ~ Interactive Visionary Installation
https://github.com/sivinjski/AwakeningCircle

vulkai studio
http://www.vulkai.com

Code by:
Ritschard Paul     https://github.com/ritschard
Danijel Sivinjski  https://github.com/sivinjski

Library:
Pololu Arduino library for APA102 https://github.com/pololu/apa102-arduino
(slower; for longer cables/distance between strips)

Setup:
- vvvv (Kinect2 triggers are sent to this Arduino Code via serial port)
- Arduino Mega 2560 (DATA and CLOCK pin defined below)
- Array of 360 [APA102] LEDs (6 strips x 60 pixels)
*/

#include <APA102.h>

const uint8_t dataPin = 51;
const uint8_t clockPin = 53;
const uint16_t ledCount = 360;

APA102<dataPin, clockPin> ledStrip;

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

//// ##############################
//// CHAKRA COLORS
//// ##############################
rgb_color RED = (rgb_color) { 255, 0, 0 };
rgb_color ORANGE = (rgb_color) { 255, 127, 0 };
rgb_color YELLOW = (rgb_color) { 255, 255, 0 };
rgb_color GREEN = (rgb_color) { 0, 255, 0 };
rgb_color BLUE = (rgb_color) { 0, 0, 255 };
rgb_color INDIGO = (rgb_color) { 75, 0, 130 };
rgb_color VIOLET = (rgb_color) { 143, 0, 255 };
rgb_color BLACK = (rgb_color) { 0 ,0 ,0 };

uint8_t animation = 0;
uint8_t brightness = 5;
rgb_color colors[ledCount];

int chakraPositions[] = {ROOT, SACRAL, PLEXUS, HEART, THROAT, EYE, CROWN};
int chakraLedStripOffsets[] = {0, 60, 120, 180, 240, 300}; 

int rndval = 0;
int prevColor = -1;
int prevStrip = -1;
int del = 100;

rgb_color getChakraColor(int chakra) {
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

rgb_color getColor(int chakra) {
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

void setup() { 
  Serial.begin(9600);
}


void loop() {

  while (Serial.available()) 
  {
    clearLeds();
    animation = Serial.read();
  }

  switch (animation)
  {
    case 0:
      rndval = random(0,7);
      clearLeds();
      ChakraRandom(chakraPositions[rndval]);
    break;

    case 1:
      ChakraInAll(ROOT);
    break;

    case 2:
      ChakraInAll(HEART);
    break;

    case 3:
      ChakraInAll(EYE);
    break;    

    case 4:
      ChakraInAll(PLEXUS);
    break;

    case 5:
      ChakraInAll(CROWN);
    break;

    case 6:
      ChakraInAll(SACRAL);
    break;

    case 7:
      ChakraInAll(THROAT);
    break;

//  'Snake' animation removed

    case 8:
      ChakraInHost();
    break;  

//  'RandomDot' animation removed

    case 9:
      rndval = random(0,7);
      if (rndval != prevColor) {
        ChakraFullRandom(rndval);
        prevColor = rndval;
      delay(500);
      }
    break;

    case 10:
      ChakraSpectar();
    break;

    case 11:
      Awakening(false, 10);
    break;

    case 12:
    ChakraFull();
    delay(10);
    break;

    case 13:
    Awakening(true, 400);
    break;
  }
}

void clearLeds() {
  for (int i = 0; i < ledCount; i++) {
  colors[i] = BLACK;
  }  
}

void clearLed(int i) {
  colors[i] = BLACK;
}

void fillChakra(int chakra, byte brightness, int offset){
  rgb_color color = getChakraColor(chakra);
  for(int i = 0; i < 5; i++ ) {
    colors[i+chakra+offset] = color;
  } 
  ledStrip.write(colors, ledCount, brightness);
}

void fillChakraFull(int chakra, int chakraLedStripOffsets){
  rgb_color color = getColor(chakra);
    for(int i = 0; i < 60; i++ ) {
      colors[i+chakraLedStripOffsets] = color; 
    } 
    ledStrip.write(colors, ledCount, brightness);
}

void fillChakraFullRandomColor(int chakraLedStripOffsets){
  rndval = random(0,7);
  if (rndval != prevColor) {
    rgb_color color = getColor(rndval);
    prevColor = rndval;
    for(int i = 0; i < 60; i++ ) {
      colors[i+chakraLedStripOffsets] = color;
    } 
    ledStrip.write(colors, ledCount, brightness);
  }
}


//// ################################################
//// ChakraRandom
//// ################################################
//// Randomly lighting up only one chakra at time, not considersing host sculptures.
void ChakraRandom(int chakra) {
  rndval = random(0,6);
  if (rndval != prevColor) {
    prevColor = rndval;
    fillChakra(chakra, brightness, chakraLedStripOffsets[rndval]);
  }
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
  delay(120);
}


//// ################################################
//// ChakraInHost
//// ################################################
//// Lighting up only one chakra at time. Each host sculpture lights up its chakra.
//// Note: There are 7 chakras but 6 sculptures, so  the CROWN / VIOLET chakra is missing here.
void ChakraInHost() {
  clearLeds();
  for(int i = 0; i < 5; i++) {
    colors[i] = RED;
    colors[i+SACRAL+60] = ORANGE;
    colors[i+PLEXUS+60*2] = YELLOW;
    colors[i+HEART+60*3] = GREEN;
    colors[i+THROAT+60*4] = BLUE;
    colors[i+EYE+60*5] = INDIGO;
  }
  ledStrip.write(colors, ledCount, brightness);
  delay(140);
}


//// ################################################
//// ChakraFullRandom
//// ################################################
//// Full strips are pulsating in their host chakra color, randomly one after another.
void ChakraFullRandom(int chakra) {
  clearLeds();
  int offset = random(0,6);
  if (offset != prevStrip) {
    fillChakraFull(chakra, chakraLedStripOffsets[offset]);
    prevStrip = offset;
  }
  // delay(200);
}


//// ################################################
//// ChakraSpectar
//// ################################################
//// All chakras are shining one strip at time, and that repeats on all strips.
void ChakraSpectar() {
  int offset = random(0,6);
  clearLeds();
  for(int i = 0; i < 5; i++ ) {
    colors[i+chakraLedStripOffsets[offset]] = RED;
    colors[i+SACRAL+chakraLedStripOffsets[offset]] = ORANGE;
    colors[i+PLEXUS+chakraLedStripOffsets[offset]] = YELLOW;
    colors[i+HEART+chakraLedStripOffsets[offset]] = GREEN;
    colors[i+THROAT+chakraLedStripOffsets[offset]] = BLUE;
    colors[i+EYE+chakraLedStripOffsets[offset]] = INDIGO;
    colors[i+CROWN+chakraLedStripOffsets[offset]] = INDIGO;
  }
  ledStrip.write(colors, ledCount, brightness);
  delay(200);
}


//// ################################################
//// ChakraFull
//// ################################################
//// Full strips are pulsating in their host chakra color, randomly one after another.
void ChakraFull() {
  for(int i = 0; i < 60; i++ ) {
    colors[i] = RED;
    colors[i+60] = ORANGE;
    colors[i+60*2] = YELLOW;
    colors[i+60*3] = GREEN;
    colors[i+60*4] = BLUE;
    colors[i+60*5] = INDIGO;
  }
  ledStrip.write(colors, ledCount, brightness);
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
    clearLeds();
    for (int i=0; i<7; i++){
      switch(i){
        case 0:
          for(int j=0; j<6; j++){
            fillChakra(ROOT, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            clearLeds();
        break; 
        case 1:
          for(int j=0; j<6; j++){
            fillChakra(SACRAL, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            clearLeds();
        break;
        case 2:
          for(int j=0; j<6; j++){
            fillChakra(PLEXUS, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            clearLeds();
        break;
        case 3:
          for(int j=0; j<6; j++){
            fillChakra(HEART, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            clearLeds();
        break;
        case 4:
          for(int j=0; j<6; j++){
            fillChakra(THROAT, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            clearLeds();
        break;
        case 5:
          for(int j=0; j<6; j++){
            fillChakra(EYE, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            clearLeds();
        break;
        case 6:
          for(int j=0; j<6; j++){
            fillChakra(CROWN, brightness, chakraLedStripOffsets[j]);
          }
          delay(del);
          if (clr)
            clearLeds();
        break; 
      }
    }
  //del = del/2;
  }
}

/*
//// ################################################
//// Snake
//// ################################################
//// Only 3 pixels in one color are going like snake from the root of first sculpture
//// till the end of array. After it finishes other 3 pixels start in other chakra color.
void Snake(rgb_color color, int del) {
  
  for(int i = 0; i < ledCount-3; i++ ) {
    memset(colors, 0, i * 3);
    colors[i] = color; 
    colors[i+1] = color; 
    colors[i+2] = color;    
    ledStrip.write(colors, ledCount, brightness);
    delay(del);
  }  
  memset(colors, 0, ledCount * 3);
}


//// ################################################
//// RandomDot
//// ################################################
void RandomDot(rgb_color color) {
  memset(colors, 0, ledCount * 3);

  int randomLED = random(0, ledCount-1);   
  for(int offset = 0; offset < 25; offset ++) { 
    if(offset % 2 == 0)  
      colors[randomLED] = color;
    else 
      colors[randomLED] = BLACK;
    ledStrip.write(colors, ledCount, brightness);
  }
  //delay(40);
}
*/
