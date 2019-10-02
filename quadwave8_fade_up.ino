//***************************************************************
// test_quadwave8_fade_up pulse example
//
// Marc Miller, 2015
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

uint16_t count;
uint8_t speed = 15;  // Higher number gives slower pulse

//---------------------------------------------------------------
void setup() {
    delay(1000);  // Startup delay
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {
    uint8_t value = quadwave8(count / speed);
    //Serial.print("  count: ");  Serial.print(count);
    //Serial.print("    value: ");  Serial.println(value);
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(80, 255, value);
    }
    FastLED.show();
    count++;
    if (value >= 255) { // If value is maxed out then reset count.
        count = 0;
    }

}
