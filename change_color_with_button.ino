//***************************************************************
// The whole strip will show the same color.  When the button
// is pressed and held down the strip will cycle through the
// rainbow.  Release the button to stop on a new color.
//
// This is how the button is wired for this example:
// http://i.imgur.com/hOgQwjX.jpg
//
// Marc Miller, Jan 2019
//              Aug 2025 - Updated formatting for JC button library
//***************************************************************


//---------------------------------------------------------------
// This sketch uses JChristensen's Button Library from:
//   https://github.com/JChristensen/JC_Button
//
#include <JC_Button.h>    // Include the button library

// Set the digital pin for the pushbutton, and declare the button
const uint8_t ButtonPin = 4;
Button Push_Button(ButtonPin, true, true, 100);


//---------------------------------------------------------------
#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 128

uint8_t hue;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // Startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  Push_Button.begin();
}


//---------------------------------------------------------------
void loop()
{
  readbutton();  // check the button to see if it's pressed

  fill_solid( leds, NUM_LEDS, CHSV(hue,255,255) );  //set color of pixels

  FastLED.show();  // display the pixels

}//end_main_loop


//---------------------------------------------------------------
//Check the button and set the appropriate flag
void readbutton() {
  Push_Button.read();  // check the state of the button

  if(Push_Button.isPressed()) {
    // To make the color change faster when the button is
    // held down, you can either reduce the value of
    // EVERY_N_MILLISECONDS, or you can add a larger value
    // to hue so it increments faster.
    // You can also subtract a value from hue to cycle
    // through the color wheel in the oposite direction.

    EVERY_N_MILLISECONDS(20) {
      hue = hue + 1;   //can be either + or - some value
    }
  }
}//end_readbutton
