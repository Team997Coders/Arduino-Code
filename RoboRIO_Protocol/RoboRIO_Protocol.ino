#include <FastLED.h>
#include <Wire.h>

const int NUM_LEDS = 61;
const int DATA_PIN = 2;
const int CLOCK_PIN = 3;
const int DATA_PIN = 4;
const int CLOCK_PIN = 5;
const int BRIGHTNESS = 60;

CRGB leds[NUM_LEDS];

const CRGB white(127, 127, 127);
const CRGB blue(0, 0, 255);
const CRGB red(255, 0, 0);
const CRGB blackout(0, 0, 0);
const CRGB green(0, 255, 0);
const CRGB yellow(255, 255, 0);

char function = '0';
char x = '0';
char y = '0';

void setup() {
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);;
  FastLED.addLeds<APA102, DATA2_PIN, CLOCK2_PIN, BGR>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);;
  FastLED.setBrightness( BRIGHTNESS );
  setColor(toColor('B'));

   Serial.begin(9600);
}

CRGB toColor(char color){
  switch(color){
    case 'R':
      return red;
      break;
    case 'B':
      return blue;
      break;
    case 'Y':
      return yellow;
      break;
    case 'W':
      return white;
      break;
    case 'G':
      return green;
      break;
    case 'K':
      return blackout;
      break;
  }
}

void setColor(const CRGB& color){
  for(int i = 0; i < NUM_LEDS; i++)
        leds[i] = color;
  FastLED.show();
  delay(50);
}

//glitter effect
void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
  FastLED.show();
  delay(50);
}

// Theatre-style crawling lights with a spacing of 3
void theaterChase(const CRGB& color, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 4; q++) {
      for (int i=0; i < NUM_LEDS; i=i+4) {
        leds[q+i] = color;    //turn every third pixel on
      }
      FastLED.show();
      delay(wait);
      for (int i=0; i < NUM_LEDS; i=i+3) {
        leds[q+i] = 0;        //turn every third pixel off
      }
    }
  }
  FastLED.show();
  delay(50);
}

void chase(const CRGB& color){
  while (Serial.available() == 0) {
    for (int i = 0; i <= NUM_LEDS; ++i) {
      leds[i] = color;
      if (i > 0){
        leds[i-1].nscale8(8);
      }
      FastLED.show();
      delay(50);
    }
    delay(50);
  }
}

void downChase(const CRGB& color){
  while (Serial.available() == 0) {
  for (int i = NUM_LEDS; i >= 0; --i) {
    leds[i] = color;
    if (i < NUM_LEDS) {
      leds[i+1].nscale8(8);
    }
    FastLED.show();
    delay(50);
  }
  delay(100);
  }
}

/*
 * Commands:
 * Colors: R = Red
 *        B = Blue
 *        G = Green
 *        Y = Yellow
 *        W = White
 *        K = blackout
 *  Funmctions:
 *        U = chase up
 *        D = chase down
 *        F = fade to black
 *        Q = add glitter (white sparkles)
 */
void loop() {
    //function = "K";
    while(!Wire.available())
      delay(1);
    function = Wire.read();
       
    //if (Serial.available() <= 0)
    //  delay(1);
    // get incoming byte:
    //function = Serial.read();    
    //  Serial.println(function);
        
    switch(function){
        case 'R':
        case 'W':
        case 'B':
        case 'G':
        case 'Y':
        case 'K':
          setColor(toColor(function));
          break;
        case 'Q':
          addGlitter(30);
          break;
        case 'U':
          setColor(toColor('K'));
          theaterChase(white, 50);
          break;
        case 'D':
          setColor(toColor('K'));
          downChase(white);
          break;
        case 'F':
          //changing the third variable changes how quickly the lights fade
          fadeToBlackBy( leds, NUM_LEDS, 10);
          break;
          
          //while(!Wire.available())
          //  delay(1);
          //x = Wire.read();
          //break;
        //case 'Z':
        //  while(Wire.available() < 2)
        //    delay(1);
        //  x = Wire.read();
        //  y = Wire.read();
        //   break;
      }
}
