//***************************************************************
// Example of blending from one color to a target color.
// Once the target color is reached a new random target color
// is picked to blend toward.
//
// leds[0] always displays the target color.
//
// Marc Miller, Feb 2017
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  100
CRGB leds[NUM_LEDS];

uint8_t blendRate = 50;  // How fast to blend.  Higher is slower.  [milliseconds]

CHSV colorStart = CHSV(96,255,255);  // starting color
CHSV colorTarget = CHSV(192,255,255);  // target color
CHSV colorCurrent = colorStart;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(3000); // 3 second delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop()
{
  EVERY_N_MILLISECONDS(blendRate){
    static uint8_t k;  // the amount to blend [0-255]
    if ( colorCurrent.h == colorTarget.h ) {  // Check if target has been reached
      colorStart = colorCurrent;
      colorTarget = CHSV(random8(),255,255);  // new target to transition toward
      k = 0;  // reset k value
      Serial.print("New colorTarget:\t\t\t"); Serial.println(colorTarget.h);
    }

    colorCurrent = blend(colorStart, colorTarget, k, SHORTEST_HUES);
    fill_solid( leds, NUM_LEDS, colorCurrent );
    leds[0] = colorTarget;  // set first pixel to always show target color
    Serial.print("colorCurrent:\t"); Serial.print(colorCurrent.h); Serial.print("\t");
    Serial.print("colorTarget:\t"); Serial.print(colorTarget.h);
    Serial.print("\tk: "); Serial.println(k);
    k++;
  }

  FastLED.show();  // update the display
}

//---------------------------------------------------------------
