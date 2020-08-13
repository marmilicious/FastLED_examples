/**********************************************************************
This is a modified version of the FastLED "MultipleStripsInOneArray"
example, original found here:
https://github.com/FastLED/FastLED/blob/master/examples/Multiple/MultipleStripsInOneArray/MultipleStripsInOneArray.ino

Mutiple strips are wired to multiple data pins, and in this example
it has been modified to use strips of different lengths.

Marc Miller,  Aug 2020
**********************************************************************/


#include <FastLED.h>

#define NUM_STRIPS 3
#define NUM_LEDS_A 10
#define NUM_LEDS_B 20
#define NUM_LEDS_C 30
#define NUM_LEDS (NUM_LEDS_A + NUM_LEDS_B + NUM_LEDS_C)

CRGB leds[NUM_LEDS];


/*********************************************************************/
void setup() {
  FastLED.addLeds<WS2811, 10, RGB>(leds, 0, NUM_LEDS_A);
  FastLED.addLeds<WS2811, 11, RGB>(leds, NUM_LEDS_A, NUM_LEDS_B);
  FastLED.addLeds<WS2811, 12, RGB>(leds, NUM_LEDS_A + NUM_LEDS_B, NUM_LEDS_C);
}


/*********************************************************************/
void loop() {

  // Run a red dot down the strip
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    leds[i] = CRGB::Black;
    delay(60);
  }


  // Create one giant rainbow across the whole display
  fill_rainbow(leds, NUM_LEDS, millis()/20);
  FastLED.show();

  delay(5000);  // Pause in this example so we can see it for a moment
                // (Normally we do not want to use a long delays like this though)


  // Fill the whole display with colors from a palette
  static uint8_t colorIndex;
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( PartyColors_p, colorIndex, 255, LINEARBLEND);
    colorIndex += 42;
  }
  FastLED.show();

  delay(5000);  // Pause in this example so we can see it for a moment
                // (Normally we do not want to use a long delays like this though)


  //FastLED.clear();  // Clear the display before starting over


}  // end main loop

