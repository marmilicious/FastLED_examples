// Marc Miller, Oct 2017

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

/* CRGB leds[NUM_LEDS];  <--not using this.  Using CRGBArray instead. */
CRGBArray<NUM_LEDS> leds;

CRGBSet partA(leds(0, NUM_LEDS / 2 - 1)); // First half of strip.
CRGBSet partB(leds(NUM_LEDS / 2, NUM_LEDS - 1)); // Second half of strip.


//---------------------------------------------------------------
void setup() {
    delay(500);
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {
    //Fill first half of strip with moving rainbow
    fill_rainbow( partA, NUM_LEDS / 2, millis() / 20);


    /*Three different ways to copy the data.
     * The second two mirror the data.
     * Uncomment one at a time to test out. */

    //partB = partA;  //creates dupicate copy in second half of strip
    leds(NUM_LEDS - 1, NUM_LEDS / 2) = partA; //creates mirrored copy in second half of strip
    //partB = leds(NUM_LEDS/2-1,0);  //creates mirrored copy in second half of strip


    FastLED.show();
    delay(2);
}

