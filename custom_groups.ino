//***************************************************************
// custom_groups.ino
// Example of making custom groups for specific sets of LEDs.
//***************************************************************

#include "FastLED.h"
#define LED_TYPE NEOPIXEL  // ***Update to your strip type.  NEOPIXEL, APA102, LPD8806, etc.
#define DATA_PIN 6  // ***Set this to your data pin number.
#define NUM_LEDS 12  // ***Remember to set this to your strip length.
#define MASTER_BRIGHTNESS 255  // Master brightness range is 0-255.
CRGB leds[NUM_LEDS];

// Specify the number of pixels and which pixels are in each custom group.
// Rename these as needed to make sense for your project!
#define NUM_GROUP_A 2  // The number of pixels in this custom group.
#define NUM_GROUP_B 4  // The number of pixels in this custom group.
uint8_t groupA[NUM_GROUP_A] = {0,5};  // The pixels in this custom group.
uint8_t groupB[NUM_GROUP_B] = {1,2,3,4};  // The pixels in this custom group.

//---------------------------------------------------------------
void setup() {
  delay(1000);  // Startup delay
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}

//---------------------------------------------------------------
void loop () {
  // Initially set all pixels to a color.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(80,255,255);
  }
  FastLED.show();
  delay(1200);

  // Set the custom groups to some other colors.
  for (int i = 0; i < NUM_GROUP_A; i++) {
    leds[groupA[i]] = CRGB::Red;
  }

  for (int i = 0; i < NUM_GROUP_B; i++) {
    leds[groupB[i]] = CRGB::Yellow;
  }
  FastLED.show();
  delay(800);

}//end main loop
