// Lights up two pixels at a time down the strip, triggered by
// a button press.
//
// This code is only designed for an even number of pixels. An
// addional check will be needed if NUM_LEDS is an odd number,
// or if you want to light up more then two pixels at a time to
// make sure not to write data past the end of the strip.
//
// No delays used. Uses EVERY_N_* to control timing.
//
//
// A button is used along with the JC_Button library to trigger
// the pattern. Get JChristensen's Button Library here:
//   https://github.com/JChristensen/JC_Button
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

#include "JC_Button.h"    // Include Button library
const uint8_t buttonPin = 4;  // Set digital pin used with debounced pushbutton
Button myButton(buttonPin, 50, true, true);  // Declare the button  (pin, dbTime, puEnable, invert)

bool RUN_PATTERN = false;  //set to false to Not run on startup.
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

  myButton.begin();  // initialize the button object

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
    }
  } else {  //not running the pattern any more so run a slow fade
    EVERY_N_MILLISECONDS(15) {
      fadeToBlackBy( leds, NUM_LEDS, 2);  //slowly fade pixels
    }
  }


  FastLED.show();

  //Read the button to see if it was pressed, and if yes set a few
  //things so we can restart the pattern.
  myButton.read();
  if(myButton.wasPressed()) {
    RUN_PATTERN = true;
    FastLED.clear();  //black out all pixels before running pattern again
    pixel = 0;  //reset pixel position
    myColor.r = myColor.r + random(30,90);  //pick a new random color for next time
    myColor.g = myColor.g - random(40,80);
    myColor.b = myColor.b + random(60,120);
  }


}  // end main loop
