// EVERY_N trigger timer and variable timer example
//
// Using EVERY_N with a timer check to always run "Event B" at
// a specific time after "Event A".
//
// Also, an example of using EVEN_N with a random variable time.
//
// Marc Miller,  May 2016
//---------------------------------------------------------------

#include "FastLED.h"
#define LED_TYPE APA102
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 39
#define COLOR_ORDER BGR
#define BRIGHTNESS 32
CRGB leds[NUM_LEDS];

uint16_t timerA = 3000;  // How often to run Event A [milliseconds]
uint16_t timerB = 500;  // How long after A to run Event B [milliseconds]
boolean counterTriggered = 0;  // Event triggered? [1=true, 0=false]


//---------------------------------------------------------------
void setup() { 
  Serial.begin(115200);  // Allows serial output (check baud rate)
  delay(3000);  // 3 second startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {
  
  // Setting the amount of time for "triggerTimer".
  // You can name "triggerTimer" whatever you want.
  static CEveryNMilliseconds triggerTimer(timerB);
  
  EVERY_N_MILLISECONDS(timerA){
    // do Event A stuff
    fill_solid(leds, NUM_LEDS, CHSV(random8(),255,128));
    counterTriggered = 1;  // Set to True
    triggerTimer.reset();  // Start trigger timer
  }
  
  if (counterTriggered == 1) {  // Will only be True if Event A has started
    if (triggerTimer) {  // Check if triggerTimer time reached
      // do Event B stuff
      for (uint8_t i=0; i<NUM_LEDS/2; i++){
        leds[random8(NUM_LEDS+1)] = CRGB::Red;
      }
      counterTriggered = 0;  // Set back to False
    }
  }


  EVERY_N_SECONDS_I( timingObj, 20) {
    // This initally defaults to 20 seconds, but then will change the run
    // period to a new random number of seconds from 10 and 30 seconds.
    // You can name "timingObj" whatever you want.
    timingObj.setPeriod( random8(10,31) );
    FastLED.clear();
    for (uint16_t i=0; i<NUM_LEDS*3; i++){
      leds[random8(NUM_LEDS+1)] = CRGB::Black;
      leds[random8(NUM_LEDS+1)] = CHSV(random8(), random8(140,255), random8(50,255));
      FastLED.show();
      delay(random8(20,80));
    }
    delay(500);
  }


  FastLED.show();
 
}//end main loop
