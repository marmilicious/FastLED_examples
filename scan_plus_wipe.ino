//***************************************************************
// This example scans a dot down the strip, and at the same time
// a wipe fills up the strip and then wipes off again. No delays
// are used.
//
// Dot color and wipe and background color can each be set.
// The total time it takes for the dot to move down the entire
// strip can be specified.
// The wipe speed can be equal to or faster then the dot by
// adjusting the wipeStepTime value.
//
//
// Marc Miller, April 2022
//***************************************************************

#include <FastLED.h>

#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 100
CRGB leds[NUM_LEDS];


CRGB bgColor = CHSV(50,120,30);
CRGB scanColor = CHSV(128,240,255);
CRGB wipeColor = CHSV(95,200,90);
CRGB fgColor = wipeColor;

// Specify the total time for dot to scan down LED strip in MILLISECONDS
uint32_t scanTotalTime = 4096;

// Calculated scan step time (in milliseconds) per pixel
uint32_t scanStepTime = (scanTotalTime / NUM_LEDS);

// Make the wipe run equal to or faster then the scan
// Divide scanStepTime by 1 or more. Try values like 1,2,4,etc.
uint16_t wipeStepTime = (scanStepTime / 2 );  // [milliseconds]

uint8_t wipeStart;     // wipe start pixel position
uint8_t wipeEnd;       // wipe end pixel position
uint8_t scanPosition;  // keeps track of position


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);  // ***For strips using Clock.
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  //Serial.println("Print out scanTotalTime, scanStepTime, wipeStepTime:");
  //Serial.println(scanTotalTime); Serial.println(scanStepTime); Serial.println(wipeStepTime);
  Serial.println("\n");
}


//---------------------------------------------------------------
void loop() {

  EVERY_N_MILLISECONDS(wipeStepTime) {
    fill_solid(leds, NUM_LEDS, bgColor);

    if (wipeEnd < NUM_LEDS-1) {
      wipeEnd++;
      fgColor = wipeColor;
    }
    else if (wipeStart < NUM_LEDS-1) {
      wipeStart++;
    }
    else if (fgColor == bgColor) {
      wipeStart = 0;  // reset wipe
      wipeEnd = 0;
      fgColor = wipeColor;
    }
    else if (wipeStart == NUM_LEDS-1 && wipeEnd == NUM_LEDS-1) {
      fgColor = bgColor;
    }
    
    fill_gradient_RGB (leds, wipeStart, fgColor, wipeEnd, fgColor );
    leds[scanPosition] = scanColor;
    blur1d(leds, NUM_LEDS, 50);  // (optional)
  }

  EVERY_N_MILLISECONDS(scanStepTime) {
    scanPosition++;
    if (scanPosition == NUM_LEDS) {
      scanPosition = 0;  // reset scan back to start
    }
  }

  FastLED.show();
  
} //end of main loop
