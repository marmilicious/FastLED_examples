//***************************************************************
// Cellular noise, based on this reddit post about ChatGPT generated code:
// https://www.reddit.com/r/FastLED/comments/187txfo/have_many_folks_played_around_with_chatgpt_and/
//
// Not to shabby for ChatGPT generated code. :)
//
// Marc Miller, Dec 2023
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS 255
CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND 60

void setup() {
  Serial.begin(115200);  // Allows serial monitor output
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  FastLED.clear();
  Serial.println("Setup done.\n");
}


//---------------------------------------------------------------
void loop() {
  // Fill the LED array with colors based on cellular noise
  fillCellularNoise();

  // Show the LED array
  FastLED.show();

  // Delay to control the animation speed
  delay(1000 / FRAMES_PER_SECOND);
}

void fillCellularNoise() {
  // Define the properties of the cellular noise
  uint8_t density = 150;  // Adjust this value to control the density of cells
  uint8_t scale = 16;     // Adjust this value to control the size of cells
  uint8_t colorSpeed = 4; // Adjust this value to control the color animation speed

  // Iterate through each pixel in the LED array
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    // Calculate the cellular noise value for the current pixel
    uint8_t value = inoise8(i * scale, millis() / colorSpeed) % 255;

    // Map the noise value to the LED's color range
    leds[i] = ColorFromPalette(RainbowColors_p, value, 255, LINEARBLEND);

    // Apply density threshold to create cells
    if (value > density) {
      leds[i] = CRGB::Black;  // Set the cell color to black
    }
  }
}
