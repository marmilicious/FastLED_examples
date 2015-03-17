//===============================================================
// hue_and_brightness_example.ino
//
// Example setup for using two potentiometers to control hue and
// brightness with FastLED.
//
// Every time the function checkKnobs() is called it will read
// the current value of both potentiometers and map those values
// to a hue and master brightness for FastLED.
//===============================================================

#include "FastLED.h"
#define LED_TYPE NEOPIXEL
#define DATA_PIN 6
#define NUM_LEDS 12
#define MASTER_BRIGHTNESS 255  // Set the master brigtness value [should be greater then min_brightness value].
uint8_t min_brightness = 30;   // Set a minimum brightness level.
uint8_t brightness;            // Mapped master brightness based on potentiometer reading.
CRGB leds[NUM_LEDS];

int potPinA = A0;   // Pin for potentiometer A (for hue)
int potPinB = A1;   // Pin for potentiometer B (for brightness)
int potValA;        // Variable to store potentiometer A value (for hue)
int potValB;        // Variable to store potentiometer B value (for brightness)
uint8_t hue;        // Hue color (0-255)


//---------------------------------------------------------------
void setup() {
  delay(2000);  // Power-up delay
  //Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(MASTER_BRIGHTNESS);  // Initially set the max brightness.  Will continuously update it later.
  pinMode(potPinA, INPUT);  // Set pin as an input.
  pinMode(potPinB, INPUT);  // Set pin as an input.
}


//---------------------------------------------------------------
void loop() {
  checkKnobs();  // Call function to check knob positions.

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);  // hue comes from pot A, and brightness value is scaled based on pot B.
  }
  FastLED.show();
}


//===============================================================
void checkKnobs(){
  potValA = analogRead(potPinA);  // Read potentiometer A (for hue).
  //potValA = map(potValA, 1023, 0, 0, 1023);  // Reverse reading if potentiometer is wired backwards. 
  hue = map(potValA, 0, 1023, 0, 255);  // map(value, fromLow, fromHigh, toLow, toHigh)

  potValB = analogRead(potPinB);  // Read potentiometer B (for brightness).
  brightness = map(potValB, 0, 1023, min_brightness, MASTER_BRIGHTNESS);
      // Map value between min_brightness and MASTER brightness values.
      // Note: We are limiting the lowest possible brightness value to the
      // min_brightness value assigned up top.
  FastLED.setBrightness(brightness);  // Set master brightness based on potentiometer position.

  //Serial.print("  pot A: "); Serial.print(potValA); Serial.print("    hue: "); Serial.print(hue);
  //Serial.print("    pot B: "); Serial.print(potValB); Serial.print("    brightness: "); Serial.println(brightness);
}


//---------------------------------------------------------------

