//***************************************************************
// Example of using memmove8 to copy/repeat pixel data down strip.
//
// Random pattern lengths are picks and then then the first n pixels
// are filled with color data.  The pattern is then copied/repeated
// down the length of the strip using memmove8.  If the pattern
// length doesn't divide into NUM_LEDS as a whole number then the
// last few pixels are copied separately.
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
//   When coping the pixel data you need to be careful about not
//   going past the end of the strip, otherwise bad things can
//   happen in memory.  If pixel data is copied to pixels that
//   don't exist (ie greater then leds[NUM_LEDS-1]) then the
//   controller can have weird memory issues/lockups/reboots.
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
uint8_t patternLength = 4;
uint8_t pRepeat = NUM_LEDS / patternLength;
uint8_t pDiff = NUM_LEDS % patternLength;


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
  
  repeatingPattern();
  FastLED.show();

}//end_main_loop


//---------------------------------------------------------------
void repeatingPattern() {
  // Every so often pick a new random pattern length
  EVERY_N_SECONDS(7) {
    FastLED.clear();
    random16_add_entropy(random16() + random());
    patternLength = random8(3,11);
    pRepeat = NUM_LEDS / patternLength;
    pDiff = NUM_LEDS % patternLength;
    Serial.print("patternLength: "); Serial.print(patternLength);
    Serial.print("\tpDiff: "); Serial.print(NUM_LEDS); Serial.print(" % "); Serial.print(patternLength);
    Serial.print(" = "); Serial.println(pDiff);
  }

  // Set first n pixels with the pattern to repeat
  fill_rainbow( leds, patternLength, millis()/100, 15 );

  // Set first pixel in pattern white to easily see where pattern restarts/repeats
  leds[0] = CHSV(0,0,200);
  
  // Copy pixel data from first n pixels to rest of strip using memmove8
  // Syntax for memmove8:
  //   memmove8( &destination[start position], &source[start position], size of pixel data )
  for (uint8_t i=0; i < pRepeat; i++) {
    memmove8( &leds[i*patternLength], &leds[0], (patternLength)*sizeof(CRGB) );
  }

  // Copy remainder pixels at end of strip if needed
  if (pDiff != 0) {
    memmove8( &leds[NUM_LEDS - pDiff], &leds[0], (pDiff)*sizeof(CRGB) );
  }

}//end_CopyPixels

//---------------------------------------------------------------
