// Example of using XY mapping and CRGBSet
// Marc Miller, Jan 2017
//***************************************************************

#include "FastLED.h"

//--------------start XY mapping stuff----------------
// Params for width and height
const uint8_t kMatrixWidth = 33;
const uint8_t kMatrixHeight = 1;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
//CRGB leds[NUM_LEDS];  // ***Not using this.  Using CRGBArray instead.***
CRGBArray<NUM_LEDS> leds;

#define LAST_VISIBLE_LED 31
uint8_t XY (uint8_t x, uint8_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint8_t XYTable[] = {
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  32,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31
  };

  uint8_t i = (y * kMatrixWidth) + x;
  uint8_t j = XYTable[i];
  return j;
}
//---------------end XY mapping stuff-----------------

CRGBSet partA(leds(6,9));  // Define custom pixel range with a name.
CRGBSet partB(leds(22,25));  // Define custom pixel range with a name.

CHSV colorOne(0,222,255);  // Define a custom color.
CHSV colorTwo(82,222,255);  // Define a custom color.



//---------------------------------------------------------------
void setup() { 
  delay(500);
  FastLED.addLeds<LPD8806, 11, 13, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

//---------------------------------------------------------------
void loop()
{
  fill_rainbow(leds, NUM_LEDS, millis()/4);  // fill strip with moving rainbow.
  leds.fadeToBlackBy(230);  // fade the whole strip down some.
  partA = colorOne;  // set partA pixel color.
  partB = colorTwo;  // set partB pixel color.
  FastLED.show();

  EVERY_N_SECONDS(2){  // Swaps the two custom colors every four seconds.
    CHSV temp = colorOne;
    colorOne = colorTwo;
    colorTwo = temp;
  }

  EVERY_N_SECONDS(14){  // blank strip.
    FastLED.clear();  // Clear strip
    //memset8(leds, 0, NUM_LEDS * 3);  // Blank out strip
    FastLED.show();
    delay(2000);
  }
  
}

