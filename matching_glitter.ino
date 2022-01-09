//***************************************************************
// This example fills the strip with a dim desaturated color and
// then, similar to the "addGlitter" funtion in the DemoReel100
// example, random pixels are sparkled, but instead of brighten
// to white, it brightens the pixel's current color.
//
// See this example for the original mentioned "addGlitter" effect.
// https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino
//
//
// Marc Miller, Jan 2022
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 128
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

uint8_t hue;


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

  EVERY_N_MILLISECONDS(20) {
    hue++;  // slowly cycle around the color wheel
  }


  EVERY_N_MILLISECONDS(80) {
    fill_solid(leds, NUM_LEDS, CHSV(hue, 175, 140) );

    // A few different things to try out. 
    // Un-comment one of these at a time.

    //addGlitter(99);  // white, the same as in DemoReel100 example
    addMatchingGlitter(99);
    //addMatchingGlitter_2(99);
    //addMatchingGlitter_3(99);
    //addMatchingGlitter_4(99);
  }

    
  FastLED.show();
  
}//end_main_loop


//---------------------------------------------------------------
void addMatchingGlitter( fract8 chanceOfGlitter) 
// If you know the current hue, it's super easy to simply
// do something like this:
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CHSV(hue,255,255);
  }
}


//---------------------------------------------------------------
void addMatchingGlitter_2( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ].maximizeBrightness(255);
  }
}


//---------------------------------------------------------------
void addMatchingGlitter_3( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    uint16_t i = random16(NUM_LEDS);
    CRGB color = leds[i];
    leds[i] += color.nscale8_video(255);
    // Can apply a second or third time for even brigter effect
    //leds[i] += color.nscale8_video(255);
    //leds[i] += color.nscale8_video(255);
  }
}


//---------------------------------------------------------------
void addMatchingGlitter_4( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    uint16_t i = random16(NUM_LEDS);
    leds[i].r = brighten8_video(leds[i].r);
    leds[i].g = brighten8_video(leds[i].g);
    leds[i].b = brighten8_video(leds[i].b);
    // Can apply a second time for even brigter effect
    //leds[i].r = brighten8_video(leds[i].r);
    //leds[i].g = brighten8_video(leds[i].g);
    //leds[i].b = brighten8_video(leds[i].b);
  }
}


//---------------------------------------------------------------
void addGlitter( fract8 chanceOfGlitter) 
// Copied from DemoReel100 example for reference.
// Adds bright white glitter.
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}




//---------------------------------------------------------------
// Also perhaps useful, see this section of the wiki:
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#dimming-and-brightening-colors
