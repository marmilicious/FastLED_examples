//***************************************************************
// Quick example of lighting up blocks of pixels
//
// Marc Miller, Aug 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

uint8_t blockSize = 8;  //number of pixels per section


//------------------------------------------------------------------
void setup(){
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500);  // Power-up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(64);
}


//------------------------------------------------------------------
void loop(){

  EVERY_N_SECONDS(2) {  //change the block size every few seconds
    blockSize = random8(2,9);
  }
  
  for (uint8_t n=0; n<(NUM_LEDS/(blockSize*2))+1; n++) {
    uint8_t pos;
    for (uint8_t i=0; i<blockSize; i++) {
      pos = (n*blockSize*2) + i;
      if ( pos >= NUM_LEDS ) { break; }  //don't go past last pixel
      leds[pos] = CRGB(255,0,0);
    }
    for (uint8_t i=0; i<blockSize; i++) {
      pos = (n*blockSize*2) + i + blockSize;
      if ( pos >= NUM_LEDS ) { break; }  //don't go past last pixel
      leds[pos] = CRGB(0,255,0);
    }
  }

  FastLED.show();
  
}



