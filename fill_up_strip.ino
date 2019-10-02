// fill_up_strip
// Moving pixels "fill" up strip, then clear and start again.
// The "filling" effect can be visually adjusted and slowed down near end.
// Has not been tested with a large number of pixels.
//
// Marc Miller,  Oct 2015
//---------------------------------------------------------------

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

float delta;
uint8_t count = 0;
uint8_t hue = random8();         // Pick a random color.
uint8_t sat = random8(160, 255); // Pick a random saturation in range.


// Mess with these values for fill speed and slowing effect.
uint8_t fill_delay = 20;        // Increase to slow fill rate.
float delay_base = 1.17;        // Used to add a delay as strip fills up.
float delay_multiplier = 2.15;  // Used to add a delay as strip fills up.
/*  Set delays to 1.0 if you don't want any slowing.  These numbers need to
    be tuned visually based on your preference and also based on the number
    of pixels in your strip.
    These values looked good to me with a 32 pixel strip.  Small changes can
    make a large difference so try small increments.
*/


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {
    // Draw the moving pixels.
    for (int i = 0; i < (NUM_LEDS - count); i++) {
        leds[i] = CHSV(hue, sat, 255);
        FastLED.show();
        delay(fill_delay);  // Slow things down just a bit.
        leds[i] = CRGB::Black;
    }

    // Add the new filled pixels.
    leds[NUM_LEDS - 1 - count] = CHSV(hue, sat, 255);
    FastLED.show();
    count++;

    // Delay the filling effect to slow near end.
    delta = (pow(delay_base, count) * delay_multiplier);  // Delta increases as strip fills up.
    delay(delta);  // Delay can increase as strip fills up.
    // Uncomment to help visualize the increasing delay.
    //Serial.print("  count:"); Serial.print(count); Serial.print("    delta: "); Serial.println(delta);


    // Clear the strip when full.
    if (count == NUM_LEDS) {
        Serial.println("    -------- Reset! --------");
        delay(1400);             // Hold filled strip for a moment.
        FastLED.clear();         // Blank out the strip data.
        FastLED.show();
        count = 0;               // Reset count.
        hue = random8();         // Pick a new random fill color.
        sat = random8(160, 255); // Pick a random saturation in range.
        delay(700);              // Breif pause before filling again.
    }

}  //end main loop
