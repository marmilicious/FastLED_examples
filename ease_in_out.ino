/*********************************************************************/
//ease in out test
/*********************************************************************/
#include <Arduino.h>
#include "FastLED.h"
#define LED_TYPE LPD8806
#define NUM_LEDS 32
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 255
CRGB leds[NUM_LEDS];

uint16_t pos;  // pixel postion.
uint16_t mappedValue;
uint8_t timeDelay = 100;  // time in ms.  Lower number is faster travel.


/*********************************************************************/
//PROGRAM SETUP
void setup() {
  Serial.begin(115200);  // Allow for output to serial monitor
  delay(3000);  // power up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}//END SETUP


/*********************************************************************/
//MAIN LOOP
void loop() {

  /*
  // Uncomment this section to show re-mapped
  // values for ease8InOutCubic from 0 to 256.
  for (uint16_t i = 0; i < 256; i++) {
    uint8_t result = ease8InOutCubic(i);
    Serial.print("  i: ");
    Serial.print(i);
    Serial.print("    ");
    Serial.print("result: ");
    Serial.println(result);
    delay(20);
    if (i == 255) { Serial.println("Loop complete."); }
  }
  Serial.println("Delay for a bit...");  // So you don't hang your serial monitor output.
  delay(10000);
  */

  // Loop through pixels.
  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    // Map pixel position to range of 0-255 that the ease function needs.
    mappedValue = map(i, 0, NUM_LEDS-1, 0, 255);  // map(inputValue, fromLow, fromHigh, toLow, toHigh)

    // Run mappedValue through ease funtion.
    mappedValue = ease8InOutCubic(mappedValue);

    // Map ease value (0-255 range) back to pixel position in range of 0 to NUM_LEDS.
    pos = map(mappedValue, 0, 255, 0, NUM_LEDS-1);  // map(inputValue, fromLow, fromHigh, toLow, toHigh)
  
    // Display pixels
    leds[pos] = CRGB::Red;
    FastLED.show();
    delay(timeDelay);
    leds[pos] = CRGB::Black;
    FastLED.show();
  }
  
  delay(1000);
}//END MAIN LOOP
