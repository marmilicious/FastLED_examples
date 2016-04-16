// Experiments with EVERY_N timer stuff.
// Testing:  manually creating timer, checking with if statement,
//   and using setPeriod(), getPeriod(), getRemaining(), and reset()
//
// Open serial monitor to follow some output.
//
// Marc Miller, Apr. 2016
//
//------------------------------------------------------------------
#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

uint16_t tTime = 10;  // seconds to next trigger.  Get's randomized.
uint16_t cTime = 10;  // Count after trigger for n seconds
uint16_t rTime = 3;  // Reset after n seconds

boolean timerTriggered = 0;  // Event triggered? [1=true, 0=false]
boolean counterTriggered = 0;  // Time to count?
boolean resetTriggered = 0;  // Should strip be reset?
boolean firstRun = 1;  // Will only have this run once.
uint8_t pos = 0;  // A pixel position.


//------------------------------------------------------------------
void setup(){
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2500);  // Power-up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(128);
  Serial.println("Setup done.");
}


//------------------------------------------------------------------
void loop(){
  static CEveryNSeconds triggerTimer(tTime);
  static CEveryNSeconds countTimer(cTime);
  static CEveryNSeconds resetTimer(rTime);

  if (firstRun == 1){
    EVERY_N_MILLISECONDS(300){
      fill_solid(leds, NUM_LEDS, CHSV(160,200,70));  // Blue
      FastLED.show();
    }
    EVERY_N_MILLISECONDS(600){
      FastLED.clear();
      FastLED.show();
    }
    EVERY_N_SECONDS(4){
      firstRun = 0;  // Set firstRun to false so it won't run again.
      resetTriggered = 1;
      resetTimer.reset();  // Start reset timer
    }
  }


  if (timerTriggered == 1) {
    if (triggerTimer) {  // check if time reached
      Serial.println("Event has been triggered!");
      triggerTimer.setPeriod( random8(8,17) );  // Set a new random trigger time
      tTime = triggerTimer.getPeriod();
      timerTriggered = 0;
      counterTriggered = 1;
      countTimer.reset();  // Start count timer
    } else {
      EVERY_N_MILLISECONDS(70){
        leds[pos] = CHSV(96,200,190);
        pos = (pos+1) % NUM_LEDS;
        fadeToBlackBy(leds, NUM_LEDS,80);
        leds[0] = CHSV(96,255,255);
        FastLED.show();
      }
      EVERY_N_SECONDS(1){
        Serial.print("    Next trigger in ");
        Serial.print(triggerTimer.getRemaining());
        Serial.println("...");
      }
    }
  }


  if (counterTriggered == 1) {
    EVERY_N_MILLISECONDS(250){
      FastLED.clear();
      for (uint8_t i=0; i<NUM_LEDS; i=i+2){
        leds[i] = CHSV(10,255,160);
        FastLED.show();
      }
    }
    EVERY_N_MILLISECONDS(500){
      FastLED.clear();
      for (uint8_t i=1; i<NUM_LEDS; i=i+2){
        leds[i] = CHSV(32,255,100);
        FastLED.show();
      }
    }

    if (countTimer) {  // check if time reached
      Serial.println(">>> Count reached! <<< ");
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show();
      counterTriggered = 0;
      resetTriggered = 1;
      resetTimer.reset();  // Start reset timer
    } else {
      EVERY_N_SECONDS(1){
        Serial.print("    Counting:  ");
        uint16_t count = countTimer.getElapsed();
        Serial.println(count);
      }
    }
  }


  if (resetTriggered == 1) {
    if (resetTimer) {  // check if time reached
      Serial.println("  Strip reset.  ");
      FastLED.clear();
      FastLED.show();
      resetTriggered = 0;  // Strip has been reset
      timerTriggered = 1; // Look for next event trigger
      countTimer.reset();  // Start count timer
      triggerTimer.reset();  // Start trigger timer
    }
  }


}//----End main loop----

