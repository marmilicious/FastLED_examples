//***************************************************************
// Example of assigning colors from a custom color array.
//
// Marc Miller, Aug 2018
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

uint8_t colorPick;

// Custom color array
CRGB colorArray[] = {
    CRGB::Red,
    CRGB::Grey,
    CRGB::Blue,
    CRGB(0, 255, 0),
    CHSV(195, 255, 255),
};



//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
    delay(2000);  // Startup delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {
    //pick a random color from the custom color array
    colorPick = random8( sizeof(colorArray) / sizeof(colorArray[0]) );

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        leds[i] = colorArray[colorPick];
        FastLED.show();
        delay(50);
        fadeToBlackBy(leds, NUM_LEDS, 220);
    }

}//end_main_loop


