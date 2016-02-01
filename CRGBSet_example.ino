#include "FastLED.h"
#define NUM_LEDS 32

//CRGB leds[NUM_LEDS];  // Not using this.  Using CRGBArray instead.
CRGBArray<NUM_LEDS> leds;

CRGBSet partA(leds(6,9));  // Define custom pixel range with a name.
CRGBSet partB(leds(22,25));  // Define custom pixel range with a name.

CHSV colorOne(0,222,255);  // Define a custom color.
CHSV colorTwo(82,222,255);  // Define a custom color.

//---------------------------------------------------------------
void setup() { 
  FastLED.addLeds<LPD8806, 11, 13, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

//---------------------------------------------------------------
void loop()
{
  fill_rainbow(leds, NUM_LEDS, millis()/10);  // fill strip with moving rainbow.
  leds.fadeToBlackBy(230);  // fade the whole strip down some.
  partA = colorOne;  // set partA pixel color.
  partB = colorTwo;  // set partB pixel color.
  FastLED.show();

  EVERY_N_SECONDS(4){  // Swaps the two custom colors every four seconds.
    CHSV temp = colorOne;
    colorOne = colorTwo;
    colorTwo = temp;
  }
}

