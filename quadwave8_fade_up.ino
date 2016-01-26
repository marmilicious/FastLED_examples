//***************************************************************
// test_quadwave8_fade_up pulse example
//
// Marc Miller, 2015
//***************************************************************

#include "FastLED.h"
#define LED_TYPE NEOPIXEL  // ***Update to your strip type.  NEOPIXEL, APA102, LPD8806, etc.
#define DATA_PIN 6  // ***Remember to set this to your data pin
#define NUM_LEDS 12  // ***Remember to set this to your strip length
#define MASTER_BRIGHTNESS 255  // Master brightness range is 0-255
CRGB leds[NUM_LEDS];

uint16_t count;
uint8_t speed = 15;  // Higher number gives slower pulse

//---------------------------------------------------------------
void setup() {
  delay(1000);  // Startup delay
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {
  uint8_t value = quadwave8(count/speed);
  //Serial.print("  count: ");  Serial.print(count);
  //Serial.print("    value: ");  Serial.println(value);
  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = CHSV(80,255,value);
  }
  FastLED.show();
  count++;
  if (value >= 255){  // If value is maxed out then reset count.
    count = 0;
  }

}
