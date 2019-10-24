//***************************************************************
// "candy cane" stripes
// Original code concept by Christopher Kirkman
// https://plus.google.com/118127046082317810519/posts/WfEcTXiroke
//
// Modified to use EVERY_N timer, added option to travel in
// reverse, and added a third color option.
//
// Marc Miller, Dec 2017
// Add randomness for number of colors and length, Dec 2018
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(1500);  // Startup delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {
    candyCane();

}//end_main_loop


//---------------------------------------------------------------
//Draw alternating bands of color, 2 or 3 colors.
//When using three colors, color1 is used between the other two colors.
void candyCane() {
    CRGB color1 = CRGB::White;  // color used between color 2 (and 3 if used)
    CRGB color2 = CRGB::Red;
    //CRGB color3 = CHSV(0,170,255);  //optional 3rd color
    CRGB color3 = CRGB(0, 255, 0); //optional 3rd color
    const uint16_t travelSpeed = 150;
    int shiftBy = 1;  //shiftBy can be positive or negative (to change direction)
    static uint8_t numColors = 3;  // Can be either 2 or 3
    static uint8_t stripeLength = 5;  //number of pixels per color
    static int offset;

    EVERY_N_SECONDS(5) {
        numColors = random8(2, 4); //picks either 2 or 3
        stripeLength = random8(3, 6); //picks random length
    }

    EVERY_N_MILLISECONDS(travelSpeed) {
        if (numColors == 2) {
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
                if ( (i + offset) % ((numColors)*stripeLength) < stripeLength ) {
                    leds[i] = color2;
                } else {
                    leds[i] = color1;
                }
            }
        }

        if (numColors == 3) {
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
                if ( (i + offset) % ((numColors + 1)*stripeLength) < stripeLength ) {
                    leds[i] = color2;
                } else if ( (i + offset + (2 * stripeLength)) % ((numColors + 1)*stripeLength) < stripeLength ) {
                    leds[i] = color3;
                } else {
                    leds[i] = color1;
                }
            }
        }

        FastLED.show();

        offset = offset + shiftBy;
        if (shiftBy > 0) { //for positive shiftBy
            if (offset >= NUM_LEDS) offset = 0;
        } else {  //for negitive shiftBy
            if (offset < 0) offset = NUM_LEDS;
        }

    }//end EVERY_N
}//end candyCane

