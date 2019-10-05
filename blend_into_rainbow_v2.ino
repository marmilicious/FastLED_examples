//***************************************************************
// Messing with blending moving colors into a cycling rainbow.
// This started up kind of simple and then I got a little crazy
// with blending in more stuff.  Gives a great fun visual though
// on my small matrix.
//
// Marc Miller, Oct 2019
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];
CRGB temp[NUM_LEDS];

uint8_t pos;  // stores a position for color being blended in
uint8_t posR, posG, posB;  // positions of moving R,G,B dots
uint8_t hue;  // used to cycle through the rainbow
uint8_t hue2;  // used to change the moving color
boolean moving = 1;  // switch for moving/not moving


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // allows serial monitor output (check baud rate)
  delay(2500); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop() {
  EVERY_N_MILLISECONDS(60) {
    hue++;
  }

  fill_rainbow(leds, NUM_LEDS, hue, 255/NUM_LEDS/4);  // draw part of the rainbow into the strip
  fadeToBlackBy(leds, NUM_LEDS, 128);  // fade the whole rainbow down some

  EVERY_N_MILLISECONDS(150) {
    if (moving) {
      pos++;
      if (pos == NUM_LEDS) { pos = 0; }  // reset to begining
    }
  }

  EVERY_N_MILLISECONDS(5) {
    if (!moving) {
      hue2 = hue2 - 1;
    }
  }

  CRGB blendThisIn  = CHSV(hue2, 140, 255);  // colors to blend into background
  CRGB blendThisIn2 = CHSV(hue2, 170, 255);
  CRGB blendThisIn3 = CHSV(hue2, 210, 255);
  CRGB blendThisIn4 = CHSV(hue2, 255, 255);

  static uint8_t blendAmount = 128;  // amount of blend  [range: 0-255]

  leds[pos]                   = nblend(leds[pos],                   blendThisIn4, blendAmount/3);
  leds[mod8(pos+1, NUM_LEDS)] = nblend(leds[mod8(pos+1, NUM_LEDS)], blendThisIn3, blendAmount/2);
  leds[mod8(pos+2, NUM_LEDS)] = nblend(leds[mod8(pos+2, NUM_LEDS)], blendThisIn2, blendAmount);
  leds[mod8(pos+3, NUM_LEDS)] = nblend(leds[mod8(pos+3, NUM_LEDS)], blendThisIn,  blendAmount);
  leds[mod8(pos+4, NUM_LEDS)] = nblend(leds[mod8(pos+4, NUM_LEDS)], blendThisIn2, blendAmount);
  leds[mod8(pos+5, NUM_LEDS)] = nblend(leds[mod8(pos+5, NUM_LEDS)], blendThisIn3, blendAmount/2);
  leds[mod8(pos+6, NUM_LEDS)] = nblend(leds[mod8(pos+6, NUM_LEDS)], blendThisIn4, blendAmount/3);

  posR = beatsin8(5, 0, (NUM_LEDS-1));
  posG = beatsin8(7, 0, (NUM_LEDS-1));
  posB = beatsin8(9, 0, (NUM_LEDS-1));

  CRGB tempColor = CRGB::Red;
  temp[posR] = tempColor;
  tempColor = CRGB::Green;
  temp[posG] = nblend(temp[posG], tempColor, 128);
  tempColor = CRGB::Blue;
  temp[posB] = nblend(temp[posB], tempColor, 128);

  EVERY_N_MILLISECONDS(2) {
    fadeToBlackBy(temp, NUM_LEDS, 1);  // create fading tail effect
  }

  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = nblend(leds[i], temp[i], 128);
  }

  leds[mod8(posR, NUM_LEDS)]   = nblend(leds[mod8(posR, NUM_LEDS)], CRGB::Red, 90);
  leds[mod8(posR+1, NUM_LEDS)] = nblend(leds[mod8(posR+1, NUM_LEDS)], CRGB::Red, 128);
  leds[mod8(posR+2, NUM_LEDS)] = nblend(leds[mod8(posR+2, NUM_LEDS)], CRGB::Red, 90);

  leds[mod8(posG, NUM_LEDS)] =   nblend(leds[mod8(posG, NUM_LEDS)], CRGB::Green, 90);
  leds[mod8(posG+1, NUM_LEDS)] = nblend(leds[mod8(posG+1, NUM_LEDS)], CRGB::Green, 128);
  leds[mod8(posG+2, NUM_LEDS)] = nblend(leds[mod8(posG+2, NUM_LEDS)], CRGB::Green, 90);

  leds[mod8(posB, NUM_LEDS)] =   nblend(leds[mod8(posB, NUM_LEDS)], CRGB::Blue, 90);
  leds[mod8(posB+1, NUM_LEDS)] = nblend(leds[mod8(posB+1, NUM_LEDS)], CRGB::Blue, 128);
  leds[mod8(posB+2, NUM_LEDS)] = nblend(leds[mod8(posB+2, NUM_LEDS)], CRGB::Blue, 90);

  FastLED.show();  // update the display

  EVERY_N_SECONDS(random8(4, 16)) {
    moving = !moving;  // randomly start/stop part of the animation for fun
  }

}// end_main_loop
