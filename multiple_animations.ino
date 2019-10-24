/*********************************************************************/
// Example of running multiple animations/patterns at the same time
// on different parts of the same LED strip.  Seperate "working" or
// "temporary" arrays are used for each section.  Whenever one of
// these working arrays is updated then the data is copied to the
// "leds" array so it can be displayed.
//
// Animation A [ledsA] is running a chase effect.
// Animation B [ledsB] is cycling through the rainbow.
// Animation C [ledsC] is running a scanner/cylon type effect.
// Animation D [ledsD] is lighting up random pixels.
//
// Note that there is only one FastLED.show() call in the program.
//
//
// Marc Miller,  April 2019
/*********************************************************************/

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

//Note: numA thru numD values must add up to NUM_LEDS
#define numA 5    //pixels 0-4
#define numB 3    //pixels 5-7
#define numC 16   //pixels 8-23
#define numD 8    //pixels 24-31

CRGB leds[NUM_LEDS];  //what actually gets displayed
CRGB ledsA[numA];     //numA thru numD are "working" arrays
CRGB ledsB[numB];
CRGB ledsC[numC];
CRGB ledsD[numD];


/*********************************************************************/
void setup() {
    Serial.begin(115200);  //allow for output to serial monitor
    delay(2500);  //power up delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    Serial.println("setup done");
}//end_setup


/*********************************************************************/
void loop() {

    //--------------Animation A--------------
    static uint8_t color;
    EVERY_N_MILLISECONDS(180) {
        fadeToBlackBy( ledsA, numA, 230);
        static uint8_t i = 0;
        ledsA[i] = CHSV( color + random8(0, 10), 160, 255 );
        i++;
        if (i == numA) {
            i = 0;    //reset
        }

        //copy ledsA data to leds
        for (uint8_t i = 0; i < numA; i++) {
            leds[i] = ledsA[i];
        }
    }

    EVERY_N_SECONDS(2) {
        color = random8();
    }


    //--------------Animation B--------------
    const uint8_t delta = 255 / numB / 5;
    static uint8_t count;
    EVERY_N_MILLISECONDS(60) {
        count++;
    }
    fill_rainbow(ledsB, numB, count, -1 * delta );

    //copy ledsB data to leds
    for (uint8_t i = 0; i < numB; i++) {
        leds[i + numA] = ledsB[i];
    }


    //--------------Animation C--------------
    for (uint8_t i = 0; i < numC; i++) {
        uint8_t blue = (millis() / 30) + (i * 3);
        if (blue < 128) {
            ledsC[i] = CRGB(200, 0, 0);
        } else {
            ledsC[i] = CRGB(50, 0, blue);
        }
    }
    fadeToBlackBy( ledsC, numC, 220);
    uint8_t pos = beatsin8(20, 0, numC - 1);
    ledsC[pos] = CRGB::Green;

    //copy ledsC data to leds
    for (uint8_t i = 0; i < numC; i++) {
        leds[i + numA + numB] = ledsC[i];
    }


    //--------------Animation D--------------
    EVERY_N_MILLISECONDS(400) {
        for (uint8_t i = 0; i < 3; i++) { //lightup some random pixels
            uint8_t pick = random8(numD);
            static uint8_t hue;
            ledsD[pick] = CHSV( hue, random8(128, 200), random8(200, 255) );
            hue = hue + random8(4, 8);
        }

        //copy ledsD data to leds
        for (uint8_t i = 0; i < numD; i++) {
            leds[i + numA + numB + numC] = ledsD[i];
        }
    }

    EVERY_N_MILLISECONDS(200) {
        uint8_t pick = random8(numD);
        ledsD[pick] = CRGB::Black;

        //copy ledsD data to leds
        for (uint8_t i = 0; i < numD; i++) {
            leds[i + numA + numB + numC] = ledsD[i];
        }
    }



    //display all the updates on leds array
    FastLED.show();

}//end_main_loop

