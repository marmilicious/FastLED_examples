//***************************************************************
// This version of lightning strikes was inspired by Stuart
// Taylor's code from:
// https://gist.github.com/hsiboy/851404307232801cb79e
//
// This DOES use delay so you can't run other stuff with this
// at the some time.  It could be converted to using variable
// EVERY_N timers, but I mostly just wanted to see what this
// looked like with my modifications first so didn't take the
// time to convert yet. Seems like this has good potential,
// but I have not looked around for other lightning strike
// code which might give an even better visual effect.
//
// This was only tested this on a small 32 pixel strip. I have
// no idea how it would look on really a long strip.
//
// NOTE: This potentially flashes lots of pixels at full White,
// so make sure your power supply can handle the needed Amps,
// and/or set the max brightness appropriately.
// 
// Marc Miller, Sept 2021
//***************************************************************


#include "FastLED.h"
#define DATA_PIN    11
#define CLOCK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];

#define MAX_FLASHES 8  // minimum is 4
#define FREQUENCY 6 // delay between strikes (increase for longer time)
uint8_t dimmer, percent;
uint8_t flashStart, flashEnd, flashNum;
CRGB bg_color = CHSV(175, 180, 16);


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output
  delay(1500); // startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done. \n");
}


// The first "flash" in a bolt of lightning is the "leader." The leader
// is usually duller and has a longer delay until the next flash. Subsequent
// flashes, the "strokes," are brighter and happen at shorter intervals.


//---------------------------------------------------------------
void loop() {

  uint8_t numFlashes = random8(3,MAX_FLASHES);
  for (uint8_t flashCounter = 0; flashCounter < numFlashes; flashCounter++) {
   
    if (flashCounter == 0) {
      dimmer = 5;  // the brightness of the leader is scaled down by a factor of 5
      flashStart = random8(0,NUM_LEDS/3);
      flashEnd = random8(NUM_LEDS/3, NUM_LEDS);
      flashNum = flashEnd - flashStart;
    } else if (flashCounter == 1) {
      dimmer = random8(2,4);  // return strokes are brighter than the leader
      flashStart = random8(0,NUM_LEDS/2);
      flashEnd = random8(NUM_LEDS - (NUM_LEDS/3), NUM_LEDS);
      flashNum = flashEnd - flashStart;
    } else {
      dimmer = random8(1,3);  // return strokes are brighter yet
      flashStart = random8(0,NUM_LEDS/3);
      flashEnd = random8(NUM_LEDS - (NUM_LEDS/4), NUM_LEDS);
      flashNum = flashEnd - flashStart;
    }

    fill_solid(leds, NUM_LEDS, bg_color);  // solid bg color
    fill_solid(leds+flashStart, flashNum - 1, CHSV(0,0,255/dimmer));
    FastLED.show();
    delay(random8(4,10));  // each flash only lasts 4-10 milliseconds
    FastLED.showColor(bg_color);  // "clear" flash before continuing

    // break up the last two flashs a bit
    if (flashCounter == numFlashes - 2) {
      for (uint8_t i = 0; i < NUM_LEDS; i++) {
        percent = random8(100);
        if (percent < 45) {
          leds[i] = bg_color;
        }
      }
    }
    if (flashCounter == numFlashes - 1) {
      for (uint8_t i = 0; i < NUM_LEDS; i++) {
        percent = random8(100);
        if (percent < 25) {
          leds[i] = bg_color;
        }
      }
    }
    FastLED.show();

    if (flashCounter == 0) { delay(random8(70,120)); }  // longer delay until next flash after the leader
    delay(40+random8(80));  // shorter delay between strikes  
  }

  FastLED.showColor(bg_color);  // "clear" last flash

  delay(random16(1000 + (FREQUENCY * 1000)));  // delay between strikes
  
}
