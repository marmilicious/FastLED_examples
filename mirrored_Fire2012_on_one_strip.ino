//***************************************************************
//
//  ***NOTE***:
//  There is an updated version of this now which as the option
//  to go from center outward or from ends inward toward center.
//  https://github.com/marmilicious/FastLED_examples/blob/master/mirrored_Fire2012.ino
//
//
// Example of running FastLED's Fire2012 example on the first
// half of a strip, and mirroring that to the the second half.
//
// The first thing I did was find & replace NUM_LEDS with NUM_LEDS/2
// in all parts of the Fire2012 function.
//
// Then I added the "mirror2ndHalf" function that always gets run
// right before FastLED.show()
//
// Marc Miller, Feb 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND 100

bool gReverseDirection = false;


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000); // 3 second delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop() {
    // Add entropy to random number generator; we use a lot of it.
    // random16_add_entropy( random());

    Fire2012(); // run simulation frame

    mirror2ndHalf();  // copy and mirror first half of strip to second half
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
}



//---------------------------------------------------------------
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  90

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 50


//---------------------------------------------------------------
// ***** NOTE: NUM_LEDS was replaced with NUM_LEDS/2 anywhere it was found
//       below.  This makes the fire only run on the first half of the strip. *****
//---------------------------------------------------------------
void Fire2012() {
    // Array of temperature readings at each simulation cell
    static byte heat[NUM_LEDS / 2];

    // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS / 2; i++) {
        heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS / 2) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k = NUM_LEDS / 2 - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
        int y = random8(7);
        heat[y] = qadd8( heat[y], random8(160, 255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS / 2; j++) {
        CRGB color = HeatColor( heat[j]);
        int pixelnumber;
        if( gReverseDirection ) {
            pixelnumber = (NUM_LEDS / 2 - 1) - j;
        } else {
            pixelnumber = j;
        }
        leds[pixelnumber] = color;
    }
}


//---------------------------------------------------------------
void mirror2ndHalf() {
    // copy in reverse order first half of strip to second half
    for (uint8_t i = 0; i < NUM_LEDS / 2; i++) {
        leds[NUM_LEDS - 1 - i] = leds[i];
    }
}
