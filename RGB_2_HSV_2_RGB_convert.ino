//***************************************************************
//  Demo of converting HSV to RGB_rainbow values.
//  And then converting RGB back to HSV to show how close it is.
//
//  Values are printed to serial monitor.
//
//  The first two pixels will show that the HSV to RGB converted
//  colors always match.  Observing the third pixel will show
//  how the conversion from RGB back to HSV is often pretty
//  close, but can be a bit off sometimes. Converting RGB to HSV
//  works best with fully saturated colors.
//
//  Marc Miller, Nov 2017
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLOCK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GBR
#define NUM_LEDS    32
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}


//---------------------------------------------------------------
void loop() {
  EVERY_N_SECONDS( 3 ) {
    //CHSV hsv( random8(), 255, 255 );  //pick random Hue
    CHSV hsv( random8(), random8(), random8() );  //pick random HSV
    CRGB rgb;
    hsv2rgb_rainbow( hsv, rgb);  //convert HSV to RGB

    //set first two pixels with HSV and RGB values to confirm they are the same.
    leds[0] = hsv;
    leds[1] = rgb;

    //print out HSV and RGB values
    Serial.print("         HSV:  ");
    Serial.print(hsv.h);
    Serial.print(", "); Serial.print(hsv.s);
    Serial.print(", "); Serial.println(hsv.v);

    Serial.print("         RGB:  ");
    Serial.print(rgb.r);
    Serial.print(", "); Serial.print(rgb.g);
    Serial.print(", "); Serial.println(rgb.b);


    //Now try converting RGB back to HSV using rgb2hsv_approximate.
    //rgb2hsv_approximate works best with fully saturated or nearly
    //saturated colors, otherwise it's not always super accurate.

    hsv = rgb2hsv_approximate( rgb );

    //set third pixels with HSV approximate values to show how close it is.
    leds[2] = hsv;

    //print out approximated HSV values
    Serial.print("HSV(approx.):  ");
    Serial.print(hsv.h);
    Serial.print(", "); Serial.print(hsv.s);
    Serial.print(", "); Serial.println(hsv.v);

    Serial.println(" ");

    FastLED.show();  //display the pixels

  }//end_EVERY_N

}//end_main_loop


