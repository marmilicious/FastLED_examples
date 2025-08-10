// Lights up two pixels at a time down the strip.
//
// This code is only designed for an even number of pixels. An
// addional check will be needed if NUM_LEDS is an odd number,
// or if you want to light up more then two pixels at a time to
// make sure not to write data past the end of the strip.
//
// No delays used. Uses EVERY_N_* to control timing.
//
//
// For this example a  seperate EVERY_N_* timer is used trigger
// running the pattern every 6 seconds, but this could be changed
// to be triggered by a button or something else.
//
//
// Marc Miller, Aug 2025
//---------------------------------------------------------------


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 120
CRGB leds[NUM_LEDS];

bool RUN_PATTERN = true;  //set to false to Not run on startup.
uint8_t pixel = 0;  //keeps track of what pixel to light up next
const uint8_t chaseRate = 120;  //number of milliseconds between chase updates
CRGB myColor = CRGB(200,42,42);  //the color to display


//---------------------------------------------------------------
void setup() { 
  Serial.begin(115200);
  delay(1500);  // startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop() {

  if (RUN_PATTERN == true) {
    if (pixel < NUM_LEDS - 1) {  //check that we're not going past the end of the strip
      EVERY_N_MILLISECONDS(chaseRate) {
        leds[pixel] = myColor;
        leds[pixel + 1] = myColor;
        pixel = pixel + 2;  //add 2 for next round
      }
    } else {  //run this if we've reached the end of the strip
      RUN_PATTERN = false;  //stop running the chase pattern
      pixel = 0;  //reset pixel position
      myColor.r = myColor.r + random(30,90);  //pick a new random color for next time
      myColor.g = myColor.g - random(40,80);
      myColor.b = myColor.b + random(60,120);
    }
  } else {  //not running the pattern any more so run a slow fade
    EVERY_N_MILLISECONDS(15) {
      fadeToBlackBy( leds, NUM_LEDS, 2);  //slowly fade pixels
    }
  }


  FastLED.show();


  EVERY_N_SECONDS(6) {
    //set to true to make the pattern run again.
    //this could be changed to be triggered by a button press instead
    RUN_PATTERN = true;
    FastLED.clear();  //make sure all the pixels are blacked out
  }


}  // end main loop


