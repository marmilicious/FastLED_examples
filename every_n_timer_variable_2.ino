// EVERY_N variable timer example
// Turns all pixels on for a short time then fades them out
// to black.  Holds black for a short time and then repeats
// On/Fade/Off pattern.  
//
// A new hue is displayed each time.
//
//
// Marc Miller,  June 2020
//---------------------------------------------------------------

#include "FastLED.h"
#define LED_TYPE WS2812B
#define DATA_PIN 11
#define NUM_LEDS 12
#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];

static uint8_t gHue;
static boolean P = 0;


//---------------------------------------------------------------
void setup() { 
  Serial.begin(115200);
  delay(1500);  // startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
      .setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup done. \n");
}

//---------------------------------------------------------------


void loop() {
  
  EVERY_N_MILLISECONDS_I( timingObj, 1) {
    // This initally defaults to 1 millisecond, but then will
    // imediately use the timingObj.setPeriod times below.
    // You can name "timingObj" anything you want.

    P = !P;  // toggle P between 0 and 1
    
    if (P) {
      timingObj.setPeriod(2000);  // time to display solid before fading
      fill_solid(leds, NUM_LEDS, CHSV(gHue, 255, 255));
      FastLED.show();
      Serial.print("fill_solid, hue: "); Serial.println(gHue);
    } else {
      timingObj.setPeriod(5000);
      Serial.println("Fading out now...\n");
      gHue = gHue + 42;  // set a new hue
    }

  }  // end of every_n variable timing


  if (P == 0) {
    EVERY_N_MILLISECONDS(15) {
      fadeToBlackBy(leds, NUM_LEDS, 1);  // fade out
      FastLED.show();
    }
  }


}  // end main loop

