//***************************************************************
// Sends "pulses" down a strip. Pulse rate can change.
//
// This example uses beatsin8 to vary the pulse rate, but the
// rate could be controlled by an external source such as a
// potentiometer, light meter, pressure sensor, etc.
// Or you could remove that bit if you just want a steady pulse
// at a constant rate.
//
// The travel speed variable adjusts how fast the pulse moves
// down the strip. You might need to adjust to your taste or
// based on what pulse rate(s) you use.
//
// The fade rate adjusts how long of a tail the pulse has. Set
// to 0 for no tail at all.
//
//
// This example was inspired by ideas from the "matrixEffect"
// sketch by Jeremy Williams:
// https://gist.github.com/Jerware/b82ad4768f9935c8acfccc98c9211111
//
//
// Marc Miller, Jan 2022
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 128
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

CRGB pulseColor = CHSV(128,220,230);
CRGB pulseTailColor = CHSV(150,180,100);
uint16_t pulseRate = 500;  // lower is faster [in milliseconds]
uint8_t travelSpeed = 25;  // lower is faster [range: 0-255]
uint8_t fadeRate = 160;  // lower is shorter tail [range: 0-255]


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(3000);  // Startup delay

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup done.\n\n");
}


//---------------------------------------------------------------
void loop() {

  // Get the current pulse rate
  // (pulseRate could be driven by an external source)
  uint8_t wave = beatsin8( 3, 0, 255); // slowly cycle between 0-255
  pulseRate = map(wave,0,255,120,1000);  // cycle between a pulseRate of 120 to 1000
  

  EVERY_N_MILLISECONDS(travelSpeed) {
    // move pulses down the strip
    for (int16_t i = NUM_LEDS-1; i >= 0; i--) {
      if (leds[i] == pulseColor) {
        if (i == NUM_LEDS-1) {
          leds[i] = pulseTailColor; // add a trail
        } else {
          leds[i+1] = pulseColor;
          leds[i] = pulseTailColor; // add a trail
        }
      }
    }

    // fade leds for tail effect
    for(int i = 0; i < NUM_LEDS; i++) {
      if (leds[i] != pulseColor) {
        leds[i].nscale8(fadeRate);  // only fades tail pixels
      }
    }
  }


  EVERY_N_MILLISECONDS_I(timingObj,1) {
    // time to start a new pulse
    leds[0] = pulseColor;
    timingObj.setPeriod(pulseRate);  // use the current pulseRate
  }


  FastLED.show();
  delay(1);  // ok to delete

  
}//end_main_loop


//---------------------------------------------------------------
