//***************************************************************
// This example shows how you can create a custom array of
// specific pixels to operate on.  This might be useful if you
// wanted to light up specific pixels on a strip that are not a
// continuous sequence.  You can also quickly create a basic
// changing pattern by animating pixel number offsets as
// demonstated here.
//
// **Note**: You must define NUM_LEDS as 32 or more when running
// this example since specific pixel numbers have been hardcoded
// into some arrays and we must be able to actually write data
// to those pixels.  (If your setup is less then 32 pixels, you
// should still set NUM_LEDS to 32, you just won't see some of
// the patterns.)
//
//
// Also, see this example by chemdoc77 to take the idea even
// further by using arrays of arrays to make animated patterns.
// https://github.com/chemdoc77/CD77_FastLED/tree/master/CD77_NeoMatrix_8X8_panel_Fun_with_FastLED
//
//
// Marc Miller, May 2021
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 128


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const uint8_t customArray[] = {1,2,3,6,9,12,13,14};
const uint8_t customArraySize = ARRAY_SIZE(customArray);

const uint8_t pulseArray[] = {16,17,21,22};
const uint8_t pulseArraySize = ARRAY_SIZE(pulseArray);

uint8_t hue;
uint8_t hueStep;
boolean togglePulse;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1500);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("\nSetup done.\n");
}

//---------------------------------------------------------------
void loop() {
  EVERY_N_MILLISECONDS(25) {
    hue++;
  }

  EVERY_N_SECONDS(3) {
    hueStep = (hueStep + 1) % 17;
  }

  // fill entire strip for some animted background color
  fill_solid(leds, NUM_LEDS, CHSV(hue,190,130) );


  // first part of strip using customArray
  for (uint8_t i = 0; i < customArraySize; i++) {
    uint8_t hueNew = hue + 15 + (hueStep * 21);
    leds[ customArray[i] ] = CHSV(hueNew,230,255);
  }


  // second part of strip displays pulseArray pattern
  EVERY_N_MILLISECONDS(750) {
    togglePulse = !togglePulse;
  }
  
  for (uint8_t i = 0; i < pulseArraySize; i++) {
    if (togglePulse) {
      leds[ pulseArray[i] ]     = CHSV(hue+80,210,255);
      // duplicate pattern down strip, shifted by 9 pixels
      leds[ pulseArray[i] + 9 ] = CHSV(hue+80,210,255);
    } else {
      // shift original pattern over one pixel
      leds[ pulseArray[i] + 1 ] = CHSV(hue+80,180,180);
      // duplicate pattern down strip, shifted by 8 pixels
      leds[ pulseArray[i] + 8 ] = CHSV(hue+80,180,180);
    }
  }

  FastLED.show();

}
