//***************************************************************
// If we want a pattern to repeat every 5 pixels then the
// first pixel set to light up would be: 0,5,10,15,20...
// This can be represented by:  leds[5 * (x - 1) + 5]
//
// Let's put this into a loop with some variables!
//
//
// Marc Miller, Nov 2015
//              May 2020 - replaced all delays with EVERY_N
//              June 2021 - renamed some variables
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 100
CRGB leds[NUM_LEDS];

uint8_t hue;     // Pixel color

// How often does the pattern repeat?
static uint16_t repeatEvery = 5;

// Therefore the number of times the pattern will repeat down the strip is:
static uint16_t numberOfRepeats = NUM_LEDS/repeatEvery;


//---------------------------------------------------------------
void setup() { 
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2500);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  Serial.print("NumberOfRepeats = "); Serial.print(numberOfRepeats);
  Serial.print(" within the strip of length "); Serial.println(NUM_LEDS);
  Serial.println("Setup done.");
}

//---------------------------------------------------------------
void loop() {

  EVERY_N_MILLISECONDS(50) {
    fadeToBlackBy( leds, NUM_LEDS, 7);  // slowly fade out pixels
  }

  EVERY_N_MILLISECONDS(500) {
    static uint8_t hueOffset;
    static uint8_t hueShift;
    
    //hue = ((255 / repeatEvery) * hueOffset) + hueShift;  // Change the hue for each pixel set.
    hue = (20 * hueOffset) + hueShift;  // Change the hue for each pixel set.
    
    for (uint16_t x = 0; x < numberOfRepeats + 1; x++) {
      static uint16_t i;
      i = (repeatEvery * (x - 1)) + repeatEvery + hueOffset;  // The pixel number to draw
      if (i < NUM_LEDS) {  // Only draw pixel numbers within NUM_LEDS
        leds[i] = CHSV(hue,180,255);
      }
    }

    hueOffset++;
    if (hueOffset == repeatEvery) {
      hueOffset = 0;
      hueShift = hueShift + random8(55,77);
    }

  } //end_every_n

  FastLED.show();  // Display all pixels in a set at the same time.


} //end_main_loop
