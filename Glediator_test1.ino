//***************************************************************
// Testing out reading RGB data sent from Glediator
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

#define BAUD_RATE 1000000  //250000  //115200
#define CMD_NEW_DATA 1


//---------------------------------------------------------------
void setup() {
  Serial.begin(BAUD_RATE);
  delay(2000);  // startup delay
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
int serialGlediator () {
  while (!Serial.available()) {
    EVERY_N_SECONDS(2) {
      Serial.println("no incoming data...");  // Print if not receiving anything
    }
  }
  return Serial.read();
}


//---------------------------------------------------------------
void loop() {
  while (serialGlediator () != CMD_NEW_DATA) {}
  Serial.readBytes((char*)leds, NUM_LEDS * 3);
  FastLED.show();
}

