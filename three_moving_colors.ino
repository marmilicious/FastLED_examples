//***************************************************************
// Three moving colors.
//
// Marc Miller,  Jan. 2019
//  April 2019 - Updated to use EVERY_N instead of delay().
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 64
CRGB leds[NUM_LEDS];

int16_t positionRed = NUM_LEDS*2/3;  // Set initial start position of Red
int16_t positionWhite = NUM_LEDS/3;  // Set initial start position of White
int16_t positionBlue = 0;  // Set initial start position of Blue

#define holdTime 120  // Milliseconds to hold position before advancing
int8_t delta = 1;  // 1 or -1.  Sets forward or backwards direction.


//---------------------------------------------------------------
void setup() {
  delay(1500);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {

  EVERY_N_MILLISECONDS(holdTime) {

    //set position and color of pixels
    leds[(positionRed + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Red;
    leds[(positionWhite + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Grey;
    leds[(positionBlue + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Blue;
  
    FastLED.show();  //show the pixels
   
    //advance position based on delta, and rollover if needed.
    positionRed = ((positionRed + delta + NUM_LEDS) % NUM_LEDS);
    positionWhite = ((positionWhite + delta + NUM_LEDS) % NUM_LEDS);
    positionBlue = ((positionBlue + delta + NUM_LEDS) % NUM_LEDS);

  }//end_every_n

}//end_main_loop

