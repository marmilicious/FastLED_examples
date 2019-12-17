//***************************************************************
// Basic palette example using one of FastLED's palettes.
//
// FastLED includes these pre-defined palettes:
//   RainbowColors_p, RainbowStripeColors_p, OceanColors_p,
//   CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
//
// Marc Miller,  March 2019
//               Dec 2019 - updated to use EVERY_N instead of delay
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 100
CRGB leds[NUM_LEDS];

uint8_t startIndex;
uint8_t colorIndex;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500);  // startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {
  EVERY_N_MILLISECONDS(10) {
    colorIndex = startIndex;
    for( int i = 0; i < NUM_LEDS; i++) {
              //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
      colorIndex = colorIndex + 10;  //how fast to advance through palette
    }
  
    FastLED.show();
    startIndex = startIndex + 1;  //how fast to move the palette down the strip
  }//end_every_n
  
}
