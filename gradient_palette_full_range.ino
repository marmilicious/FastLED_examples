//***************************************************************
// This example displays the full range of a custom gradient
// palette over the full range of a pixel string.  This is done
// by mapping the palette index to the pixel number.
//
// More info on gradient palettes here:
// https://github.com/FastLED/FastLED/wiki/Gradient-color-palettes
//
//
// *Also Note*:
// In order to support smooth blending for "circular" palettes,
// each color blends with "the next", and in the case of the last
// color, "the next" is the first color since it wraps around,
// so the last 15 numbers are blending back toward index 0.
// Sometime circular blending is very useful, other times it
// might not be what you want.  You can use index 240 as your
// last value to avoid blending.
//
// For this example I decided to Not have the display wrap back
// around, so I'm specifically using a palette index of 0-240.
//
// ***FastLED update in May 2023*** ColorFromPalette also now has
// the blend type: LINEARBLEND_NOWRAP
// This should be similar and not blend the end colors back into
// the start colors, and you could still index all the way to 255.
//
// Marc Miller, Oct 2018
//              Aug 2025 - Updated to not use delay(). Also made it
//                         more interesting by animating the pixels
//                         coming on and fading everything off.
//                         Tweaked the custom palette too.
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  128

bool SHOW_COLORS = true;
uint16_t pixelNumber;  // used to keep track of pixel position
uint16_t count;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);  // startup delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

// Create custom gradient palette
DEFINE_GRADIENT_PALETTE( example_gp ) {
  0,    0,0,255,
  3,    0,40,200,
 50,    0,255,0,
115,    30,50,120, 
120,    80,50,255, 
190,    255,0,255,
238,    220,190,130,  // I'm making this my last color choice
255,    220,190,130 };  // And then just duplicating it here

CRGBPalette16 myPal = example_gp;  


//---------------------------------------------------------------
void loop () {

  EVERY_N_SECONDS(6) {  //do this every 6 seconds
    SHOW_COLORS = true;  //time to run the animation again!
    FastLED.clear();  //set all the pixels back to black
  }


  if (SHOW_COLORS == true) {
    EVERY_N_MILLISECONDS(50) {
      uint8_t paletteIndex = map(pixelNumber,0,NUM_LEDS-1,0,240);  //palette index is now based on pixel number
      leds[pixelNumber] = ColorFromPalette( myPal, paletteIndex, BRIGHTNESS, LINEARBLEND);
      pixelNumber++;  //go to next pixel
      if (pixelNumber == NUM_LEDS) {
        SHOW_COLORS = false;  //stop indexing up through the pixels
        pixelNumber = 0;  //reset pixel count for next time
        count = 0;  //reset count
      }
    }
  } else {  //ie. SHOW_COLORS is false
    EVERY_N_MILLISECONDS(10) {
      if (count >= 300) {  //don't start fading for 10x300 = 3000 milliseconds
        fadeToBlackBy( leds, NUM_LEDS, 5);  //fade all pixels a bit
      }
      count++;
    }
  }

  FastLED.show();


}//end_main_loop


