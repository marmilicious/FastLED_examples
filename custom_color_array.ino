//***************************************************************
// Example of assigning colors from a custom color array.
//
// Marc Miller, Aug 2018
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];

uint8_t colorPick;

// Custom color array
CRGB colorArray[] = {
  colorArray[0] = CRGB::Red,
  colorArray[1] = CRGB::Grey,
  colorArray[2] = CRGB::Blue,
  colorArray[3] = CRGB(0,255,0),
  colorArray[4] = CHSV(195,255,255),
};


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);  // ***For strips using Clock.
  //FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {
  //pick a random color from the custom color array
  colorPick = random8( sizeof(colorArray) / sizeof(colorArray[0]) );

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = colorArray[colorPick];
    FastLED.show();
    delay(50);
    fadeToBlackBy(leds, NUM_LEDS, 220);
  }
  
}//end_main_loop


