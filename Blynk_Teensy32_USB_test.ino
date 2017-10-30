//***************************************************************
//  Blynk test with Teensy 3.2 and USB connection.
//
//  Three widgets were used to send data from the Blynk app to
//  the MCU:
//  - Button (turns display on/off)
//  - Slider (controls brightness)
//  - zeRGBa (RGB data sets pixel color)
//
//  Marc Miller, Oct 2017
//***************************************************************


/***********************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Downloads, docs, tutorials: http://www.blynk.cc
  USB HOWTO: http://tiny.cc/BlynkUSB
 ***********************************************************/


//---------------------------------------------------------------
#include "FastLED.h"  //include FastLED library
#define NUM_LEDS 35   // Number of pixels in strip
CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
#include <BlynkSimpleStream.h>

// Blynk App. Auth Token goes here:
char auth[] = "put_your_token_number_here_inside_the_quotes";

// define custom variables to hold incoming data from Blynk widgets
uint8_t sliderBrightness;  //output: V1, slider
uint8_t zeRGBa[3];         //output: V2, zeRGBa data (set to "merge")
boolean buttonA;           //output: V3, button (set as "switch")


BLYNK_WRITE(V1)  //slider value 0-255
{
  sliderBrightness = param.asInt(); // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(V2)  //set as "merge" in zeRGBa widget
{
  zeRGBa[0] = param[0].asInt();  //red
  zeRGBa[1] = param[1].asInt();  //green
  zeRGBa[2] = param[2].asInt();  //blue
}

BLYNK_WRITE(V3)  //toggles pixel display On/Off, set as "switch"
{
  buttonA = param.asInt();  //value is either 0 or 1
}


//---------------------------------------------------------------
void setup()
{
  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  FastLED.addLeds<APA102,11,13,BGR,DATA_RATE_MHZ(12)>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
}


//---------------------------------------------------------------
void loop()
{
  Blynk.run();

  if (buttonA == 1) {
    fill_solid(leds, NUM_LEDS, CRGB(zeRGBa[0],zeRGBa[1],zeRGBa[2]) );
  } else {
    FastLED.clear();  //blank out all pixel data
  }
  
  FastLED.setBrightness(sliderBrightness);
  FastLED.show();
}


