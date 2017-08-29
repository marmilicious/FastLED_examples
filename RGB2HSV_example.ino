//***************************************************************
//  Example of using rgb2hsv_approximate to convert from rgb to hsv.
//  rgb2hsv works best with fully saturated colors.
//  More info about rgb2hsv_approximate here:
//  https://github.com/FastLED/FastLED/blob/master/hsv2rgb.h#L52
//
//  Marc Miller, Aug 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

CRGB rgb;
CHSV hsv;

//------------------------------------------------------------------
void setup(){
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500);  // Power-up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  Serial.println("Setup done.");
}


//------------------------------------------------------------------
void loop(){

  EVERY_N_SECONDS(3) {
    rgb = CRGB( random8(),random8(),random8() );  //pick random r,g,b values
    hsv = rgb2hsv_approximate( rgb );  //convert rgb to hsv

    for(uint8_t i=0; i<NUM_LEDS/2; i++) {  //first 1/2 of strip
      leds[i] = CRGB(rgb);  //fill with r,g,b values
    }

    for(uint8_t i=NUM_LEDS/2; i<NUM_LEDS; i++) {  //second half of strip
      leds[i] = CHSV(hsv);  //fill with approximated h,s,v values
    }

    FastLED.show();
    
    //print out the rgb values and the converted rgb (from the approximated hsv)
    Serial.print("\tr: "); Serial.print(leds[0].r);
    Serial.print("\tg: "); Serial.print(leds[0].g);
    Serial.print("\tb: "); Serial.println(leds[0].b);  
    Serial.print("\th: "); Serial.print(leds[NUM_LEDS/2].r);
    Serial.print("\ts: "); Serial.print(leds[NUM_LEDS/2].g);
    Serial.print("\tv: "); Serial.println(leds[NUM_LEDS/2].b);  
    Serial.println(" ");

  }//end_every_n

}//----End main loop----

