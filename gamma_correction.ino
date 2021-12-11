//***************************************************************
// A few different ways to do gamma correction.
//
// The first 8 pixels are un-corrected.  The next sets of
// 8 pixels each use different gamma correction methods.
//
// What is gamma correction you ask? Here's the basics:
// https://learn.adafruit.com/led-tricks-gamma-correction/the-issue
//
//
// Marc Miller, Dec 2021
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 128  // Of course master brightness effects the look too
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

const uint8_t fadeRate = 3;  // larger value is slower
uint8_t sat = 240;  // try out different saturations

uint8_t brightness;  // stores animated brightness
uint8_t hue;  // stores current hue
uint8_t hueStep = 10;  // how fast to step thru rainbow
int8_t delta = 1;  // fading up or down

extern const uint8_t gamma8[];


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(3000);  // Startup delay

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup done.\n\n");
}


//---------------------------------------------------------------
void loop() {
  EVERY_N_MILLISECONDS(fadeRate) {
    brightness = brightness + delta;
    if (brightness == 255) {
      delta = -1;
    }
    if (brightness == 0) {
      delta = 1;
      hue = hue + random8(50,129);
    }


    // no gamma correction
    fill_rainbow_hsv(&leds[0], 8, hue, sat, brightness, hueStep);

  
    // use basic cheap and fast gamma correction, basically gamma 2.0
    fill_rainbow_hsv(&leds[8], 8, hue, sat, brightness, hueStep);
    for (uint8_t i = 8; i < 16; i++) {
      leds[i].r = dim8_video(leds[i].r);
      leds[i].g = dim8_video(leds[i].g);
      leds[i].b = dim8_video(leds[i].b);
    }
  

    // use a look up table to adjust the input brightness
    uint8_t lookupBrightness = pgm_read_byte(&gamma8[brightness]);
    fill_rainbow_hsv(&leds[16], 8, hue, sat, lookupBrightness, hueStep);
  

    // use FastLED's napplyGamma_video
    // see info here:  https://fastled.io/docs/3.1/colorutils_8h_source.html
    fill_rainbow_hsv(&leds[24], 8, hue, sat, brightness, hueStep);
    napplyGamma_video(&leds[24], 8, 2.5);
  
  }

  FastLED.show();

  if (brightness == 0 || brightness == 255) {
    delay(1000);  // brief pause and top and bottom
  }
  
}//end_main_loop



//---------------------------------------------------------------
// This is the gamma lookup for mapping 255 brightness levels
// The lookup table would be similar but have slightly shifted
// numbers for different gammas (gamma 2.0, 2.2, 2.5, etc.)
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


//---------------------------------------------------------------
// custom fill_rainbow function that takes h,s,v input
void fill_rainbow_hsv( struct CRGB * pFirstLED, int numToFill,
                      uint8_t initialhue, uint8_t initialsat,
                      uint8_t initialval, uint8_t deltahue )
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.sat = initialsat;
    hsv.val = initialval;
    for( int i = 0; i < numToFill; i++) {
        pFirstLED[i] = hsv;
        hsv.hue += deltahue;
    }
}


//---------------------------------------------------------------
