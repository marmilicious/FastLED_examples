//***************************************************************
//  Blynk test with Teensy 3.2 and USB connection.
//
//  Four widgets were used to send data from the Blynk app:
//  - Button (turns display on/off)
//  - Slider (controls brightness)
//  - zeRGBa (RGB data sets pixel color)
//  - Menu   (selects a pattern)
//
//  Looks like this on my phone:
//  https://i.imgur.com/6x9AvOQ.png
//
//  Marc Miller, Oct 2017
//       update: Feb 2018 - added a simple Menu example
//***************************************************************


/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk downloads, docs, tutorials, etc:
    http://www.blynk.cc
  Blynk USB HOWTO info:
    http://tiny.cc/BlynkUSB
 *************************************************************/


//---------------------------------------------------------------
#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

void solid();
void rainbow();
void juggle();
void sinelon();
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { solid, rainbow, juggle, sinelon };
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


//---------------------------------------------------------------
#include <BlynkSimpleStream.h>

// Blynk App. Auth Token goes here:
char auth[] = "put_your_token_number_here_inside_the_quotes";

// define custom variables to hold incoming data from Blynk widgets
uint8_t sliderBrightness;  //output: V1, slider
uint8_t zeRGBa[3];         //output: V2, zeRGBa data (set to "merge")
boolean buttonA;           //output: V3, button (set as "switch")
uint8_t pattern;           //output: V4, current pattern number

BLYNK_WRITE(V1) { //slider value 0-255
    sliderBrightness = param.asInt(); // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(V2) { //set as "merge" in zeRGBa widget
    zeRGBa[0] = param[0].asInt();  //red
    zeRGBa[1] = param[1].asInt();  //green
    zeRGBa[2] = param[2].asInt();  //blue
}

BLYNK_WRITE(V3) { //toggles pixel display On/Off, set as "switch"
    buttonA = param.asInt();  //value is either 0 or 1
}

BLYNK_WRITE(V4) {
    switch (param.asInt()) {
    case 1: {
        pattern = 1;  //rainbow
        break;
    }
    case 2: {
        pattern = 2;  //juggle
        break;
    }
    case 3: {
        pattern = 3;  //sinelon  (color selected by zeRGBa picker)
        break;
    }
    case 4: {
        pattern = 0;  //solid  (color selected by zeRGBa picker)
        break;
    }
    default:
        pattern = 0;  //run if no pattern selected
    }
}

//---------------------------------------------------------------
void setup() {
    // Blynk will work through Serial
    // Do not read or write this serial manually in your sketch
    Serial.begin(9600);
    Blynk.begin(Serial, auth);

    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}


//---------------------------------------------------------------
void loop() {
    Blynk.run();

    EVERY_N_MILLISECONDS(10) {
        if (buttonA == 1) {  //strip is ON.
            gPatterns[pattern]();

        } else {  //strip is OFF
            FastLED.clear();  //blank out all pixel data

        }

        FastLED.setBrightness(sliderBrightness);
        FastLED.show();

    }//end_EVERY_N

    EVERY_N_MILLISECONDS( 20 ) {
        gHue++;    // slowly cycle the "base color" through the rainbow
    }

}//end_main_loop


//---------------------------------------------------------------
// Patterns
//---------------------------------------------------------------
void rainbow() {
    // FastLED's built-in rainbow generator
    fill_rainbow( leds, NUM_LEDS, gHue, NUM_LEDS / 8);
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

void sinelon() {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 12);
    int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
    leds[pos] += CRGB(zeRGBa[0], zeRGBa[1], zeRGBa[2]);
}

void solid() {
    // fill entire strip with a solid color
    fill_solid(leds, NUM_LEDS, CRGB(zeRGBa[0], zeRGBa[1], zeRGBa[2]) );
}
