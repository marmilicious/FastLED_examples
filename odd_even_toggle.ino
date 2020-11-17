// Example showing how to operate on odd or even pixels.
//
// Odd pixels are assigned one hue, and even pixels a different
// hue.  The brightness of odd and even pixels is also toggled
// at a specified regular interval.
//
//
// Marc Miller,  Nov 2020
//***************************************************************

#include "FastLED.h"
#define DATA_PIN 11
#define CLOCK_PIN 13
#define LED_TYPE LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS 32
#define BRIGHTNESS 128
CRGB leds[NUM_LEDS];


const uint16_t toggleSpeed = 900;  // how fast to toggle hueA and B [time in ms]
uint8_t hueOffset = 42;  // values from 20-128 are good
uint8_t hueA = 105;
uint8_t hueB = hueA + hueOffset;
uint8_t valueA = 255;
uint8_t valueB = 128;  // set the same as valueA for no brightness toggling
boolean newHue = false;


//===============================================================
void setup() {
  Serial.begin(115200);
  delay(1500);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done.\n");  
}
 

//===============================================================
void loop() {

  EVERY_N_MILLISECONDS(toggleSpeed) {

    if (newHue) {
      hueA = hueA + random8(64,128);  // pick a new hue
      hueB = hueA + hueOffset;
      newHue = false;  // set back to false
    }

    // toggle brightness A and B values
    uint8_t temp = valueA;
    valueA = valueB;
    valueB = temp;

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      if ( i % 2 == 0) {
        // pixel is even number
        leds[i] = CHSV(random8(hueA,hueA+20), 255, valueA );
      } else {
        //pixel is odd number
        leds[i] = CHSV(random8(hueB,hueB+20), 255, valueB );
      }
    }

    FastLED.show();
  }

  EVERY_N_SECONDS(7) {
    newHue = true;
  }
  
}  // end main loop


//---------------------------------------------------------------
/* Different ways to check if a value is odd or even:

  if ( value % 2 == 0) {
    // is even number
  } else {
    // is odd number
  }
  
  if ( mod8(value,2) == 0) {
    // is even number
  } else {
    // is odd number
  }
  
  if ((value & 0x01) == 0) {
    // is even number
  } else {
    // is odd number
  }

*/
