//***************************************************************
// Example of using memmove8 to transfer pixel data around.
//
// A pattern we want to repeat is assigned to the first four pixels.
// That color data is then repeated down the rest of the strip by
// using memmove8 to copy it to the other pixels.
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
// *NOTE*:
//   The destination start position must be correct and you
//   need to be carful about not going past the end of the strip,
//   otherwise bad things can happen in memory.  If the number
//   of pixels in your strip does not divide up evenly with the
//   number of pixels you are copying you will need to manually
//   copy the last few pixels.  Do not copy pixel data to pixels
//   that don't exist (ie greater then leds[NUM_LEDS-1]).  It will
//   cause weird memory issues.
//
// Marc Miller, April 2020
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 50
#define NUM_LEDS 32

CRGB leds[NUM_LEDS];


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
    // Set first 4 pixels with the pattern we want to repeat.
    fill_rainbow(leds, 4, millis()/20, 20);

    // Copy the pixel data down the rest of the strip.
    CopyPixels();
    
    FastLED.show();  //display leds


}//end_main_loop


//---------------------------------------------------------------
void CopyPixels() {
  // Copy pixel data from frist 4 pixels to rest of strip.

  for (uint8_t i=0; i < NUM_LEDS/4; i++) {
    memmove8( &leds[i*4], &leds[0], (4)*sizeof(CRGB) );
  }

}//end_CopyPixels


//---------------------------------------------------------------
// Reminder of the memmove8 format:
//   memmove8( &destination[start position], &source[start position], size of pixel data )
//---------------------------------------------------------------

