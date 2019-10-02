//***************************************************************
//  Repeting block pattern example
//
//    There is a check to make sure we don't try to write data to
//    pixels that don't exist because the causes bad things to
//    happen.
//
//  Marc Miller, July 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

uint8_t blockSize = 5;  // number of pixels to light up in a block
uint16_t wait = 200;  // delay time

uint8_t count;  // used to keep track of what block to light up.
uint16_t loopStart = 0;
uint16_t loopEnd = blockSize;


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(1500); // startup delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();  // clear strip data
}

//---------------------------------------------------------------
void loop() {

    loopStart = count * blockSize;
    loopEnd = blockSize + count * blockSize;

    // Trying to write data to pixels that don't exit is bad.
    // Check to make sure we are still within our NUM_LEDS range
    // and clamp to NUM_LEDS if needed.
    if (loopEnd > NUM_LEDS) {
        loopEnd = NUM_LEDS;    // limit maximum to NUM_LEDS
    }

    uint8_t hue = random8();  // for fun, pick a new color for each block
    for(uint16_t i = loopStart; i < loopEnd; i++) {
        leds[i] =  CHSV( hue, 255, 255 );
    }
    FastLED.delay(wait);

    for(uint16_t i = loopStart; i < loopEnd; i++) {
        leds[i] =  CRGB::Black;
    }

    count++;  // increase count by one
    // reset count if we have come to the end of the strip
    if ((count * blockSize) >= NUM_LEDS) {
        count = 0;
    }

    //EVERY_N_SECONDS(5){
    //  blockSize = random8(2,9);  // for fun, pick a new random block size
    //}

}//end_main_loop


