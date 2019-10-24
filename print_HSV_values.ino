//***************************************************************
//  Print out HSV values to serial monitor example...
//
//  Marc Miller, July 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];
CHSV hsv[NUM_LEDS];


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(1500); // startup delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
}


//---------------------------------------------------------------
void loop() {
    EVERY_N_SECONDS( 5 ) {

        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            hsv[i].h = random8();
            hsv[i].s = random8(200, 256);
            hsv[i].v = random8(64, 85);
            leds[i] = CHSV(hsv[i].h, hsv[i].s, hsv[i].v);

            Serial.print("HSV[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.print(hsv[i].h);
            Serial.print(" ");
            Serial.print(hsv[i].s);
            Serial.print(" ");
            Serial.println(hsv[i].v);
        }

        FastLED.show();

    }//end_EVERY_N

}//end_main_loop

