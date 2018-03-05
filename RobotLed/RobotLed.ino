#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 51 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define LEFT_DATAPIN    2
#define LEFT_CLOCKPIN   3
#define RIGHT_DATAPIN   4
#define RIGHT_CLOCKPIN  5

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

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


//https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
void setup() {
  delay( 2000 ); // power-up safety delay
  
  FastLED.addLeds<DOTSTAR, LEFT_DATAPIN, LEFT_CLOCKPIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<DOTSTAR, RIGHT_DATAPIN, RIGHT_CLOCKPIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  setColor(toColor('R'));
}

int previous(int i, int num = 1, int min = 0, int max = NUM_LEDS) {
    for (; num != 0; --num) {
        if (i == min) {
            i = max;
        }
        --i;
    }
    return i;
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
  for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color;
  }
  FastLED.show();
  delay(50);
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
 *        F = fireworks
 */
void loop() {
    function = "";
    
    if (Serial.available() > 0) {
      // get incoming byte:
       function = Serial.read();    
      Serial.println(function);
      //setColor(toColor(function)}
    
      //function = 'K';
        
      switch(function){
        case 'R':
        case 'W':
        case 'B':
        case 'G':
        case 'Y':
        case 'K':
          setColor(toColor(function));
          break;
        //case 'U':
        //  setColor(toColor('K'));
        //  chase(white);
        //  break;
        //case 'D':
        //  setColor(toColor('K'));
        //  downChase(white);
        //  break;
        //case 'F':
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
}
