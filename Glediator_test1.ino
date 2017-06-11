//***************************************************************
// Testing out reading RGB data sent from Glediator
// http://www.solderlab.de/index.php/software/glediator
//
// For Glediator’s serial protocol each frame starts with 0x01,
// which is then followed by all the pixel data.  The number of
// LEDs (matrix size) needs to be set in Glediator, and also
// the output needs to be set to:
//    Output Mode:  Glediator_Protocol
//    Mapping Mode: Single_Pixels
//    Pixel Order:  HL_BL (for a single strip, or one of the the others per your setup)
//    Color Order:  RGB
//
//    Baud-Rate:    1,000,000 was suggested, but you can use 
//                  whatever as long as they are both set the
//                  same here and in Glediator.
//
//    *** Upload and have this code running first, then start up
//    Glediator otherwise it won't give you a COM port number.
//
//
// Marc Miller, June 2017
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];

#define BAUD_RATE 115200  //1000000  //250000   // Set the same in Glediator
#define CMD_NEW_DATA 1


//---------------------------------------------------------------
void setup() {
  Serial.begin(BAUD_RATE);
  delay(1500);  // startup delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  startupLEDsTest();  // run quick RGB color test
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
int serialGlediator () {
  while ( !Serial.available() ) {}
  return Serial.read();
}


//---------------------------------------------------------------
void loop() {
  while (serialGlediator () != CMD_NEW_DATA) {}
  Serial.readBytes((char*)leds, NUM_LEDS * 3);
  FastLED.show();
}


//---------------------------------------------------------------
void startupLEDsTest() {
  // startup blink test to confirm LEDs are working.
  FastLED.setBrightness(32);
  fill_solid(leds, NUM_LEDS, CRGB(255,0,0));  // fill red
  FastLED.show();
  delay(1000);
  fill_solid(leds, NUM_LEDS, CRGB(0,255,0));  // fill green
  FastLED.show();
  delay(1000);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));  // fill blue
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(BRIGHTNESS);
}
