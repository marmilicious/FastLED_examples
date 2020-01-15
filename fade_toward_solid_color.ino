/****************************************************************
This example displays a color on all pixels, then blends the
display to a new color and holds it for a bit.  Then repeats...

The serial monitor will print out the target color when a new
one is picked.

Marc Miller, Jan 2020
****************************************************************/

#include "FastLED.h"
#define LED_TYPE      LPD8806
#define COLOR_ORDER   GRB
#define DATA_PIN      11
#define CLOCK_PIN     13   // ignore for clockless pixel types
#define NUM_LEDS      32
uint8_t BRIGHTNESS = 128;  // master brightness [0-255]
CRGB leds[NUM_LEDS];

CRGB currentColor(130,25,25);  // starting color
CRGB targetColor(30,40,130);   // starting target color
const uint8_t fadeRate = 42;   // larger number is a slower fade

boolean fadeToColor = 1;  // turns on/off the fading toward target

  
//===============================================================
void setup() {
  Serial.begin(115200);
  delay(3000);  // (optional) safety start up delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);  // set initial master brightness

  fill_solid(leds, NUM_LEDS, currentColor);
  FastLED.show();   // update the display
  Serial.println("\nSetup done.\n");
}


//===============================================================
void loop() {

  EVERY_N_SECONDS(15) {
    newTargetColor();  // set the new target color
    fadeToColor = 1;  // start fading toward target
  }

  EVERY_N_MILLISECONDS(fadeRate) {
    if (currentColor == targetColor) {
      fadeToColor = 0;  // stop fading
    }
    if (fadeToColor) {
      nblendU8TowardU8( currentColor.r, targetColor.r);
      nblendU8TowardU8( currentColor.g, targetColor.g);
      nblendU8TowardU8( currentColor.b, targetColor.b);
      currentColor = CRGB(currentColor.r,currentColor.g,currentColor.b);
    }

    fill_solid(leds, NUM_LEDS, currentColor);

    // For Testing --- always display target on first pixel
    leds[0] = targetColor;

    FastLED.show();
  }

} // end_main_loop


//===============================================================
// Modified helper function that blends one uint8_t toward another,
// based on function from Mark Kriegsman's fadeTowardColor example:
// https://gist.github.com/kriegsman/d0a5ed3c8f38c64adcb4837dafb6e690
void nblendU8TowardU8(uint8_t& current, const uint8_t target)
{
  if( current == target) {
    return;
  }

  if( current < target ) {
    uint8_t delta = target - current;
    delta = scale8_video( delta, 1);
    current += delta;
  } else {
    uint8_t delta = current - target;
    delta = scale8_video( delta, 1);
    current -= delta;
  }
}


//---------------------------------------------------------------
// Pick a new target color to fade toward
void newTargetColor() {
  random16_add_entropy( random() );
  targetColor = CHSV( random8(), 255, 255);

  Serial.print("New targetColor ");
  Serial.print("\tr: "); Serial.print(targetColor.r);
  Serial.print("\tg: "); Serial.print(targetColor.g);
  Serial.print("\tb: "); Serial.println(targetColor.b); 
}


//---------------------------------------------------------------
