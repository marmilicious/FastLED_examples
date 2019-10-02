//***************************************************************
// A simple fade up/down example.
// By using HSV instead of RGB, the value of V can be adjusted
// to control brightness. The value of V could be driven by a
// counter (as in this this example), a trig function (sin, cos,
// etc), or even by an external device such as a potentiometer,
// or a light or distance sensor.
//
// Info on using HSV in the wiki here:
// https://github.com/FastLED/FastLED/wiki/Controlling-leds
//
//
// Alternatively, instead of varying V in HSV, the master
// brightness, FastLED.setBrightness(BRIGHTNESS), could be
// similarly varied over time.  It will of course adjust the
// brightness of all pixels and won't allow just a specific
// range to be faded up/down.
//
//
// Marc Miller, Oct 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

uint8_t hue = 0;
uint8_t sat = 255;
uint8_t val = 0;
boolean fadeDirection = 1;  // [1=fade up, 0=fade down]


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(2000);  // Startup delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {

    if (fadeDirection == 1) {  //fade up
        EVERY_N_MILLISECONDS(3) {
            fill_solid( leds, NUM_LEDS, CHSV(hue, sat, val) );
            val = val + 1;
            if (val == 255) {
                fadeDirection = !fadeDirection;  //reverse direction
                Serial.print("---fade up done---");
                Serial.print("  fadeDirection: ");
                Serial.println(fadeDirection);
            }
        }
    }

    if (fadeDirection == 0) {  //fade down
        EVERY_N_MILLISECONDS(9) {
            fill_solid( leds, NUM_LEDS, CHSV(hue, sat, val) );
            val = val - 1;
            if (val == 0) {
                fadeDirection = !fadeDirection;  //reverse direction
                Serial.print("--fade down done--");
                Serial.print("  fadeDirection: ");
                Serial.println(fadeDirection);
            }
        }
    }

    FastLED.show();

    EVERY_N_MILLISECONDS(90) { //cycle around the color wheel over time
        hue++;
    }

}//end_main_loop


