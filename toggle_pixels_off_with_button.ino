//***************************************************************
// Example to turn some pixels virtually "off" with a button.
// Pixels are still powered and reciving data, but are simply
// set to display black when "off".
//
// This example is for a 32 pixel strip.  Pressing the button
// turns "off" the last 8 pixels.  The pixel number where the
// cutoff starts is determined in the for loop on line 93.
//
// This is how the button is wired for this example:
// http://i.imgur.com/hOgQwjX.jpg
//
// Instead of a momentary button a switch could be used and
// the controller could directly read the input pin as high
// or low.  The button library would not be needed in this case.
//
// Marc Miller, Oct 2017
//***************************************************************


//---------------------------------------------------------------
// This sketch uses JChristensen's Button Library from:
//   https://github.com/JChristensen/Button
//
#include "Button.h"    // Include the Button library

// Set the digital pin for the pushbutton, and declare the button
const uint8_t ButtonPin = 4;
Button OnOff_Button(ButtonPin, true, true, 100);


//---------------------------------------------------------------
#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

boolean OnOff_Status = 1;  // Status of extra pixels.  [1=On, 0=Off]


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(1500); // short delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
}


//---------------------------------------------------------------
void loop() {
    readbutton();  // check the button to see if it's been pressed

    fill_rainbow( leds, NUM_LEDS, millis() / 20 );


    extraLeds_OnOff();  // always run right before FastLED.show()
    FastLED.show();  // display the pixels

}//end_main_loop




//---------------------------------------------------------------
// Functions

//---------------------------------------------------------------
//Check the button and set the appropriate flag
void readbutton() {
    OnOff_Button.read();  // check the state of the button

    if(OnOff_Button.wasPressed()) {
        OnOff_Status = !OnOff_Status;
        Serial.print("OnOff_Status: ");
        Serial.println(OnOff_Status);
    }

}//end_readbutton


//---------------------------------------------------------------
// Turn extra pixels on or off
void extraLeds_OnOff() {

    if (OnOff_Status == 0) {
        for (uint8_t i = 24; i < NUM_LEDS; i++) { //i=24 is the first pixel in the extras to be turned off.
            leds[i] = CRGB::Black;  // turn "off"
        }
    }

}//end_extraLeds_OnOff

