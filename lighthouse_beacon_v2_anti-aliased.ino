//***************************************************************
// rotating beacon for lighthouse emulation, Anti-aliased version
// Marc Miller, Nov 2015
//
//Based on Mark Kriegsman's Anti-aliased light bar example here:
//http://pastebin.com/yAgKs0Ay#
//***************************************************************

#include "FastLED.h"
#define LED_TYPE NEOPIXEL
#define DATA_PIN 6
//#define CLOCK_PIN 13
#define NUM_LEDS 12
//#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];

int8_t hue = 42;  // Light color
int8_t sat = 190;  // Saturation
int8_t val = BRIGHTNESS;  // Max brightness of light
uint8_t pixelPos = 0; // position of the "fraction-based bar"
int delta = -1; // Number of 16ths of a pixel to move.  (Use negative value for reverse.)
uint8_t width  = 3; // width of light in pixels
uint16_t holdTime = 80;  // Milliseconds to hold between microsteps.
int8_t fadeRate = 40;  // Fade ends a bit.
 

//---------------------------------------------------------------
void setup() {
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  //Serial.begin(115200); 
}


//---------------------------------------------------------------
// Fractional bar funtion
void drawFractionalBar( uint8_t pos16, uint8_t width, uint8_t hue)
{
  uint8_t i = pos16 / 16; // convert from pos to raw pixel number
  uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position
  uint8_t firstpixelbrightness = 255 - (frac * 16);
  uint8_t lastpixelbrightness  = 255 - firstpixelbrightness;
  uint8_t bright;
  for( uint8_t n = 0; n <= width; n++) {
    if( n == 0) {
      bright = firstpixelbrightness;
      leds[i] += CHSV( hue, sat, bright);
      //fadeToBlackBy( leds, NUM_LEDS, fadeRate );  // creates outward fade
    }
    else if (n == width) {
      bright = lastpixelbrightness;
      leds[i] += CHSV( hue, sat, bright);
      fadeToBlackBy( leds, NUM_LEDS, fadeRate );  // creates outward fade
    }
    else {
      bright = 255;  // center pixel full bright
      leds[i] += CHSV( hue, sat, bright);
    }
    i++;
    if( i == NUM_LEDS) i = 0; // wrap around
  }
}

//---------------------------------------------------------------
void loop() {  // START MAIN LOOP
  if (delta > 0) {
    pixelPos += delta;
  } else {
    pixelPos = (pixelPos + delta + (NUM_LEDS *16)) % (NUM_LEDS * 16);
  }

  if( pixelPos >= (NUM_LEDS * 16)) {
    pixelPos -= (NUM_LEDS * 16);
  }

  static byte countdown = 0;
  if( countdown == 0) { countdown = 16; } // reset countdown
  countdown -= 1;  // decrement once each loop though

  memset8( leds, 0, NUM_LEDS * sizeof(CRGB));  // Clear the pixel buffer
  drawFractionalBar( pixelPos, width, hue);  // Draw the pixels
  FastLED.show();  // Show the pixels
  delay(holdTime);  // Delay a bit before micro advancing

}  // END MAIN LOOP
