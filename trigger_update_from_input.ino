//***************************************************************
// Example of using an input to trigger a pixel update.
//
// A simple button is used here, but the input could be modified
// to other types of inputs by changing the "checkInput" function
// that monitors the input.
//
// This example uses JChristensen's Button Library from:
//   https://github.com/JChristensen/Button
//
//
// Marc Miller, Feb 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

// use JChristensen's Button library
#include "Button.h"
const int buttonPin = 4;  // digital pin used for debounced push button
Button myButton(buttonPin, true, true, 40);  // declare the button


uint8_t pos = NUM_LEDS - 1; // set initial position
boolean input = false;  // set input status


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000); // 3 second delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    Serial.println("Setup done. \n");
}

//---------------------------------------------------------------
void loop() {
    checkInput();  // calls funtion to check the input

    if (input == true) {
        leds[pos] = CRGB::Black;  // turn off previously lit pixel
        pos++;  // Advance to next pixel
        if (pos == NUM_LEDS) {
            pos = 0;    // rollover if needed
        }
        leds[pos] = CHSV(random8(), 255, 255); // turn on new pixel with random color
        //input = false;  // set input status back to false
    }

    FastLED.show();  // update the display

}//end_main_loop


//---------------------------------------------------------------
void checkInput() { // function to check input
    myButton.read();
    if(myButton.wasPressed() ) {
        Serial.println("Button pressed!");
        input = true;  // set input status to true
    } else {
        input = false;
    }
}//end_checkInput

