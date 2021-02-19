/****************************************************************
breakbeatsaw8
Sawtooth wave with a pause at the end before starting over.

Works similarly to beat8, but has an extra value on the end
for the time to delay [milliseconds] before starting over.

Usage:  breakbeatsaw8(rate, low, high, delay)
  rate is in BPM (Beats Per Minute)
  low is the "from" value
  high is the "to" value
  delay is in milliseconds

If no delay value is added on the end then the delay will be 0.


Marc Miller, Feb 2020
****************************************************************/

#define USE_GET_MILLISECOND_TIMER 1  // needed for breakbeatsaw8
#include "FastLED.h"

#define LED_TYPE      LPD8806
#define COLOR_ORDER   GRB
#define DATA_PIN      11
#define CLOCK_PIN     13  // delete for clockless pixel types
#define NUM_LEDS      32
uint8_t BRIGHTNESS = 128;
CRGB leds[NUM_LEDS];

uint8_t pos, hue;

// used with breakbeatsaw8
uint32_t resetTime = millis();  
uint32_t get_millisecond_timer() {
  return millis() - resetTime;
}


//===============================================================
void setup() {
  Serial.begin(115200);
  delay(2000);  // safety start up delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done.\n");
  resetTime = millis();
}


//---------------------------------------------------------------
// custom breakbeatsaw8 function
int breakbeatsaw8( accum88 beats_per_minute, uint16_t lowest,
                   uint16_t highest, uint16_t end_delay = 0 )
{
  uint8_t beatsaw = beat8( beats_per_minute, 0);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8( beatsaw, rangewidth);
  uint8_t result = lowest + scaledbeat;

  static CEveryNMilliseconds holdTimer(end_delay);
  static boolean holding = 0;
  
  if (result == highest && holding == 0) {
    holdTimer.reset();  // zero timer
    holding = 1;
    //Serial.println("holding...");
  }

  if (holding == 1 ) {
    if (holdTimer) {
      holding = 0;  // reset back to False
      resetTime = millis();  // resets position
    }
    result = highest;  // only return highest while holding
  }

  return result;
}


//===============================================================
void loop() {

  // Draw a rainbow on first pixel so we can see the loop
  // is always running, even when the sawtooth is stopped.
  fill_rainbow( leds, 1, millis()/10);

  uint8_t posPast = pos;  // save past position

  pos = breakbeatsaw8(50, 1, NUM_LEDS - 1, 2000);  // (rate, low, high, delay)

  if (pos < posPast) {  // true whenever we start over
    hue = hue + random8(64,90);
  }

  if (pos % 2) {  // odd vs even pixel numbers
    leds[pos] = CHSV(hue, 170, 150);
  } else {
    leds[pos] = CHSV(hue, 255, 255);
  }
  
  FastLED.show();
  
} // end_main_loop


//---------------------------------------------------------------
