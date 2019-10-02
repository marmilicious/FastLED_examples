//***************************************************************
// Moving simple pattern
// It will wrap around, and the direction can be reversed.
// This is done a bit different then you might expect as I'm
// filling a seperate CRGB array with the custom colors/pattern
// just once in setup().  Then coping that into the main leds
// array as needed when moving it around.  There are certanly
// fancier ways to do this, but this gets a simple pattern like
// going quickly.
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
CRGB base[NUM_LEDS];

int8_t delta = 1;  // 1 or -1.  (Negative value reverses direction.)
#define RATE 200  //how fast patern moves (smaller number is faster)

uint8_t pos = 0;  // position along strip


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
    delay(3000);  // Startup delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    //setup custom colors/pattern in the base array
    fill_solid( base, NUM_LEDS, CHSV(200, 160, 55) ); // base color
    base[0] = base[1] = base[2] = CRGB::Blue;  //custom colors
    base[3] = base[4] = base[5] = CRGB::Grey;
    base[6] = base[7] = base[8] = CRGB::Red;
} //end setup


//---------------------------------------------------------------
void loop() {
    EVERY_N_MILLISECONDS(RATE) {

        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            leds[mod8(pos + delta + i, NUM_LEDS)] = base[i];
        }
        FastLED.show();  // Display the pixels.
        pos = pos + delta;

    } //end EVERY_N

} //end MAIN LOOP

