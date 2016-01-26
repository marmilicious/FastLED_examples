/*********************************************************************/
// sin cos linear test
// This test cycles a few LEDs through the color wheel
// to compare sin8 vs cos8 vs linear.
//     Pixels 0 & 1 use sin8(0-255)
//     Pixels 2 & 3 use cos8(0-255)
//     Pixels 4 & 5 with linear 0-255
//     Pixels 7 lights white at the loop ends as an indicator.
//
// Marc Miller, 2015
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

uint8_t potValue;  // Simulated potentiometer value.
uint8_t sin_output;
uint8_t cos_output;
uint16_t slowBy = 15;  // miliseconds to delay


/*********************************************************************/
//PROGRAM SETUP
void setup() {
  Serial.begin(115200);  // Allow for output to serial monitor
  delay(2000);  // power up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}//END SETUP


/*********************************************************************/
//MAIN LOOP
void loop() {
  //====run forward direction====
  for (potValue = 0; potValue < 255; potValue++) {
    sin_output = sin8(potValue);
    cos_output = cos8(potValue);

    for (uint8_t j = 0; j < 2; j++) {
      leds[j] = CHSV(sin_output, 255, 255);
    }
    for (uint8_t j = 2; j < 4; j++) {
      leds[j] = CHSV(cos_output, 255, 255);
    }
    for (uint8_t j = 4; j < 6; j++) {
      leds[j] = CHSV(potValue, 255, 255);
    }

    FastLED.show();
    Serial.print("  potValue = "); Serial.print(potValue);
    Serial.print("\tsin8: "); Serial.print(sin_output);
    Serial.print("\tcos8: "); Serial.println(cos_output);

    //Blink pixel 7 white as a visual to show loop ends.
    if (potValue == 0) {
      leds[7] = CHSV(0,0,64);
      Serial.println("----------------------------------------------");
    }
    else {
      leds[7] = CRGB::Black;
    }

    delay(slowBy);
  }

  //====run reverse direction====
  for (potValue = 255; potValue > 0; potValue--) {
    sin_output = sin8(potValue);
    cos_output = cos8(potValue);

    for (uint8_t j = 0; j < 2; j++) {
      leds[j] = CHSV(sin_output, 255, 255);
    }
    for (uint8_t j = 2; j < 4; j++) {
      leds[j] = CHSV(cos_output, 255, 255);
    }
    for (uint8_t j = 4; j < 6; j++) {
      leds[j] = CHSV(potValue, 255, 255);
    }

    FastLED.show();
    Serial.print("  potValue = "); Serial.print(potValue);
    Serial.print("\tsin8: "); Serial.print(sin_output);
    Serial.print("\tcos8: "); Serial.println(cos_output);

    //Blink pixel 7 white as a visual to show loop ends.
    if (potValue == 255) {
      leds[7] = CHSV(0,0,64);
      Serial.println("----------------------------------------------");
    }
    else {
      leds[7] = CRGB::Black;
    }

    delay(slowBy);
  }

}//END MAIN LOOP
