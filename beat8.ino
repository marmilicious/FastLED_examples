//***************************************************************
// beat8 example.  Moves pixel position based on beat8.
//
// Marc Miller, Jan 2016
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 50
CRGB leds[NUM_LEDS];

uint8_t gHue = 0;  // Hue used to cycle through rainbow.
uint16_t pos;  // Pixel position on LED strip.


//---------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(3000);  // 3 second delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() 
{
  EVERY_N_MILLISECONDS( 50 ) { gHue++; }  // Slowly cycle through the rainbow

  beat8_tail();  // Subroutine to move the pixel!
  
  FastLED.show();  // Display the pixels.

}//end main loop



//===============================================================
void beat8_tail()
{
  EVERY_N_MILLISECONDS( 5 ) {
    fadeToBlackBy( leds, NUM_LEDS, 4);  // Fade out pixels.
  }
  pos = beat8(8) % NUM_LEDS;  // modulo the position to be within NUM_LEDS
  leds[pos] = CHSV( gHue, 200, 255);
  //Serial.print("pos: "); Serial.println(pos);
}


//---------------------------------------------------------------
//EOF
