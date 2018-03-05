#include <FastLED.h>

const int NUM_LEDS = 61;
const int DATA_PIN = 3;
const int CLOCK_PIN = 4;
const int BRIGHTNESS = 64;
#define FRAMES_PER_SECOND 30
bool gReverseDirection = false;
bool glitterMode = false;

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
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  setColor(toColor('K'));

  Serial.begin(9600);
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
  for(int i = 0; i < NUM_LEDS; i++)
        leds[i] = color;
  FastLED.show();
  delay(50);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if (glitterMode) {
    if( random8() < chanceOfGlitter) {
      leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
  }
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

void fireworks() {
  while (Serial.available() == 0) {
    Fire2012(); // run simulation frame
  
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
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
 *        F = fireworks
 */
void loop() {
    //function = "K";
    
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
        case 'Q':
          glitterMode = ~glitterMode;
          break;
        case 'U':
          setColor(toColor('K'));
          chase(white);
          break;
        case 'D':
          setColor(toColor('K'));
          downChase(white);
          break;
        case 'F':
          fireworks();
          break;
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
