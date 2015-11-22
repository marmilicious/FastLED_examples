//***************************************************************
// repeating_pattern.ino
// Marc Miller, Nov 2015
//
// If we want a pattern to repeat every 5 pixels then the
// first pixel set to light up would be: 0,5,10,15,20...
// This can be represented by:  leds[5*(x-1) + 5]
// Let's put this into a loop with some variables!
//
// There are severial print statements so you can open up
// the serial monitor to see what's going on.
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 100
CRGB leds[NUM_LEDS];

uint8_t hue;     // Pixel color
uint8_t offset;  // To keep track of the offset in the pattern
uint16_t i;      // A pixel position on the strip

// How often does the pattern repeat?  Change as needed.
static uint16_t repeatEvery = 5;

// Therefore the number of times the pattern will repeat down the strip is:
static uint16_t numberOfRepeats = NUM_LEDS/repeatEvery;


//---------------------------------------------------------------
void setup() { 
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  delay(2500);  // Delay to make sure serial monitor is ready.
    Serial.print("NumberOfRepeats = "); Serial.print(numberOfRepeats);
    Serial.print(" within the strip of length "); Serial.print(NUM_LEDS);
    Serial.println(".");
} //END SETUP


//---------------------------------------------------------------
void loop() {
  
  for (offset = 0; offset < repeatEvery; offset++) {  // Operate on each repeating pixel set
    hue = (255/repeatEvery) * offset;  // Change the hue for each pixel set.
    for (uint16_t x = 0; x < numberOfRepeats+1; x++) {
      i = (repeatEvery*(x-1)) + repeatEvery + offset;  // The pixel number to draw
      if (i < NUM_LEDS) {  // Only draw pixel numbers within NUM_LEDS
        leds[i] = CHSV(hue,180,255);
          Serial.print("    offset: "); Serial.print(offset);
          Serial.print("      x: "); Serial.print(x);
          Serial.print("      leds[i]: "); Serial.print(i);
          Serial.print("      hue: "); Serial.println(hue);
        FastLED.show();  // Display each pixel individually in a set
        delay(100);
      }
    } //end of loop for pixel set

    Serial.println(" "); 
    // Uncomment the show() below and comment out the show() above to
    // have all the pixels in a set display at the same time.
    //FastLED.show();  // Display all pixels in a set at the same time.

    delay(800);  // Pause before lighting next pixel set
  } //end offset loop

  delay(1000);      // pause before clearing
  FastLED.clear();  // clear the strip
  FastLED.delay(1400);  // pause before starting over

} //END MAIN LOOP


//---------------------------------------------------------------
//TODO:  Allow a negative repeatEvery value to reverse the direction?
//
