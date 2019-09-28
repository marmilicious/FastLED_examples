//***************************************************************
// Messing with blending a moving cycling color into a moving
// background rainbow.
//
// Marc Miller, Sept 2019
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];

uint8_t pos;  //stores a position for color being blended in
uint8_t hue, hue2;  //stores a color


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2500); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop() {
  EVERY_N_MILLISECONDS(60){
    hue++;  //used to cycle through the rainbow
  }

  fill_rainbow(leds, NUM_LEDS, hue, 255/NUM_LEDS/4);  //draw part of the rainbow into the strip
  fadeToBlackBy(leds, NUM_LEDS, 70);  //fade the whole strip down some

  EVERY_N_MILLISECONDS(150) {
    pos++;
    if (pos == NUM_LEDS) { pos = 0; }  //reset to begining
  }

  EVERY_N_MILLISECONDS(5){
    hue2 = hue2 - 1;  //used to change the moving color
  }
  CRGB blendThisIn  = CHSV(hue2, 140, 255);  //color to blend into the background
  CRGB blendThisIn2 = CHSV(hue2, 170, 255);  //color to blend into the background
  CRGB blendThisIn3 = CHSV(hue2, 210, 255);  //color to blend into the background
  CRGB blendThisIn4 = CHSV(hue2, 255, 255);  //color to blend into the background

  static uint8_t blendAmount = 240;  //amount of blend  [range: 0-255]

  leds[pos]                  = nblend(leds[pos],                  blendThisIn4, blendAmount/3);
  leds[mod8(pos+1,NUM_LEDS)] = nblend(leds[mod8(pos+1,NUM_LEDS)], blendThisIn3, blendAmount/2);
  leds[mod8(pos+2,NUM_LEDS)] = nblend(leds[mod8(pos+2,NUM_LEDS)], blendThisIn2, blendAmount);
  leds[mod8(pos+3,NUM_LEDS)] = nblend(leds[mod8(pos+3,NUM_LEDS)], blendThisIn,  blendAmount);
  leds[mod8(pos+4,NUM_LEDS)] = nblend(leds[mod8(pos+4,NUM_LEDS)], blendThisIn2, blendAmount);
  leds[mod8(pos+5,NUM_LEDS)] = nblend(leds[mod8(pos+5,NUM_LEDS)], blendThisIn3, blendAmount/2);
  leds[mod8(pos+6,NUM_LEDS)] = nblend(leds[mod8(pos+6,NUM_LEDS)], blendThisIn4, blendAmount/3);

  FastLED.show();  //update the display

  EVERY_N_SECONDS(random8(5,21)) {
    delay(3000);  //randomly stop the animation to observe the blending
  }

}//end_main_loop

