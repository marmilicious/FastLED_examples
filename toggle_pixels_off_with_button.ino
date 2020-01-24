//***************************************************************
// Example to toggles some pixels on or off using a button.
// The "off" pixels are still powered and reciving data, but
// are simply set to display black when "off".
//
// In this example pressing the button toggles pixels 8-15 and 24-31.
//
// This is how the button is wired for this example:
// https://i.imgur.com/qH6cYRp.jpg
//
// Instead of a momentary button a switch could be used and
// the controller could directly read the input pin as high
// or low.  The button library would not be needed in this case.
//
// Marc Miller, Oct 2017
//   Updated Jan 2020 - for JC_button library updates
//***************************************************************


//---------------------------------------------------------------
// This sketch uses JChristensen's Button Library from:
//   https://github.com/JChristensen/JC_Button
//
#include "JC_Button.h"    // Include Button library

// Set the digital pin for the pushbutton, and declare the button
const uint8_t ButtonPin = 4;
Button OnOff_Button(ButtonPin, true, true, 100);


//---------------------------------------------------------------
#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 128

boolean OnOff_Status = 1;  // Status of extra pixels.  [1=On, 0=Off]


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // short delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  OnOff_Button.begin();  // initialize the button object
  Serial.println("Setup done.\n");
}


//---------------------------------------------------------------
void loop()
{
  readbutton();  // check the button to see if it's been pressed

  fill_rainbow( leds, NUM_LEDS, millis()/20 );

  
  extraLeds_OnOff();  // always run right before FastLED.show()
  FastLED.show();  // display the pixels

}//end_main_loop




//---------------------------------------------------------------
// Functions
//---------------------------------------------------------------
//Check the button and set the appropriate flag
void readbutton() {
  OnOff_Button.read();  // check the state of the button


  //**** Use this bit to toggle on/off each time the button is pressed.
  //     Comment this bit out and use the below for something different.
  if(OnOff_Button.wasPressed()) {
    OnOff_Status = !OnOff_Status;
    Serial.print("OnOff_Status: "); Serial.println(OnOff_Status);
  }


//  //**** Comment out the above bit and use this to have the button
//  //     only turn the pixels off while it is pressed (ie. held down).
//  if(OnOff_Button.isPressed()) {
//    OnOff_Status = 0;
//  } else {
//    OnOff_Status = 1;
//  }


}//end_readbutton


//---------------------------------------------------------------
// Turn some pixels on or off
void extraLeds_OnOff(){

  if (OnOff_Status ==0) {
    for (uint8_t i=8; i < 16; i++) {
      leds[i] = CRGB::Black;  // turn "off"
    }

    for (uint8_t i=24; i < 32; i++) {
      leds[i] = CRGB::Black;  // turn "off"
    }

  }

}//end_extraLeds_OnOff

