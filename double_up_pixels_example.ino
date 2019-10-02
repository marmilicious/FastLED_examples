//***************************************************************
// double up pixels for DMX example
// Once running, open the serial monitor to see what's happening.
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

// For DMX, the R,G,B for each pixel will be assigned a channel number, thus:
uint16_t DMXchan = 3 * NUM_LEDS; // Number of DMX channels.
// Actually, the above would only need to be half of 3*NUM_LEDS since we are
// doubling pixels up for this test, but I'm not worrying about that.

uint16_t pixel = 0;  // for storing a pixel number
uint16_t chanR = 0;  // for storing the Red channel value
uint16_t chanG = 0;  // for storing the Green channel value
uint16_t chanB = 0;  // for storing the Blue channel value


//---------------------------------------------------------------
void setup() {
    delay(1000);  // Power-up delay
    Serial.begin(57600);  // Allows serial monitor output (check baud rate)
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}


//---------------------------------------------------------------
void loop() {
    Serial.println("...........................................................");
    for (pixel = 0; pixel < NUM_LEDS; pixel++) {
        if ((pixel & 0x01) == 0) { // Checks least significant bit to determine if even or odd
            // pixel number is an even
            Serial.print("  pixel ");
            Serial.print(pixel);
            Serial.print(" assigned:  ");
            for (uint8_t c = 0; c < 3; c++) {
                DMXchan = 3 * ((pixel + 2) / 2) - 2 + c;
                Serial.print("  chan=");
                Serial.print(DMXchan);
            }
            delay(50);
        } else {
            // pixel number is odd number
            Serial.print("  pixel ");
            Serial.print(pixel);
            Serial.print(" assigned:  ");
            for (uint8_t c = 0; c < 3; c++) {
                DMXchan = 3 * ((pixel + 1) / 2) - 2 + c;
                Serial.print("  chan=");
                Serial.print(DMXchan);
            }
            delay(50);
            Serial.println(" ");
        }
        Serial.println(" ");
    }
    delay(200);


    //================================================================
    // Read incoming serial DMX data.
    // Figure out pixel number based on incomming DMX channel number.
    //================================================================

    Serial.println("******[SIMULATING RECEIVING CHANNELS, EACH WITH VALUE 255]******");
    int count = 0;  // Using count to switch between R,G, and B LEDs in each pixel.

    // Starting chan at 1, and adding +1 to DMXchan since DMX protocol has channels start at 1.
    for (uint16_t chan = 1; chan < DMXchan + 1; chan++) {
        pixel = ((chan + 2) / 3 * 2) - 2; // Calculate what pixel the incoming channel belongs to.

        Serial.print("  DMXChan ");
        Serial.print(chan);
        Serial.print(" goes to pixel ");
        Serial.print(pixel);

        if (count == 0) {
            Serial.print("   Adding R value, and duplicating to pixel ");
            Serial.print(pixel + 1);
            Serial.println(" ");
            chanR = 255;
            chanG = 0;
            chanB = 0;
        }
        if (count == 1) {
            Serial.print("    Adding G value, and duplicating to pixel ");
            Serial.print(pixel + 1);
            Serial.println(" ");
            chanG = 255;
        }
        if (count == 2) {
            Serial.print("     Adding B value, and duplicating to pixel ");
            Serial.print(pixel + 1);
            Serial.println(" ");
            chanB = 255;
        }
        count++;

        leds[pixel] = CRGB(chanR, chanG, chanB);
        FastLED.show();
        delay(200);
        leds[pixel + 1] = CRGB(chanR, chanG, chanB); // Dublicate color to next pixel
        FastLED.show();
        delay(700);

        if (count == 3) {
            leds[pixel] = CRGB::Black;
            leds[pixel + 1] = CRGB::Black;
            FastLED.show();
        }
        if (count > 2) {
            count = 0;
        }

    }
    delay(500);
    Serial.println(" ");


}  // ----End main loop----
