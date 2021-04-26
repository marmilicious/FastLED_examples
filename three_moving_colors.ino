//***************************************************************
// Three moving colors.
//     Uses modulo, %, to make pixel position "loop" around and
//     stay in valid pixel range.
//
// Marc Miller,  Jan. 2019
//     Apr. 2019 - updated to use EVERY_N instead of delay()
//     Apr. 2021 - cleaned up some duplicate code
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 64
CRGB leds[NUM_LEDS];

// Set initial start position of each color
int16_t positionA = NUM_LEDS*2/3;
int16_t positionB = NUM_LEDS/3;
int16_t positionC = 0;

const uint16_t holdTime = 120;  // Adjusts speed of travel
int8_t delta = 1;  // 1 or -1.  Sets travel direction


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

    // Fading tail effect.  Comment out for solid colors
    fadeToBlackBy( leds, NUM_LEDS, 100);
    
    // assign pixel colors
    leds[positionA] = CRGB::Red;
    leds[positionB] = CRGB::Grey;  // Using grey so not as bright
    leds[positionC] = CRGB::Blue;
  
    FastLED.show();  // Show the pixels
   
    // Advance position based on delta, and rollover if needed.
    positionA = ((positionA + delta + NUM_LEDS) % NUM_LEDS);
    positionB = ((positionB + delta + NUM_LEDS) % NUM_LEDS);
    positionC = ((positionC + delta + NUM_LEDS) % NUM_LEDS);

  }//end every_n
  
}
