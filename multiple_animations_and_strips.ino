
/*********************************************************************/
// Example OUTLINE (Not complete code) for running different patterns
// on 4 different strips at the same time.  This example is for a
// colored "star" shape with three different length "tails" streaming
// out from one side to give a "shooting star" sort of look.
//
// EVERY_N_MILLISECONDS is used to allow each strip (or tail) to be
// updated at a seperate rate as needed.  [Or each strip could be
// running it's own completely different pattern at whatever rate
// needed.
//
// Note that there is only one FastLED.show() call at the bottom of
// the program which updates all pixels at the same time.
//
//
// Marc Miller,  Oct 2019
/*********************************************************************/

#include "FastLED.h"
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define MASTER_BRIGHTNESS 255

#define DATA_PIN_STAR 11  // update pin numbers for data lines as needed
#define DATA_PIN_TAIL1 12
#define DATA_PIN_TAIL2 13
#define DATA_PIN_TAIL3 14

#define NUM_LEDS 50  // adjust number of pixels as needed
#define NUM_TAIL1 30
#define NUM_TAIL2 60
#define NUM_TAIL3 45

CRGB leds[NUM_LEDS];
CRGB tail1[NUM_TAIL1];
CRGB tail2[NUM_TAIL2];
CRGB tail3[NUM_TAIL3];


/*********************************************************************/
void setup() {
  Serial.begin(115200);  //allow for output to serial monitor
  delay(2500);  //power up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN_STAR, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN_TAIL1, COLOR_ORDER>(tail1, NUM_TAIL1);
  FastLED.addLeds<LED_TYPE, DATA_PIN_TAIL2, COLOR_ORDER>(tail2, NUM_TAIL2);
  FastLED.addLeds<LED_TYPE, DATA_PIN_TAIL3, COLOR_ORDER>(tail3, NUM_TAIL3);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  FastLED.clear();
  Serial.println("Setup done.");
}//end_setup


/*********************************************************************/
void loop() {

  //------code to color the star (first strip) here------
  EVERY_N_SECONDS(60) {
    // Use leds and NUM_LEDS in this section.
    fill_solid(leds, NUM_LEDS, CHSV(0, 255, 255));
  }


  //------code for tail1 pattern (second strip)------
  EVERY_N_MILLISECONDS(60) {
    // Putting the tail1 pattern stuff inside this EVERY_N allows
    //   the speed for just this tail to be individually set.
    // Instead of leds, use tail1 in this section, such as tail1[i] to
    //   update this strip's pixels.  Also use NUM_TAIL1 for loops lengths
    //   in this section, such as:
    //   for (int i = 0; i < NUM_TAIL1; i++) {
    ...do stuff here...
  }


  //------code for tail2 pattern------
  EVERY_N_MILLISECONDS(45) {
    // This tail can run a similar or different pattern and with it's own timing.
    // Use tail2[i] and NUM_TAIL2 in this section.
    ...do stuff here...
  }


  //------code for tail3 pattern------
  EVERY_N_MILLISECONDS(55) {
    // This tail can also do it's own thing as needed.
    ...do stuff here...
  }


  // Update the display for all strips.
  FastLED.show();

}//end_main_loop

