//***************************************************************
// This sketch shows a few ways to use fill_solid.
//
//
// fill_solid can be used like this to fill an entire strip with
// a single color:
//    fill_solid( leds, NUM_LEDS, CRGB::Red );  // color red
//    or
//    fill_solid( leds, NUM_LEDS, CRGB(255,0,0) );  // color red
//    or
//    fill_solid( leds, NUM_LEDS, CHSV(0,255,255) );  // color red
//
//
// And here are two ways to use fill_solid to fill a specific
// number of pixels starting at a specific position on the strip.
//
// This fills 10 pixels starting at leds[5]
//    fill_solid(leds+5, 10, CRGB::Blue);
//
// and this fills 4 pixels starting at leds[7]
//    fill_solid( &(leds[7]), 4, CRGB::Red );
//
// This second format could be good if you wanted to use variables
// to animate the fill start position and/or the amount to fill.
//
// When using fill_solid like above one needs to be very careful
// not to fill past the end of the CRGB leds array. In other words,
// make sure you don't fill past leds[NUM_LED - 1].  Writing RGB
// data to pixels that don't exist can cause bad things to happen
// in the controller's memory. "Bad things" could mean very random
// infrequent display errors, very visible incorrect display
// errors, and/or the controller locking up.
//
//
// Marc Miller,  March 2023
//***************************************************************

#include <FastLED.h>
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);  // ***For strips using Clock.
  //FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clearData();  // Set all pixels to black
}


//---------------------------------------------------------------
void loop() {
  /* The first way to use fill_solid to fill a range of pixels */

  // Fill 4 pixels starting at leds[0]
  fill_solid(leds, 4, CRGB(220,180,0) );

  // Fill 8 pixels starting at leds[4]
  fill_solid(leds+4, 8, CRGB::Blue);

  // Fill 4 pixels starting at leds[12]
  fill_solid( leds+12, 4, CRGB::Red);



  /* The second way to use fill_solid to fill a range of pixels */

  // Fill 8 pixels starting at leds[16]
  fill_solid( &(leds[16]), 8, CRGB::Grey );


  // Because this next fill starts several pixels beyond where the
  // previous fill ended there will be some black pixels before where
  // this fill starts.
  //
  // Fill 4 pixels starting at leds[28]  (ie. leds[28] thru leds[31])
  // Thus we are being careful Not to fill past the end of our CRGB
  // array which is leds[0] thru leds[NUM_LEDS-1]).
  fill_solid( &(leds[28]), 4, CHSV(172,255,255) );


  FastLED.show();
  delay(1);
  
}//end_main_loop

