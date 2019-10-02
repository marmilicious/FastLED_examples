//***************************************************************
// Basic example of using analog LEDs with FastLED.
//   Concept originally based on the FastLED AnalogOutput example:
//   https://github.com/FastLED/FastLED/tree/master/examples/AnalogOutput
//
// This has been updated to allow for adjusting "master brightness".
//
// Also pixel data is operated on as if we were using a single
// addressable RGB LED (ie. as if NUM_LEDS = 1), and then the
// R,G,B values are used to update the analog LED display.
//
// The RGB LED I was using to test is a common anode RGB LED so
// I needed to adjust for that as noted below [lines 44-49].
//
//
// Marc Miller, March 2019
//***************************************************************

#include "FastLED.h"
#define REDPIN   9  //pins connected to analog LED R,G,B
#define GREENPIN 10
#define BLUEPIN  11

CRGB leds[1];  //hard coding to 1 for analog case
uint8_t BRIGHTNESS = 255;  //For adjusting master brightness
uint8_t hue;


//---------------------------------------------------------------
void setup() {
    delay(1500);
    pinMode(REDPIN,   OUTPUT);  //setup pins for controlling analog RGB LED
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN,  OUTPUT);
    colorBars();  //display R,G,B color check during startup to confirm wiring
}


//---------------------------------------------------------------
//Function to update the analog LED display
void showAnalogRGB( const CRGB &rgb) {
    //Note: scale8 is used to adjust values based on BRIGHTNESS

    //Also note, I needed to update this to use 255 minus the rgb
    //value since the LED I am using is common anode.  If your
    //analog RGB LED is common cathode you don't need the "255-" part.
    analogWrite(REDPIN,   255 - scale8(rgb.r, BRIGHTNESS) ); //for common anode
    analogWrite(GREENPIN, 255 - scale8(rgb.g, BRIGHTNESS) );
    analogWrite(BLUEPIN,  255 - scale8(rgb.b, BRIGHTNESS) );
    //analogWrite(REDPIN,   scale8(rgb.r, BRIGHTNESS) );  //for common cathode
    //analogWrite(GREENPIN, scale8(rgb.g, BRIGHTNESS) );
    //analogWrite(BLUEPIN,  scale8(rgb.b, BRIGHTNESS) );
}


//---------------------------------------------------------------
void loop() {
    EVERY_N_MILLISECONDS(250) {  //cycle through the rainbow
        fill_rainbow( leds, 1, hue);
        hue = hue + 5;
    }

    EVERY_N_MILLISECONDS(5) {  //change the master brightness
        static uint8_t delta = 1;
        BRIGHTNESS = BRIGHTNESS - delta;
    }

    showAnalogRGB( leds[0]);  //update the LED display

}//end_main_loop


//---------------------------------------------------------------
void colorBars() {
    //This function is called once during startup to confirm the R,G,B wiring
    fill_solid(leds, 1, CRGB::Red);
    showAnalogRGB( leds[0] );
    delay(1500);
    fill_solid(leds, 1, CRGB::Green);
    showAnalogRGB( leds[0] );
    delay(1500);
    fill_solid(leds, 1, CRGB::Blue);
    showAnalogRGB( leds[0] );
    delay(1500);
    fill_solid(leds, 1, CRGB::Black);
    showAnalogRGB( leds[0] );
    delay(500);
}

