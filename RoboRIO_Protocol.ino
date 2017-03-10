#include <FastLED.h>
#include <Wire.h>

const int NUM_LEDS = 10;
const int DATA_PIN = 0;
const int adjust = 32;

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
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Wire.begin();
  pinMode(DATA_PIN, OUTPUT);
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
    case 'K':
      return blackout;
      break;
  }
}

void setColor(const CRGB& color){
  for(int i = 0; i < NUM_LEDS; i++)
        leds[i] = color;
}

void chase(const CRGB& color){
  for (int i = 0; i != NUM_LEDS; ++i) {
    leds[i] = color;
    fade(leds[previous(i)]);
    fade(leds[previous(i, 2)]);
    fade(leds[previous(i, 3)]);
    leds[previous(i, 4)] = white;
    FastLED.show();
    delay(50);
  }
}

void downChase(const CRGB& color){
  for (int i = NUM_LEDS; i != 0; --i) {
    leds[i] = color;
    fade(leds[previous(i)]);
    fade(leds[previous(i, 2)]);
    fade(leds[previous(i, 3)]);
    leds[previous(i, 4)] = white;
    FastLED.show();
    delay(50);
  }
}

void fade(const CRGB& color){
}

void flash(const CRGB& color1, const CRGB& color2){
}

void loop() {
  
  if(Wire.available()){
    
    function = Wire.read();
    
    switch(function){
      case 'R':
      case 'W':
      case 'B':
      case 'G':
      case 'Y':
      case 'K':
        setColor(toColor(function));
        break;
      case 'C':
      case 'A':
      case 'F':
        while(!Wire.available())
          delay(1);
        x = Wire.read();
        break;
      case 'Z':
        while(Wire.available() < 2)
          delay(1);
        x = Wire.read();
        y = Wire.read();
         break;
  }

  switch(function){
    case 'R':
    case 'W':
    case 'G':
    case 'K':
    case 'Y':
    case 'B':
      break;
    case 'C':
      chase(toColor(x));
      break;
    case 'A':
      fade(toColor(x));
      break;
    case 'F':
      flash(toColor(x), toColor(y));
      break;  
  }
  
}

}
