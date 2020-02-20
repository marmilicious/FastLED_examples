
/*********************************************************************/
// Example outline (incomplete code) for running different patterns
// on 4 different LED strips (strip A,B,C,D) at the same time.
//
// EVERY_N_MILLISECONDS is used to allow each strip to be updated at
// a seperate rate, and each strip can be running a different pattern.
//
// Note that there is only one FastLED.show() call at the bottom of
// the program which updates all pixels at the same time.
//
//
// Marc Miller,  Feb 2020
/*********************************************************************/

#include "FastLED.h"
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define MASTER_BRIGHTNESS 255

#define DATA_PIN_A 11  // update pin numbers for data lines as needed
#define DATA_PIN_B 12
#define DATA_PIN_C 13
#define DATA_PIN_D 14

#define NUM_LEDS_A 50  // adjust number of pixels as needed
#define NUM_LEDS_B 30
#define NUM_LEDS_C 60
#define NUM_LEDS_D 45

CRGB ledsA[NUM_LEDS_A];
CRGB ledsB[NUM_LEDS_B];
CRGB ledsC[NUM_LEDS_C];
CRGB ledsD[NUM_LEDS_D];


/*********************************************************************/
void setup() {
  Serial.begin(115200);  //allow for output to serial monitor
  delay(2500);  //power up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN_A, COLOR_ORDER>(ledsA, NUM_LEDS_A);
  FastLED.addLeds<LED_TYPE, DATA_PIN_B, COLOR_ORDER>(ledsB, NUM_LEDS_B);
  FastLED.addLeds<LED_TYPE, DATA_PIN_C, COLOR_ORDER>(ledsC, NUM_LEDS_C);
  FastLED.addLeds<LED_TYPE, DATA_PIN_D, COLOR_ORDER>(ledsD, NUM_LEDS_D);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  FastLED.clear();
  Serial.println("Setup done.");
}//end_setup


/*********************************************************************/
void loop() {

  //------code for the pattern on the first strip, ledsA------
  // This will run the stuff inside the EVERY_N section every 4 seconds
  // Use ledsA and NUM_LEDS_A in this section to update date for strip A.

  EVERY_N_SECONDS(4) {
    static uint8_t hueA;
    fill_solid(ledsA, NUM_LEDS_A, CHSV(hueA, 255, 255));
    hueA++;
  }



  //------code for the second strip, ledsB here------
  // Putting the ledsB pattern inside this EVERY_N section allows
  // the speed for just this strip to be individually adjusted.
  // This will run every 500 milliseconds.
  // Use ledsB in this section, such as ledsB[i], and NUM_LEDS_B
  // with for loops so it uses the correct length of strip B.
  // For example:

  EVERY_N_MILLISECONDS(500) {
    for (int i = 0; i < NUM_LEDS_B; i++) {
      ledsB[i] = do stuff here...
  }



  //------code for ledsC pattern------
  // This strip can run a similar or different pattern, and have
  // it's own timing.  This will run every 35 milliseconds.
  // Use ledsC[i] and NUM_LEDS_C in this section.

  EVERY_N_MILLISECONDS(35) {
    ...do stuff for ledsC here...
  }



  //------code for ledsD pattern------
  // This strip can also do it's own individual thing.  
  // Use ledsD and NUM_LEDS_D in this section.

  EVERY_N_MILLISECONDS(150) {
    fill_rainbow( ledsD, NUM_LEDS_D, millis()/20);
  }


  // Update the display for all strips.
  FastLED.show();

}//end_main_loop

