// Example of using switch case to run through a series of steps.
//
// A pixel is held off for a period of time, then ramps up to
// a dim green, is briefly held at that brightness, and then
// finally the the pixel is turned up to full brightness for a
// different brief period of time (in milliseconds).  It's then
// turned off again until it's time to run through the steps
// again.
//
// The timing of two of the switch cases is determined by two
// EveryN timers.  These timers are individually reset to zero
// when the section they control is started.
//
//
// Marc Miller,  Feb 2021
//***************************************************************

#include "FastLED.h"
#define DATA_PIN 11
#define CLOCK_PIN 13
#define LED_TYPE LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS 32
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];


const uint8_t greenPixel = 2;  // pixel number for green flash
static uint8_t greenFlash = 0;  // the current switch case (start Off)
static uint8_t greenDimVal = 70;  // ramp up to this dim value 
static CEveryNMilliseconds holdTimer(350);  // time to hold dim value before flash
static CEveryNMilliseconds fullTimer(20);  // time to hold flash/full brightness
uint8_t gVal;


//===============================================================
void setup() {
  Serial.begin(115200);
  delay(1500);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done.\n");  
}

//===============================================================
void loop() {

  // fill up the strip up with some animated colors
  static uint8_t colorIndex;
  EVERY_N_MILLISECONDS(80){
    colorIndex++;
  }
  for( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette( LavaColors_p, colorIndex + (3*i), 10, LINEARBLEND);
  }



  // how often to run the green flash
  //   EVERY_N_MILLISECONDS, _SECONDS, _MINUTES, or _HOURS  
  EVERY_N_SECONDS(5){  
    greenFlash = 1;  // switch to case 1 (start green fade in)
    gVal = 0;  // reset brightness value to 0
  }



  switch (greenFlash) {
    
    case 0:
      // turn pixel Off
      leds[greenPixel] = CHSV(0,0,0);
      break;
      
    case 1:
      // fade in green
      leds[greenPixel] = CHSV(96,255,gVal);
      EVERY_N_MILLISECONDS(5) {  // increase to slow ramp up time
        gVal += 1;
        if (gVal >= greenDimVal) {
          greenFlash = 2;  // move on to hold
          holdTimer.reset();  // zero timer for next section
        }
      }
      break;
      
    case 2:
      // hold green
      leds[greenPixel] = CHSV(96,255,greenDimVal);
      if (holdTimer) {
        greenFlash = 3;  // move on to full brightness
        fullTimer.reset();  // zero timer for next section
      }
      break;
      
    case 3:
      // full bright green
      leds[greenPixel] = CHSV(96,235,255);  // slightly desaturated
      if (fullTimer) {
        greenFlash = 0;  // go back to green off
        //Serial.println("green pixel turned Off");
      }
      break;
      
    default:
      break;
      
  }  // end of switch



  FastLED.show();


}  // end main loop
