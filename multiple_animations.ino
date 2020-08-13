/*********************************************************************/
// Example of running multiple animations/patterns at the same time
// on different parts of the same LED strip.  Seperate "working" or
// "temporary" arrays are used for each section.  Whenever one of
// these working arrays is updated then the data is copied to the
// "leds" array so it can be displayed.
//
// Animation A [ledsA] is running a chase effect.
// Animation B [ledsB] is cycling through the rainbow.
// Animation C [ledsC] is running a scanner/cylon type effect.
// Animation D [ledsD] is lighting up random pixels.
//
// Note that there only needs to be one FastLED.show() call at the
// bottom of the main loop.
//
//
// Marc Miller,  April 2019
//               Aug 2020 - Made NUM_LEDS the sum of numA-numD
//                          Added some extra info at bottom
/*********************************************************************/

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 255

#define numA 5    //pixels 0-4
#define numB 3    //pixels 5-7
#define numC 16   //pixels 8-23
#define numD 8    //pixels 24-31

// Note: numA thru numD values will add up to NUM_LEDS, the total
// number of pixels in the whole display, which is 32 in this exmaple.

#define NUM_LEDS (numA + numB + numC + numD)


CRGB leds[NUM_LEDS];  //what actually gets displayed
CRGB ledsA[numA];     //numA thru numD are "working" arrays
CRGB ledsB[numB];
CRGB ledsC[numC];
CRGB ledsD[numD];


/*********************************************************************/
void setup() {
  Serial.begin(115200);  //allow for output to serial monitor
  delay(2500);  //power up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  FastLED.clear();
  Serial.println("setup done");
}//end_setup


/*********************************************************************/
void loop() {

  //--------------Animation A--------------
  static uint8_t color;
  EVERY_N_MILLISECONDS(180) {
    fadeToBlackBy( ledsA, numA, 230);
    static uint8_t i = 0;
    ledsA[i] = CHSV( color+random8(0,10), 160, 255 );
    i++;
    if (i == numA) { i = 0; }  //reset

    //copy ledsA data to leds
    for (uint8_t i=0; i<numA; i++) { leds[i] = ledsA[i]; }
  }

  EVERY_N_SECONDS(2) {
    color = random8();    
  }


  //--------------Animation B--------------
  const uint8_t delta = 255/numB/5;
  static uint8_t count;
  EVERY_N_MILLISECONDS(60) { count++; }
  fill_rainbow(ledsB, numB, count, -1*delta );

  //copy ledsB data to leds
  for (uint8_t i=0; i<numB; i++) { leds[i+numA] = ledsB[i]; }


  //--------------Animation C--------------
  for (uint8_t i=0; i<numC; i++) {
    uint8_t blue = (millis()/30)+(i*3);
    if (blue < 128) {
      ledsC[i] = CRGB(200, 0, 0);
    } else {
      ledsC[i] = CRGB(50, 0, blue);
    }
  }
  fadeToBlackBy( ledsC, numC, 220);
  uint8_t pos = beatsin8(20,0,numC-1);
  ledsC[pos] = CRGB::Green;

  //copy ledsC data to leds
  for (uint8_t i=0; i<numC; i++) { leds[i+numA+numB] = ledsC[i]; }

  
  //--------------Animation D--------------
  EVERY_N_MILLISECONDS(400) {
    for (uint8_t i=0; i<3; i++) {  //lightup some random pixels
      uint8_t pick = random8(numD);
      static uint8_t hue;
      ledsD[pick] = CHSV( hue, random8(128,200), random8(200,255) );
      hue = hue + random8(4,8);
    }
    
    //copy ledsD data to leds
    for (uint8_t i=0; i<numD; i++) { leds[i+numA+numB+numC] = ledsD[i]; }
  }

  EVERY_N_MILLISECONDS(200) {
    uint8_t pick = random8(numD);
    ledsD[pick] = CRGB::Black;

    //copy ledsD data to leds
    for (uint8_t i=0; i<numD; i++) { leds[i+numA+numB+numC] = ledsD[i]; }
  }



  //display all the updates on leds array
  FastLED.show();

}//end_main_loop


/*
Additonal note:
The entire led "strip" (all sections) could also be
operated on by simply doing stuff directly to the
leds array.  Such as:

void loop {
  // Create one giant rainbow across the whole display
  fill_rainbow(leds, NUM_LEDS, millis()/20);
  FastLED.show();
}


OR...

void loop {
  // Fill the whole display with colors from a palette
  EVERY_N_MILLISECONDS(50) {
    static uint8_t colorIndex;
    for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette( PartyColors_p, colorIndex, 255, LINEARBLEND);
      colorIndex += 3;
    }
    FastLED.show();
  }
}

  
*/
