// Marc Miller, Oct 2017

#include "FastLED.h"
#define NUM_LEDS 32

//CRGB leds[NUM_LEDS];  // Not using this.  Using CRGBArray instead.
CRGBArray<NUM_LEDS> leds;

CRGBSet partA(leds(0,NUM_LEDS/2-1));  // First half of strip.
CRGBSet partB(leds(NUM_LEDS/2,NUM_LEDS-1));  // Second half of strip.


//---------------------------------------------------------------
void setup() { 
  delay(500);
  FastLED.addLeds<LPD8806, 11, 13, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

//---------------------------------------------------------------
void loop()
{
  //Fill first half of strip with moving rainbow
  fill_rainbow( partA, NUM_LEDS/2, millis()/20);


  /*Three different ways to copy the data.
   * The second two mirror the data.
   * Uncomment one at a time to test out. */
   
  //partB = partA;  //creates dupicate copy in second half of strip
  leds(NUM_LEDS-1,NUM_LEDS/2) = partA;  //creates mirrored copy in second half of strip
  //partB = leds(NUM_LEDS/2-1,0);  //creates mirrored copy in second half of strip
  

  FastLED.show();
  delay(2);
}

