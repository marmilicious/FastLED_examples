//***************************************************************
// Example of using an input to trigger a pixel update.
//
// A simple button is used here, but the input could be modified
// to other types of inputs by changing the "checkInput" function
// that monitors the input.
//
// This example uses JChristensen's Button Library from:
//   https://github.com/JChristensen/JC_Button
// 
//
// Marc Miller, Feb 2017
//   Updated Jan 2020 - for JC_button library updates
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  50
CRGB leds[NUM_LEDS];

// use JChristensen's Button library 
#include "JC_Button.h"  
const uint8_t buttonPin = 4;  // digital pin used for debounced push button
Button myButton(buttonPin, true, true, 40);  // declare the button


uint8_t pos = NUM_LEDS-1;  // set initial position
boolean input = false;  // set input status


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(3000); // 3 second delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  myButton.begin();  // initialize the button object
  Serial.println("Setup done. \n");
}

//---------------------------------------------------------------
void loop() {
  checkInput();  // calls funtion to check the input

  if (input == true) {
    leds[pos] = CRGB::Black;  // turn off previously lit pixel
    pos++;  // Advance to next pixel
    if (pos == NUM_LEDS) { pos = 0; }  // rollover if needed
    leds[pos] = CHSV(random8(),255,255);  // turn on new pixel with random color
    //input = false;  // set input status back to false
  }

  FastLED.show();  // update the display
  delay(2);

}//end_main_loop


//---------------------------------------------------------------
void checkInput(){  // function to check input
  myButton.read();
  if(myButton.wasPressed() ) {
    Serial.println("Button pressed!");
    input = true;  // set input status to true
  } else {
    input = false;
  }
}//end_checkInput
