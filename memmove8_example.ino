//***************************************************************
// Example of using memmove8 to transfer pixel data around.
//
// A temp CRGB array is filled with a rainbow. That color data is
// then copied from temp to leds every two seconds, alternating
// between a straight copy, and copying seperate sections of
// temp to different pixel locations in leds.
//
// The snytax for memmove8 is:
// memmove8( &destination[start position], &source[start position], size of pixel data )
//
// The above is written as "size of pixel data" instead of "number
// of pixels" because in this case it would be the number of
// pixels x 3.  Three because our current CRGB has data for three
// color channels: RGB.
// In code we can write that as:  pixels * sizeof(CRGB).
//
// Marc Miller, April 2018
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 50
#define NUM_LEDS 32

CRGB leds[NUM_LEDS];
CRGB temp[NUM_LEDS];

boolean shuffle = true;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done.  \n");
}

//---------------------------------------------------------------
void loop() {
  // Fill temp with some color data which will be tranfered to leds.
  fill_rainbow(temp, NUM_LEDS, millis()/300, 256/NUM_LEDS);

  EVERY_N_SECONDS(2) {

    shuffle = !shuffle;  //toggle true or false
    Serial.print("shuffle = "); Serial.print(shuffle);

    if (shuffle) {
      ShufflePixels();  //copy from temp to leds with shuffling
      Serial.println("    Pixels shuffled.");
    } else {
      CopyPixels();  //straight copy of temp to leds, no shuffling
      Serial.println("    No change.");
    }

    FastLED.show();  //display leds

  }//end EVERY_N

}//end_main_loop


//---------------------------------------------------------------
void CopyPixels() {
  // Straight transfer of all temp pixel data to leds.
  // This results in leds being exactly the same as temp.
  memmove8( &leds, &temp, (NUM_LEDS)*sizeof(CRGB) );

}//end_CopyPixels


//---------------------------------------------------------------
void ShufflePixels() {
  // Move pixel data from temp (specifing start position) to
  // destination leds (specifing starting position), moving the
  // data for X number of pixels.
  //   memmove8( &destination[start position], &source[start position], size of pixel data );

  // move pixel data from temp 24-31 to leds 0-7.
  memmove8( &leds[0], &temp[NUM_LEDS/4*3], (NUM_LEDS)*sizeof(CRGB)/4 );

  // move pixel data from temp 16-23 to leds 8-15.
  memmove8( &leds[NUM_LEDS/4], &temp[NUM_LEDS/4*2], (NUM_LEDS)*sizeof(CRGB)/4 );

  // move pixel data from temp 8-15 to leds 16-23.
  memmove8( &leds[NUM_LEDS/4*2], &temp[NUM_LEDS/4], (NUM_LEDS)*sizeof(CRGB)/4 );

  // move pixel data from temp 0-7 to leds 24-31.
  memmove8( &leds[NUM_LEDS/4*3], &temp[0], (NUM_LEDS)*sizeof(CRGB)/4 );

}//end_ShufflePixels

