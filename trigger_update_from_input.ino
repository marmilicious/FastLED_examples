//***************************************************************
// Example of using an input (button) to trigger an update.
// Each press (or rather release in this case) of the button
// updates the display, increasing the number of lit pixels. 
//
// After the last pixel is lit up, the next button press resets
// the strip (clears all pixels). Then it will start over again
// with the next button press.
//
// This example uses JChristensen's Button Library from:
//   https://github.com/JChristensen/JC_Button
// 
// Open serial monitor to see output progress.
//
// Marc Miller, Feb 2017
//   Updated Jan 2020 - for JC_button library updates
//   Updated May 2022 - changed myButton.wasPressed to .wasReleased to
//                      make sure we only advace one pixel per press
//***************************************************************


#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    21  // number of pixles in strip
#define BRIGHTNESS  50
CRGB leds[NUM_LEDS];


// use JChristensen's Button library 
#include "JC_Button.h"  
const uint8_t buttonPin = 4;  // digital pin used for debounced push button
Button myButton(buttonPin, 100, true, true);  // declare the button  (pin, dbTime, puEnable, invert)


uint8_t pos = NUM_LEDS;  // set initial position
boolean input = false;  // set input status


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(3000); // 3 second delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  myButton.begin();  // initialize the button object
  startupBlink();  // blink display at startup
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop() {
  checkInput();  // calls funtion to check the input

  if (input == true) {
    pos++;  // Advance to next pixel
    if (pos > NUM_LEDS) {  // rollover back to start
      pos = 0;
      Serial.print("Position reset.  ");
    }
    if (pos == NUM_LEDS) {  // reset state reached, clear leds
      FastLED.clear();
      Serial.print("Reset, pixels cleared.  ");
    }
    else {
      if (pos == 20) {
        fill_solid(leds, NUM_LEDS, CRGB::Green);
      }
      else {
        leds[pos] = CHSV(0,255,255);  // turn on new pixel
      }
    }
    Serial.print("pos = "); Serial.println(pos);
  }

  FastLED.show();  // update the display
  delay(2);

}//end_main_loop


//---------------------------------------------------------------
void checkInput(){  // function to check input
  myButton.read();
  //if(myButton.wasPressed() ) { 
  if(myButton.wasReleased() ) {
    Serial.print("Button pressed.  ");
    input = true;  // set input status to true
  } else {
    input = false;
  }
}//end_checkInput


//---------------------------------------------------------------
void startupBlink(){  // function to blink display upon startup
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show(); 
  delay(1000);
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show(); 
  delay(1000);
  FastLED.clear();
  FastLED.show(); 
}//end_startupBlink

