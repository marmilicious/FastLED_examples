//***************************************************************
// Marquee fun
//  Pixel position down the strip comes from this formula:
//      pos = spacing * (i-1) + spacing
//  i starts at 0 and is incremented by +1 up to NUM_LEDS/spacing.
//
// Marc Miller, Apr 2016
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

uint16_t holdTime = 250;  // Milliseconds to hold position before advancing.
uint8_t spacing = 2;      // Sets pixel spacing. (Use 2 or greater.)
int8_t delta = 1;         // Sets forward or backwards direction amount. (Can be negative.)

boolean fadingTail = 0;   // Add fading tail? [1=true, 0=falue]
uint8_t fadeRate = 170;   // How fast to fade out tail. [0-255]

uint8_t hue = 60;         // Starting color.
boolean DEBUG = 0;        // Print some info to serial monitor. [1=true, 0=falue]

int16_t pos;              // Pixel position.
int8_t advance;           // Stores the advance amount.


//---------------------------------------------------------------
void setup() {
    //Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000);  // Startup delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {

    EVERY_N_SECONDS(40) { // Change direction every N seconds.
        delta = -1 * delta;
    }

    EVERY_N_SECONDS(10) { // Demo changing the pixel spacing every N seconds.
        spacing = spacing + 1;
        if (spacing == 9) {
            spacing = 2;    // Reset spacing to 2
        }
        if (spacing > 4) {
            spacing = 8;    // Jump spacing up to 8
        }
        hue = hue + random8(30, 61); // Shift the hue to something new.
    }

    EVERY_N_MILLISECONDS(holdTime) { // Advance pixels to next position.
        if (DEBUG == 1) { // Print out some info if DEBUG is true.
            Serial.print("delta: ");
            Serial.print(delta);
            Serial.print("   spacing: ");
            Serial.print(spacing);
            Serial.print("    ");
        }
        for (uint8_t i = 0; i < (NUM_LEDS / spacing); i++) {
            pos = (spacing * (i - 1) + spacing + advance) % NUM_LEDS;
            leds[pos] = CHSV(hue, 255, 255);

            if (DEBUG == 1) { // Print out lit pixels if DEBUG is true.
                Serial.print(" ");
                Serial.print(pos);
            }
        }
        if (DEBUG == 1) {
            Serial.println(" ");
        }
        FastLED.show();

        // Fade out tail or set back to black for next loop around.
        if (fadingTail == 1) {
            fadeToBlackBy(leds, NUM_LEDS, fadeRate);
        } else {
            for (uint8_t i = 0; i < (NUM_LEDS / spacing); i++) {
                pos = (spacing * (i - 1) + spacing + advance) % NUM_LEDS;
                leds[pos] = CRGB::Black;
            }
        }

        // Advance pixel postion down strip, and rollover if needed.
        advance = (advance + delta + NUM_LEDS) % NUM_LEDS;
    }

}//end_main_loop
