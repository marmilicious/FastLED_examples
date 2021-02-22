/****************************************************************
mirror display and faded out ends of strip
This example shows one basic way to mirror the display on
a strip.  It also fades out the ends of the strip to black.  

This example fades the ends over 7 pixels.


Marc Miller, Feb 2021
****************************************************************/

#include "FastLED.h"

#define LED_TYPE      LPD8806
#define COLOR_ORDER   GRB
#define DATA_PIN      11
#define CLOCK_PIN     13  // delete for clockless pixel types
#define NUM_LEDS      32
uint8_t BRIGHTNESS = 255;
CRGB leds[NUM_LEDS];


//===============================================================
void setup() {
  Serial.begin(115200);
  delay(2000);  // safety start up delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done.\n");
}

//===============================================================
void loop() {

  // fill first half the strip with a rainbow
  fill_rainbow( leds, NUM_LEDS / 2, (millis() / 50) );
  
  // mirror data
  mirror();

  // fade ends by X number of pixels
  fadeEnds(7);

  // update the display
  FastLED.show();

} // end_main_loop



//---------------------------------------------------------------
// mirror data to the other half
void mirror() {
  for (uint8_t i = 0; i < NUM_LEDS / 2; i++) {
    leds[NUM_LEDS - 1 - i] = leds[i];
  }
}


//---------------------------------------------------------------
// fade both ends of strip over X number of pixels
void fadeEnds(uint8_t fadeOver) {
  uint8_t fadePP = 255 / fadeOver;  // fade per pixel
  for (uint8_t i = 0; i < fadeOver + 1; i++) {
    uint8_t fadeAmmount = (255 - (fadePP * i));
    leds[i].fadeToBlackBy(fadeAmmount);
    leds[NUM_LEDS - 1 - i].fadeToBlackBy(fadeAmmount);
  }
}


//---------------------------------------------------------------

