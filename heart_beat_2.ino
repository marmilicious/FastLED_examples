//***************************************************************
// Heart beat pattern (lub-dub, lub-dub, lub-dub...)
//
// The lub part of the heart beat is displayed on one group of
// pixels, and the dub part on a second group.
// The beat speed and the delay of secondary dub part of the
// beat can be separately adjusted. 
//
// Marc Miller, June 2022
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLOCK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255

CRGBArray <NUM_LEDS> leds;
CRGBSet Lubs(leds(0,7));  // pixels for the first part of heart beat
CRGBSet Dubs(leds(8,15));  // pixels for the second part of heart beat

uint16_t beat_speed = 1100;  // smaller number is faster [ms]
uint8_t dub_offset = 180;  // how long after lub the dub starts [0-255 ms]

CRGB lubs_color = CHSV(15,230,255);
CRGB dubs_color = CHSV(0,255,90);


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output
  delay(1500); // startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done. \n");
}


//---------------------------------------------------------------
void loop() {

  // Lubs
  static boolean lubbing = 0;
  EVERY_N_MILLISECONDS_I( timingLubs, 1) {
    lubbing = !lubbing;
    if (lubbing) {
      timingLubs.setPeriod(dub_offset);  // time to hold before fading
      Lubs = lubs_color;
    } else {
      timingLubs.setPeriod(beat_speed);
    }
  }
  if (lubbing == 0) {
    EVERY_N_MILLISECONDS(10) {
      Lubs.fadeToBlackBy(25);  // fade out
    }
  }

  // Dubs
  static boolean dubbing = 0;
  EVERY_N_MILLISECONDS_I( timingDubs, dub_offset) {
    dubbing = !dubbing;
    if (dubbing) {
      timingDubs.setPeriod(dub_offset);  // time to hold before fading
      Dubs = dubs_color;
    } else {
      timingDubs.setPeriod(beat_speed);
    }
  }
  if (dubbing == 0) {
    EVERY_N_MILLISECONDS(7) {
      Dubs.fadeToBlackBy(15);  // fade out
    }
  }


  FastLED.show();
  
}

//---------------------------------------------------------------
// Hopefully the dub stays in sync with the main lub beat if
// this runs for a long time. I did not test that.
// If not a different method using a timer that is continuously
// reset based on whenever a lub occurs might need to be used.
//
