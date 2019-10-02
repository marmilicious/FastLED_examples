//***************************************************************
// Example of defining a CRGB color using Hex,
// and passing that to a function.
//
// Marc Miller, Feb 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

#define MGPu CRGB(0x8010ff) // Mardi Gras Purple
#define MGGr CRGB(0x2cb004) // Green
#define MGGo CRGB(0xff4a00) // Gold


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000); // 3 second delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {
    cylon(MGPu);  // Purple.  (Call cylon function with this color.)
    cylon(MGGr);  // Green
    cylon(MGGo);  // Gold
}


//---------------------------------------------------------------
void fadeall()  {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].nscale8(250);
    }
}

void cylon(CRGB streakcolor) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = streakcolor;
        FastLED.show();
        fadeall();
        delay(10);
    }
    for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
        leds[i] = streakcolor;
        FastLED.show();
        fadeall();
        delay(10);
    }
}


