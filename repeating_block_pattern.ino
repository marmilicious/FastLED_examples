//***************************************************************
//  Basic example that repeats colored blocks of random size down
//  the strip.  The block size is changed every several seconds.
//
//  Important to note is there is a check to make sure we don't
//  try to write data to pixels that don't exist (ie. values
//  equal or greater then NUM_LEDS).  Trying to write pixel
//  data to pixels that don't exist the causes bad things to
//  happen in memory.
//
//  Marc Miller, July 2017
//               May 2020 - replaced delays with EVERY_N
//***************************************************************


#include "FastLED.h"
#define DATA_PIN    11
#define CLOCK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  50
CRGB leds[NUM_LEDS];

uint8_t blockSize = 5;  // number of pixels to light up in a block
uint8_t count;  // used to keep track of what block to light up.
uint16_t loopStart = 0;
uint16_t loopEnd = blockSize;
uint8_t hue;
bool sizeUpdate;

//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2500); // startup delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  // clear strip data
}

//---------------------------------------------------------------
void loop() {

  loopStart = count * blockSize;
  loopEnd = blockSize + count*blockSize;

  // Trying to write data to pixels that don't exit is bad.
  // Check to make sure we are still within our NUM_LEDS range
  // and clamp to NUM_LEDS if needed.
  if (loopEnd > NUM_LEDS) { 
    loopEnd = NUM_LEDS;  // limit maximum to NUM_LEDS
  }
  
  hue = hue + random8(8,17);  // for fun, pick a new color for each block

  EVERY_N_MILLISECONDS(250) {
    for(uint16_t i = loopStart; i < loopEnd; i++) {
      leds[i] =  CHSV( hue, 255, 255 );
    }
    
    FastLED.show();

    for(uint16_t i = loopStart; i < loopEnd; i++) {
      leds[i].fadeToBlackBy(220);  // fade down
    }
    
    count++;  // increase count by one
    
    // reset count if we have come to the end of the strip 
    if ((count * blockSize) >= NUM_LEDS) {
      count = 0;
      // Only change the block size when starting over on the strip
      // and after the minimum time has passed (from timer below).
      if (sizeUpdate) {
        blockSize = random8(2,9);  // for fun, pick a new random block size
        sizeUpdate = false;
      }
    }
   
  } //end_every_n


  EVERY_N_SECONDS(5) {
    sizeUpdate = true;  // trigger size update
  }


}//end_main_loop

