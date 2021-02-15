/****************************************************************
Creates a slow twinkling effect (currently pastelish colors)

A second CRGB array is used to store some data about the pixels.
If you have lot of pixels this will use up more memory.


Marc Miller, Dec 2020
****************************************************************/

#include "FastLED.h"

#define LED_TYPE      LPD8806
#define COLOR_ORDER   GRB
#define DATA_PIN      11
#define CLOCK_PIN     13   // ignore for clockless pixel types
#define NUM_LEDS      32
uint8_t BRIGHTNESS = 180;  // master brightness [0-255]

CRGB leds[NUM_LEDS];
CRGB pData[NUM_LEDS];  // extra array to store some info about the pixels

uint8_t hue, sat, val;


//===============================================================
void setup() {
  Serial.begin(115200);
  delay(2000);  // safety start up delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);  // set initial master brightness
  FastLED.clear();  // clear all pixel data
  FastLED.show();   // update the display
  Serial.println("\nSetup done.\n");
}


//===============================================================
void loop() {

  EVERY_N_MILLISECONDS_I(timingObj, 100) {
    uint8_t n = random8(1,5);
    for (uint8_t i = 0; i < n; i++) {    
      uint8_t p = random8(NUM_LEDS);
      if (leds[p] == CRGB(0,0,0)) {
        pData[p] = CRGB(random8(), random8(0,150), 1);  // These r,g,b channels will actually store hue, sat, val
      }
    }
    timingObj.setPeriod(random16(150,600));  // random time to display next new pixel
  }

  EVERY_N_MILLISECONDS(8) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      if (pData[i]) {  // true if there is data

        if ((pData[i].b % 2 == 0) && (pData[i].b >= 2)) {  // even so decrease
          pData[i].b = pData[i].b - 2;  // darken pixel
        } else {  // odd so increase
          pData[i].b = pData[i].b + 2;  // brighten pixel
        }

        if (pData[i].b == 0) {  // if true reset some data to zero
          pData[i].r = 0;
          pData[i].g = 0;
        }

        if (pData[i].b == 255) {  // then switch to val to an even number so brightness will go down
          pData[i].b = 254;
        }

        sat = map(pData[i].b, 0, 255, pData[i].g, 170);  // desaturate some
        leds[i] = CHSV(pData[i].r, sat, pData[i].b);  // copy data to leds to be displayed
      }
    }

    FastLED.show();

} // end_main_loop


//---------------------------------------------------------------
//todo:  create version with Christmas colors
//       make hot chocolate
//
