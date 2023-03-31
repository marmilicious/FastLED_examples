//***************************************************************
// Example of combining an "overlay" pattern with a "base" pattern.
// 
// The overlay is toggled On/Off with a push button, but this could
// be modified to triggered by anything else as needed.
//
// Line 88 can be uncommented if you only want the overlay pattern
// to run a single time affter it has been triggered.
//
// Marc Miller, Feb 2018
//   Updated Jan 2020 - for JC_button library updates
//   Updated March 2023 - use gHue instead of white and tweak fading
//***************************************************************


#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255

CRGB base[NUM_LEDS];      //base pattern that's always running
CRGB overlay[NUM_LEDS];   //this is added to base
CRGB leds[NUM_LEDS];      //what is sent to pixels and displayed

bool toggleButton = 0;    //toggled On/Off with each button press

uint8_t gHue;             //slowly rotating hue color

//---------------------------------------------------------------
// This example uses JChristensen's Button Library from:
//   https://github.com/JChristensen/JC_Button
#include "JC_Button.h"       //include button library
const uint8_t buttonPin = 4;  //set digital pin connected to momentary push button
Button myButton(buttonPin, 100, true, true);  //declare the button  (pin, dbTime, puEnable, invert)


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  myButton.begin();  // initialize the button object
  Serial.println("Setup done.\n");
}


//---------------------------------------------------------------
void loop()
{
  triggerCheck();     //check the overlay trigger (a push button in this example)

  EVERY_N_MILLISECONDS(30) { gHue++; }  //slowly cycle through the rainbow

  basePattern();      //update the base display
  overlayPattern();   //update the overlay display (as needed)
  combinePatterns();  //combine the base and overlay before displaying

  FastLED.show();     //show the pixels

}//end_main_loop


//---------------------------------------------------------------
void basePattern() {  //NOTE: we are drawing to the "base" CRGB here
  fill_rainbow( base, NUM_LEDS, gHue, 255/NUM_LEDS/2);
  fadeToBlackBy( base, NUM_LEDS, 220);  //dim the rainbow down
}//end_basePattern


//---------------------------------------------------------------
void overlayPattern() {  //NOTE: we are drawing to the "overlay" CRGB here
  if (toggleButton) {  //if True then overlay is currently "On"
    EVERY_N_MILLISECONDS(60) {
      static uint8_t pos = 0;
      overlay[pos] = overlay[NUM_LEDS-1-pos] = CHSV(gHue+128,180,255);
      pos++;
      if (pos == NUM_LEDS/2) {  //check against desired range
        pos = 0;  //reset for next round
          // Uncomment this next line to have the overlay pattern only run a
          // single time. Otherwise the button toggles the overlay addition on/off.
        //toggleButton = !toggleButton;  
      }
    }

    EVERY_N_MILLISECONDS(15) {
      fadeToBlackBy( overlay, NUM_LEDS, 30);  //creates fading tail effect
    }

  } else {  //overlay is currently "Off"
    EVERY_N_MILLISECONDS(15) {
      fadeToBlackBy( overlay, NUM_LEDS, 4);  //fadeout overlay pattern to zero
    }
  }
  
}//end_overlayPattern


//---------------------------------------------------------------
void combinePatterns() {
  for (uint8_t i=0; i<NUM_LEDS; i++) {
    leds[i] = base[i];      //copy base to leds
    leds[i] += overlay[i];  //add overlay to leds
  }
}//end_combinePatterns


//
// Instead of using += and adding the overlay above, the individual
// r,g,b channels could also be combined in this fashion to preserve
// brightness levels.
//
// New = 255 - scale8( 255 - base, 255 - overlay);
//
//
// FastLED's blend could also be used, but the ratio would need to
// be animated in some fashion.
// leds[i] = blend( base[i], overlay[i], ratio );  // [ratio is 0-255]
//


//---------------------------------------------------------------
void triggerCheck() {
  myButton.read();  //read the button
  if(myButton.wasPressed()) {
    toggleButton = !toggleButton;  //inverts current state
    Serial.print("Button pressed!  toggleButton = ");
    Serial.println(toggleButton);
  }
}//end_triggerCheck


//---------------------------------------------------------------
