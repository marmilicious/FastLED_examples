//***************************************************************
// Example showing how fadeToBlackBy works.
// Open the serial monitor to see r,g,b values.
//
// Each time fadeToBlackBy(X) is called it will fade a pixel's
// r,g,b values by a certain percentage.  Calling it repeatedly
// will eventually fade to black (where r,g,b = 0,0,0).
//
// Note that the specified number is a *percentage* out of 255,
// so for example, using 64 would be 64/255 = 25%, and using
// 10 would be 10/255 = 4%.  0 would be no fading.
//
// It can be used to fade all leds of a CRGB array like:
//   fadeToBlackBy(leds, NUM_LEDS, 128);  // 128/255 = 50%
//
// Or can be used to fade a specific pixel.
//   leds[i].fadeToBlackBy(128);  // fade by 50%
//
//
// Marc Miller, Oct 2020
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 255

CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
void setup() { 
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2500);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);

  // In this exmaple we will start out by filling the strip with this redish color
  fill_solid(leds, NUM_LEDS, CRGB(255,128,64));

  FastLED.show();  // Update the display
  printPixelInfo(0);  // Print out the r,g,b values in the serial monitor
}


//---------------------------------------------------------------
void loop() {

  // Reduce this 250 ms to something smaller to change
  // how often the fade is called.
  EVERY_N_MILLISECONDS(250) {

    // Try different fade values here and note how it changes the look.
    //fadeToBlackBy(leds, NUM_LEDS, 10);  // 10/255 = 4%
    fadeToBlackBy(leds, NUM_LEDS, 64);  // 64/255 = 25%
    //fadeToBlackBy(leds, NUM_LEDS, 128);  // 128/255 = 50%
    //fadeToBlackBy(leds, NUM_LEDS, 200);  // 200/255 = 78%

    FastLED.show();  // Update the pixel display
    printPixelInfo(0);  // Print out the r,g,b values in the serial monitor

    
    // Check if we have gone all the way to black (off).
    // If true, then pick a new random color to fade.
    if ( leds[0] == CRGB(0,0,0) ) {
      delay(2000);
      fill_solid(leds, NUM_LEDS, CHSV(random8(), random8(128,255), 255));

      FastLED.show();
      Serial.println(" ");
      printPixelInfo(0);

    }
    
  }  // end every_N

}  // end main loop


// Try experimenting with different fade values and different values
// for EVERY_N_MILLISECONDS above to see how it effects things.
    

//---------------------------------------------------------------
// Function to print out r,g,b info for specified pixel number
void printPixelInfo(uint16_t i) {
  Serial.print("  leds["); Serial.print(i); Serial.print("] ");
  Serial.print("\tr: "); Serial.print(leds[i].r);
  Serial.print("\tg: "); Serial.print(leds[i].g);
  Serial.print("\tb: "); Serial.println(leds[i].b);
}

