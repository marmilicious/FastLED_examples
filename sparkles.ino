//***************************************************************
// Sparkle effect with a few variables.
// Inspired by this reddit post:
// https://www.reddit.com/r/FastLED/comments/vidjkx/i_made_an_extremely_lofi_fireworks_code_from_an/
//
// Changing the three different variables changes the look quite
// a bit. Play around a bit to see what you like.
// 
//
// Marc Miller, June 2022
//***************************************************************


#include <FastLED.h>

#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);  // ***For strips using Clock.
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}


//---------------------------------------------------------------
void loop() {

  // sparkles(duration, amount, spread)
  // duration: how long a sparkel lasts [max 255]
  // amount: how often a new random pixel lights up [0-100]
  // spread: how spread out a sparkle becomes [0-172]
  sparkles(random8(80,200), 100, 30);  // sparkle duration, amount, and spread
  FastLED.show();
  
} //end of main loop


//---------------------------------------------------------------
void sparkles(uint8_t sparkel_duration, uint8_t sparkel_amount, uint8_t sparkel_spread) {
  static uint8_t sparkle_pixel;
  EVERY_N_MILLISECONDS_I( timingObj, 1) {
    timingObj.setPeriod(sparkel_duration);
    leds[sparkle_pixel] = CRGB::Black;
    uint8_t previous_pixel = sparkle_pixel;
    while (previous_pixel == sparkle_pixel) {  // pixel can't repeat
      sparkle_pixel = random8(NUM_LEDS);
    }
    if (random8(100) < sparkel_amount) {
      //leds[sparkle_pixel] = CRGB(random8(), random8(), random8());
      leds[sparkle_pixel] = CHSV(random8(), random8(20,200), random8(50,255));
    }
  }
  EVERY_N_MILLISECONDS(10) {
    //fadeToBlackBy(leds, NUM_LEDS, 1);  // fade out a bit over time
    blur1d(leds, NUM_LEDS, sparkel_spread);  // spreads and fades out color over time
  }
}


// For info on blur1d see, search for blur1d in:
// https://fastled.io/docs/3.1/colorutils_8h_source.html
