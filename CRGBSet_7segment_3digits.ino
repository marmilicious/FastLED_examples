//***************************************************************
//  Three digit seven segment number display example using CRGBArray and CRGBSet.
//  A number from 0-999 can be displayed.  This demo repeats counting from 0 to 225.
//
//  For this example the number segments are wired in the following layout:
//  http://imgur.com/cgabMfK
//  It also assumes that each segment has the same number of pixels.  There is
//  a variable below which allows the number of pixels per segment to be changed.
//
//  This example uses 5 Pixels Per Segment (pps).
//  5pps x 7segments x 3digits = 105 pixels total
//
//
//  Marc Miller, Feb 2019
//  (This sketch is a modified version of my 2digit version.)
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      105  //35 per digit in this example (35 x 3 digits = 105)
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

/* CRGB leds[NUM_LEDS];  <--not using this.  Using CRGBArray instead. */
CRGBArray<NUM_LEDS> leds;

#define FRAMES_PER_SECOND 120

uint8_t pps = 5;  // number of Pixels Per Segment
CHSV segON100(96, 200, 255); // color of 100s digit segments
CHSV segON10(142, 200, 255); // color of 10s digit segments
CHSV segON(42, 255, 255); // color of 1s digit segments

// Name segments (based on layout in link above) and define pixel ranges.
CRGBSet segA(  leds(pps * 0,  pps - 1 + (pps * 0)  ));
CRGBSet segB(  leds(pps * 1,  pps - 1 + (pps * 1)  ));
CRGBSet segC(  leds(pps * 2,  pps - 1 + (pps * 2)  ));
CRGBSet segD(  leds(pps * 3,  pps - 1 + (pps * 3)  ));
CRGBSet segE(  leds(pps * 4,  pps - 1 + (pps * 4)  ));
CRGBSet segF(  leds(pps * 5,  pps - 1 + (pps * 5)  ));
CRGBSet segG(  leds(pps * 6,  pps - 1 + (pps * 6)  ));

uint8_t count = 0;  // keeps track of what number to display


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000); // 3 second delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();  // Initially clear all pixels
}


//---------------------------------------------------------------
void loop() {
    EVERY_N_MILLISECONDS(250) {
        setSegments(count);  // Determine which segments are ON or OFF
        count++;  // Advance the count by one
        if (count > 225) {
            count = 0;    // Reset count back to zero
        }
    }
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}


//---------------------------------------------------------------
void setSegments(uint8_t count) {
    // Based on the current count set number segments on or off
    uint8_t c1 = 0;  // Variable to store 1s digit
    uint8_t c10 = 0;  // Variable to store 10s digit
    uint8_t c100 = 0;  // Variable to store 100s digit
    uint8_t c;
    CHSV segCOLOR(0, 0, 0);

    c1 = count % 10;
    c10 = (count / 10) % 10;
    c100 = (count / 100) % 10;

    //Serial.print("count = "); Serial.print(count);  // Print to serial monitor current count
    //Serial.print("\t  100s: "); Serial.print(c100);  // Print 100s digit
    //Serial.print("   10s: "); Serial.print(c10);  // Print 10s digit
    //Serial.print("   1s: "); Serial.println(c1);  // Print 1s digit

    // Operate on 1s digit segments first, shift them over,
    // then 10's digit, and then do the 100s digit segments.
    for (uint8_t i = 0; i < 3; i++) {
        if (i == 0) {
            c = c1;
            segCOLOR = segON;
        }
        if (i == 1) {
            c = c10;
            segCOLOR = segON10;
        }
        if (i == 2) {
            c = c100;
            segCOLOR = segON100;
        }

        segA = segB = segC = segD = segE = segF = segG = CRGB::Black;  // Initially set segments off

        if (c == 0) {
            segB = segC = segD = segE = segF = segG = segCOLOR;
        }
        if (c == 1) {
            segB = segG = segCOLOR;
        }
        if (c == 2) {
            segA = segB = segC = segE = segF = segCOLOR;
        }
        if (c == 3) {
            segA = segB = segC = segF = segG = segCOLOR;
        }
        if (c == 4) {
            segA = segB = segD = segG = segCOLOR;
        }
        if (c == 5) {
            segA = segC = segD = segF = segG = segCOLOR;
        }
        if (c == 6) {
            segA = segC = segD = segE = segF = segG = segCOLOR;
        }
        if (c == 7) {
            segB = segC = segG = segCOLOR;
        }
        if (c == 8) {
            segA = segB = segC = segD = segE = segF = segG = segCOLOR;
        }
        if (c == 9) {
            segA = segB = segC = segD = segF = segG = segCOLOR;
        }

        if (i == 0) {  // Shift segments over to 1s digit display area
            for (uint8_t p = 0; p < (7 * pps); p++) {
                leds[p + (2 * 7 * pps)] = leds[p];
            }
        }

        if (i == 1) {  // Shift segments over to 10s digit display area
            for (uint8_t p = 0; p < (7 * pps); p++) {
                leds[p + (7 * pps)] = leds[p + (7 * pps)];
            }
        }

        //----Comment this out if you want the 100s and 10s digits to display zeros instead of being off----
        if (c10 == 0 && i == 1) {  // If count is less then 10 set all 10s digit segments off
            segA = segB = segC = segD = segE = segF = segG = CRGB::Black;
        }
        if (c100 == 0 && i == 2) {  // If count is less then 100 set all 100s digit segments off
            segA = segB = segC = segD = segE = segF = segG = CRGB::Black;
        }
        //-------------------------------------------------------------------

    }
}//end setSegments

