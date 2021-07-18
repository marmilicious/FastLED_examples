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
  CRGB::Red,
  CRGB::Grey,
  CRGB::Blue,
  CRGB(0,255,0),
  CHSV(195,255,255),
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


//---------------------------------------------------------------
/*
Not used here in this code, but here are examles of how you can
assign custom color names (using RGB, HSV, web names, or hex)
which can later be used in your code.

CRGB myColorA (0,128,128);
CHSV myColorB (160,128,255);

CRGB myColorC = CHSV(42,128,255);
CRGB myColorD = CRGB::Purple;
CRGB myColorE = 0xE1A024;

#define myColorF CRGB(128,0,255)
#define myColorG 0x902C02

Then in your code you can do:
leds[i] = myColorA;

*/
