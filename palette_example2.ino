//***************************************************************
// Gradient palette that can have colors or anchor points updated on the fly.
// This is based on some code from Mike Brown.
// https://plus.google.com/u/0/102054795236178256184/posts/UWbQBcmKDdU?cfem=1
//
// Randomly picks start and end colors.  Colors in middle remain
// constant, but for fun their anchor points are randomly picked.
//
// More info on gradient palettes here:
// https://github.com/FastLED/FastLED/wiki/Gradient-color-palettes
//
// Marc Miller, May 2017
//***************************************************************

#include "FastLED.h"
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  255

CRGBPalette256 currentPalette;
CHSV hsv;
CRGB rgb;

//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);  // startup delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop () {
  EVERY_N_SECONDS(2){
    newGradientPalette();
  }
  FastLED.show();
  FastLED.delay(100);
}


//---------------------------------------------------------------
void newGradientPalette(){
  uint8_t xyz[16];  // Needs to be 4 times however many colors are being used.
                    // 3 colors = 12, 4 colors = 16, etc.

  hsv = CHSV(random8(), 255, 255);  // pick random hue
  hsv2rgb_rainbow( hsv, rgb);  // convert to R,G,B
  
  xyz[0] = 0;  // anchor of first color - must be zero
  xyz[1] = rgb.r;
  xyz[2] = rgb.g;
  xyz[3] = rgb.b;

  xyz[4] = random8(48,118);  // random anchor point for second color
  xyz[5] = 255;
  xyz[6] = 0;
  xyz[7] = 0;
  
  xyz[8] = random8(138,225);  // random anchor point for third color
  xyz[9] = 0,
  xyz[10] = 255;
  xyz[11] = 0;

  hsv = CHSV(random8(), 255, 255);  // pick random hue
  hsv2rgb_rainbow( hsv, rgb);  // convert to R,G,B

  xyz[12] = 255;  // anchor of last color - must be 255
  xyz[13] = rgb.r;
  xyz[14] = rgb.g;
  xyz[15] = rgb.b;

  currentPalette = xyz;

  for (int p = 0; p < NUM_LEDS; p++) {
    leds[p] = ColorFromPalette(currentPalette, p*255/(NUM_LEDS-1), BRIGHTNESS, NOBLEND);
  }

}

