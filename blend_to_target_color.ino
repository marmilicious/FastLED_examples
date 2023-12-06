//***************************************************************
// Example of blending from one color to a target color.
// Once the target color is reached a new random target color
// is picked to blend toward. 
//
// Colors are picked here in HSV format, but this could similarly
// be done with RGB.
//
// leds[0] always displays the target color.
//
// More info about using CHSV can be found here:
// https://github.com/FastLED/FastLED/wiki/Pixel-reference#setting-hsv-colors-
//
//
// Marc Miller, Feb 2017
//              Dec 2023 - HSV info, serial output updates
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  80
CRGB leds[NUM_LEDS];

uint8_t blendRate = 50;  // How fast to blend.  Higher is slower.  [milliseconds]

CHSV colorStart = CHSV(96,235,255);  // starting color in HSV format
CHSV colorTarget = CHSV(192,245,242);  // target color in HSV format
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
    static uint8_t k;
    if ( colorCurrent == colorTarget ) {  // Check if target has been reached
      // The above check compares Hue, Saturation, and Value all in one go.
      // If needed, one could also specifically compare the H, S, or V using:
      // colorCurrent.hue == colorTarget.hue
      // colorCurrent.sat == colorTarget.sat
      // colorCurrent.val == colorTarget.val

      colorStart = colorCurrent;
      colorTarget = CHSV( random8(),random8(160,255),random8(240,255) );  // new target to transition toward
      k = 0;  // reset k value

      Serial.print("\nNew colorTarget HSV: ");
      Serial.print(colorTarget.hue); Serial.print(",");
      Serial.print(colorTarget.sat); Serial.print(",");
      Serial.print(colorTarget.val); Serial.print("\n");
    }


    colorCurrent = blend(colorStart, colorTarget, k, SHORTEST_HUES);

    fill_solid( leds, NUM_LEDS, colorCurrent );

    leds[0] = colorTarget;  // set first pixel to always show target color

    Serial.print("Current HSV: ");
    Serial.print(colorCurrent.hue); Serial.print(",");
    Serial.print(colorCurrent.sat); Serial.print(",");
    Serial.print(colorCurrent.val); Serial.print("    ");
     Serial.print("Target HSV: ");
    Serial.print(colorTarget.hue); Serial.print(",");
    Serial.print(colorTarget.sat); Serial.print(",");
    Serial.print(colorTarget.val); Serial.print("    ");
    Serial.print("k: "); Serial.println(k);

    k++;  // increase k by 1
  }

  FastLED.show();  // update the display
}

//---------------------------------------------------------------
