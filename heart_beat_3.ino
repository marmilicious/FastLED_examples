//***************************************************************
// Heart beat pattern (lub-dub, lub-dub, lub-dub...)
//   Yet another variation on a way to code this effect, and
//   provide some timing control.
//
// The lub (first) part of the heart beat is displayed on one
// group of pixels, and the dub (second) part on a second group.
// The beat speed and the delay of secondary dub beat can be
// separately adjusted. 
//
// Marc Miller, July 2022
//***************************************************************


#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255

CRGBArray <NUM_LEDS> leds;
CRGBSet lubs(leds(0,7));  // Pixels for lub (first) part of heart beat
CRGBSet dubs(leds(8,15));  // Pixels for dub (second) part of heart beat

#define LUB_TIME   785  // Time between main lubs [milliseconds]
#define DUB_DELAY  120  // Short delay for when secondary dub starts [milliseconds]
uint8_t beatHue = 0;  // Hue of heart beat (0 is red)


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output
  delay(2000); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done. \n");
}


//---------------------------------------------------------------
void loop() {

  heart_beat();

  FastLED.show();
  
}//end main loop




//---------------------------------------------------------------
// Heart beat function
void heart_beat() {

  static boolean lubRunning = 0;  // Is lub running? [1=true/0=false]
  static boolean dubRunning = 0;  // Is dub running? [1=true/0=false]
  static boolean dubTrigger = 0;  // Is dub triggered? [1=true/0=false]
  static CEveryNMilliseconds dubTimer(DUB_DELAY);  // Create timer for dub delay
  static uint8_t lubValue, dubValue;

  //---------------------------------
  // Regularly fade out the heart beat pixels
  EVERY_N_MILLISECONDS(5) {  // How often to do the fade
    lubs.fadeToBlackBy(21);  // Amount to fade [use smaller number for slower fade]
    dubs.fadeToBlackBy(18);
  }

  //---------------------------------
  // Timing of heart beat
  EVERY_N_MILLISECONDS(LUB_TIME) {
    lubRunning = 1;
    lubValue = 20;  // Starting value when ramping up [Use 1 or greater]
    dubTrigger = 1;
    dubTimer.reset();  // Reset dub timer
  }
  if (dubTrigger && dubTimer) {
    dubRunning = 1;
    dubValue = 1; // Starting value when ramping up [Use 1 or greater]
    dubTrigger = 0;  // Reset trigger
  }

  //---------------------------------
  // Assign pixel data
  if(lubRunning) {
    EVERY_N_MILLISECONDS(7) {
      lubValue = brighten8_video(lubValue);
    }
    lubs = CHSV(beatHue,255,lubValue);
    if (lubValue >= 250) {
      lubRunning = 0;  // Reset
    }
  }

  if(dubRunning) {
    EVERY_N_MILLISECONDS(7) {
      dubValue = brighten8_video(dubValue);
    }
    dubs = CHSV(beatHue,255,dubValue);
    if (dubValue >= 250) {
      dubRunning = 0;  // Reset
    }
  }

  //---------------------------------
  // Just for fun... Uncomment for rainbow heart beats!
  EVERY_N_MILLISECONDS(DUB_DELAY) {
    //beatHue = beatHue + random8(32,65);
  }

}//end heart_beat
