//***************************************************************
// Patterns of Blinking!
// Each pixel repeats a certain color pattern.  Each pixel's
// pattern can be different in both colors and length.
// All patterns blink at the same rate.
//
// This example was inspired by Mike Thornbury's forum post:
// https://plus.google.com/101708620681849403392/posts/J1HPyNBj7ET
//
// Initially there is a bit of manual setup needed to add the
// patterns and fill some things into the arrays.  There is
// probably fancier (less manual) ways to do this, but this
// works for now!
//
// Marc Miller, April 2018
//***************************************************************


#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint16_t patternDelay = 1000;  //time to display each color [milliseconds]


// Custom colors can be added like this:
long colorA = 0x00FF7F;  //using HEX
long black =  0x000000;

CRGB temp = CRGB(200, 64, 200); //using RGB
long colorB = ((long)temp.r << 16L) | ((long)temp.g << 8L) | (long)temp.b;

CRGB temp2 = CHSV(140, 160, 255); //using HSV
long colorC = ((long)temp2.r << 16L) | ((long)temp2.g << 8L) | (long)temp2.b;


// *** ADD patterns here, specify color order (includeing black). ***
// Colors can be listed multiple times for a longer display (for example,
// as green in pattern p2).  Use black to turn a pixel off for a count.
long p0[] = {colorA, colorB};
long p1[] = {CRGB::Red, colorC, black};
long p2[] = {CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Yellow};
long p3[] = {CRGB::Purple, CRGB::Red, CRGB::Purple, black, CRGB::Blue};


// *** ADD each p# pattern above to this array. ***
long *patternList[] = { p0, p1, p2, p3 };


// *** ADD additional lines here for each pattern above. ***
// (This calculates the number of colors in each pattern.)
const uint8_t psize0 = ARRAY_SIZE(p0);
const uint8_t psize1 = ARRAY_SIZE(p1);
const uint8_t psize2 = ARRAY_SIZE(p2);
const uint8_t psize3 = ARRAY_SIZE(p3);


// *** ADD each psize# above to this array. ***
const uint8_t patternSize[] = {psize0, psize1, psize2, psize3};


const uint8_t nPatterns = ARRAY_SIZE(patternList); //calculate the number of patterns
uint8_t patternCount[nPatterns];  //keeps track of each pattern's current position


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(1500); // startup delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();

    Serial.print("Number of patterns: ");
    Serial.println(nPatterns);
    Serial.println("Number of colors per pattern: ");
    for (uint8_t i = 0; i < nPatterns; i++) {
        Serial.print("  psize");
        Serial.print(i);
        Serial.print(" = ");
        Serial.println(patternSize[i]);
    }
    Serial.println("\nSetup done. \n");
}


//---------------------------------------------------------------
void loop() {

    EVERY_N_MILLISECONDS(patternDelay) {

        for (uint8_t p = 0; p < nPatterns; p++) {
            // Can use this to print out info for checking the pattern counts.
            //Serial.print("p"); Serial.print(p);
            //Serial.print("    patternCount: ");
            //Serial.println(patternCount[p]);

            leds[p] = patternList[p][patternCount[p]];
            patternCount[p] = patternCount[p] + 1;
            if (patternCount[p] == patternSize[p]) {
                patternCount[p] = 0;  //reset count for this pattern
            }
        }

        FastLED.show();  //this can be moved outside the EVERY_N block as needed

    }//end_EVERY_N

}//end_main_loop


