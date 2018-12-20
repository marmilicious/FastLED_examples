//***************************************************************
// Example of adding a color to an existing pixel's color
// Pixels are first filled with a random color.
// Then a set color is incrementally added to every other pixel
// down the strip.
//
// Example inspired by this post:
// https://plus.google.com/105878980393966525266/posts/JByGQmH5y7E
//
// Marc Miller, Dec 2018
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define COLOR_ORDER GRB
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS      32
#define BRIGHTNESS    255
CRGB leds[NUM_LEDS];

uint8_t gHue;  //variable to store a hue


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1500);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {
  EVERY_N_SECONDS(5) {
    uint8_t r = random8();  //pick some random colors
    uint8_t g = random8();
    uint8_t b = random8();
    updatePixels(r,g,b);
    FastLED.show();    
  }

}//end_main_loop


//---------------------------------------------------------------
void updatePixels(byte red, byte green, byte blue) {
  CRGB color = CRGB(red, green, blue);
  fill_solid(leds, NUM_LEDS, color);  //fill entire strip with color

  //Comment this out if you want hue to continuously increment
  gHue = 0;  //reset hue to zero.  

  for (uint8_t i=0; i<(NUM_LEDS/2); i++) {
    CRGB hueShift = CHSV(gHue,255,255);  //color to add each time
    leds[(i*2)+1] += hueShift;  //add color to existing pixel color
    gHue = gHue + 20;  //increment hue by set amount
  }

}//end_updatePixels
