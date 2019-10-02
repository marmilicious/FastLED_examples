//***************************************************************
// This is Mark Kriegsman's FastLED DemoReel100 example with
// a modificaiton to use a button for changing patterns.  The
// timer used for picking a new pattern has been commented out
// and there is a button check inside the main loop now.
//
// Search for "BUTTON STUFF" to find the various button releated
// code additions.
//
// Marc Miller, March 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND  120


//---------------------------------------------------------------
//BUTTON STUFF
// This uses JChristensen's Button Library from:
//   https://github.com/JChristensen/Button
#include "Button.h"    // Include Button library
const int buttonPin = 4;  // Set digital pin used with debounced pushbutton
Button myButton(buttonPin, true, true, 50);  // Declare the button


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000); // 3 second delay for recovery
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, confetti, sinelon, juggle };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


//---------------------------------------------------------------
void loop() {
    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000 / FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) {
        gHue++;    // slowly cycle the "base color" through the rainbow
    }

    // BUTTON STUFF
    //   Not using this timer to change patterns any more.  Instead check the button.
    //     EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
    //
    readbutton();  // check for button press

}//end_main_loop

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


//---------------------------------------------------------------
void nextPattern() {
    // add one to the current pattern number, and wrap around at the end
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() {
    // FastLED's built-in rainbow generator
    fill_rainbow( leds, NUM_LEDS, gHue, 12);
}

void confetti() {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( gHue + random8(64), random8(128, 200), random8(48, 255));
}

void sinelon() {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 12);
    int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
    leds[pos] += CHSV( gHue, 255, 192);
}

void juggle() {
    // four colored dots, weaving in and out of sync with each other
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for( int i = 0; i < 4; i++) {
        leds[beatsin16( i + 5, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}


//BUTTON STUFF
//---------Function to read the button and do something----------
void readbutton() {
    myButton.read();
    if(myButton.wasPressed()) {
        Serial.println("Button pressed!  Next pattern...   ");
        nextPattern();  // Change to the next pattern

        //Flash pixel zero white as a visual that button was pressed.
        leds[0] = CHSV(0, 0, 255); //Set first pixel color white
        FastLED.show();  //Update display
        delay(100);  //Short pause so we can see leds[0] flash
        leds[0] = CRGB::Black;  //Set first pixel off
        FastLED.show();  //Update display
        delay(100);  //Short pause so we can see leds[0] flash
    }
}//end_readbutton


//---------------------------------------------------------------

