//***************************************************************
// Rotating beacon for lighthouse emulation, Anti-aliased version
//
// Based on Mark Kriegsman's Anti-aliased light bar example here:
// http://pastebin.com/yAgKs0Ay#
//
// Marc Miller, Nov 2015
//   Dec 2023 - fixed some variable types, replaced the delay
//              with EVERY_N_MILLISECONDS, formatting
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define COLOR_ORDER GRB
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 255

uint8_t hue = 42;  // Lighthouse beacon color
uint8_t sat = 190;  // Saturation
uint8_t val = BRIGHTNESS;  // Max brightness of light

int delta = 1; // Number of 16ths of a pixel to move.  (Use negative value for reverse.)
int width  = 3; // width of light in pixels
uint16_t holdTime = 80;  // Milliseconds to hold between microsteps. (Bigger moves slower.)
uint8_t fadeRate = 40;  // Fade out end a bit.

int pixelPos = 0; // position of the "fraction-based bar" [don't edit]


//===============================================================
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500);  // Power-up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done.\n");
}


//===============================================================
void loop() {

  EVERY_N_MILLISECONDS(holdTime) {  // wait a bit before micro advancing
    if (delta > 0) {
      pixelPos += delta;
    } else {
      pixelPos = (pixelPos + delta + (NUM_LEDS * 16)) % (NUM_LEDS * 16);
    }

    if( pixelPos >= (NUM_LEDS * 16)) {
      pixelPos -= (NUM_LEDS * 16);
    }

    static byte countdown = 0;
    if( countdown == 0) { countdown = 16; } // reset countdown
    countdown -= 1;  // decrement once each time through

    memset8( leds, 0, NUM_LEDS * sizeof(CRGB));  // Clear the pixel buffer
    drawFractionalBar( pixelPos, width, hue);  // Draw the pixels
  }

  FastLED.show();  // Show the pixels

} // end_main_loop


//---------------------------------------------------------------
// Fractional bar funtion
void drawFractionalBar( int pos16, int width, uint8_t hue)
{
  int i = pos16 / 16; // convert from pos to raw pixel number
  uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position
  uint8_t firstpixelbrightness = 255 - (frac * 16);
  uint8_t lastpixelbrightness  = 255 - firstpixelbrightness;
  uint8_t bright;
  for( uint8_t n = 0; n <= width; n++) {
    if( n == 0) {
      bright = firstpixelbrightness;  // first pixel in the bar
      leds[i] += CHSV( hue, sat, bright);
    }
    else if (n == width) {
      bright = lastpixelbrightness;  // last pixel in the bar
      leds[i] += CHSV( hue, sat, bright);
      fadeToBlackBy( leds, NUM_LEDS, fadeRate );  // creates outward fade
    }
    else {
      bright = 255;  // center pixel always max bright
      leds[i] += CHSV( hue, sat, bright);
    }
    i++;
    if( i == NUM_LEDS) i = 0; // wrap around
  }
}
