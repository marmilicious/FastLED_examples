// This example shows how to reset the one of FastLED's timers.
//
// I have set this exampe up so each time a button is pressed the
// timer is reset. Open the serial monitor to follow some output.
//
// Marc Miller, March 2023
//------------------------------------------------------------------

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
// This uses JChristensen's Button Library from:
//   https://github.com/JChristensen/JC_Button
#include "JC_Button.h"    // Include Button library
const uint8_t buttonPin = 4;  // Set digital pin used with pushbutton
Button myButton(buttonPin, 200, true, true);  // Declare the button  (pin, dbTime, puEnable, invert)
//---------------------------------------------------------------


uint16_t updateTime = 0;  // Initial timer length [will be in *seconds* based on CEveryNSeconds line below]
static CEveryNSeconds myTimer(updateTime);  // you can name "myTimer" whatever you like. Just upate the name in all places used.

uint8_t hue;


//------------------------------------------------------------------
void setup(){
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2500);  // Power-up delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(128);
  FastLED.clear();
  FastLED.show();

  myButton.begin();  // initialize the button object

  Serial.println("\nSetup done.\n");
}


//------------------------------------------------------------------
void loop(){
  readbutton();  // check for button press

  // Fill strip with a new color every N seconds
  if (myTimer) {  // check if time reached
    Serial.println("Time to update the color.");
    hue = hue + random(60,128);
    fill_solid( leds, NUM_LEDS, CHSV(hue,255,128) );
    myTimer.setPeriod(10);  // set timer length
  }

  // Some feedback in the serial monitor for testing.
  EVERY_N_SECONDS(1){
    Serial.print("  Next color update in ");
    Serial.print(myTimer.getRemaining());
    Serial.println(" seconds.");
  }

  FastLED.show();

}//----End main loop----


//---------Function to read the button and do something----------
void readbutton() {
  myButton.read();
  if(myButton.wasPressed()) {
    myTimer.reset();  // reset timer
    Serial.println("*Button pressed!*  Timer reset.");
  }
}//end_readbutton



