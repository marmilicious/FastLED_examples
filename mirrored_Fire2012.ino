//***************************************************************
// Example of running FastLED's Fire2012 example mirrored on
// both halves of a strip.  
//
// gReverseDirection sets if the fire goes from center out or
// from the ends inward.
//
// The fire is first drawn into a temporary array (leds_temp) that
// is half the length of the strip.  Then the data is copied from
// leds_temp and mirrored to leds before running FastLED.show()
//
// Marc Miller, Jan 2018
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32  // Total number of pixels in strip
#define BRIGHTNESS  128
CRGB leds[NUM_LEDS];
CRGB leds_temp[NUM_LEDS/2];  // half the total number of pixels

#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;  //false = center outward, true = from ends inward


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2000); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  Fire2012_halfStrip(); // draw fire data to leds_temp

  mirror2ndHalf();  // copy and mirror data from leds_temp to leds

  FastLED.show(); // display leds
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


//---------------------------------------------------------------
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  90

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 50




//---------------------------------------------------------------
// **** NOTE these changes to the Fire2012 function:
// - function renamed to Fire2012_halfStrip
// - leds was replaced with leds_temp so the fire is drawn into
//     the temporary (leds_temp) array.
// - NUM_LEDS was replaced with NUM_LEDS/2 (half the strip length).
// - The gReverseDirection logic was removed from Fire2012 code
//     since the reversing happens in the mirror2ndHalf function.
//---------------------------------------------------------------
void Fire2012_halfStrip() {
  // Fuction only operates on half the number of pixels (NUM_LEDS/2)
  
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS/2];

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS/2; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / (NUM_LEDS/2)) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= (NUM_LEDS/2) - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160,255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS/2; j++) {
    CRGB color = HeatColor( heat[j]);
    leds_temp[j] = color;
  }

}//end_Fire2012_halfStrip


//---------------------------------------------------------------
void mirror2ndHalf() {
  //copy and mirror pixel data from leds_temp to leds array.

  if (gReverseDirection == false) {  //false is center outward
    for (uint8_t i=0; i<NUM_LEDS/2; i++) {
      leds[(NUM_LEDS/2)-1-i] = leds_temp[i];
      leds[(NUM_LEDS/2)+i] = leds_temp[i];
    }

  } else {  //true is from ends inward
    for (uint8_t i=0; i<NUM_LEDS/2; i++) {
      leds[i] = leds_temp[i];
      leds[(NUM_LEDS-1)-i] = leds_temp[i];
    }
  }
  
}//end_mirror2ndHalf


