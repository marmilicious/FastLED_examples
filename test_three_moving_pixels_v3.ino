//***************************************************************
// Three moving pixels example
//***************************************************************

#include "FastLED.h"
#define LED_TYPE NEOPIXEL  // ***Update to your strip type.  NEOPIXEL, APA102, LPD8806, etc.
#define DATA_PIN 6  // ***Remember to set this to your data pin
//#define CLOCK_PIN 13  // ***Remember to set this to your clock pin
#define NUM_LEDS 12  // ***Remember to set this to your strip length
//#define COLOR_ORDER BGR
#define MASTER_BRIGHTNESS 100  // Master brightness range is 0-255
CRGB leds[NUM_LEDS];
int16_t positionWhite = 4;  // Set initial start position of White pixel
int16_t positionRed = 0;  // Set initial start position of Red pixel
int16_t positionBlue = 8;  // Set initial start position of Blue pixel

uint16_t holdTime = 1000;  // Milliseconds to hold position before advancing
int8_t delta = -1;  // 1 or -1.  Sets forward or backwards direction.


//---------------------------------------------------------------
void setup() {
  delay(1000);  // Startup delay
  //Serial.begin(57600);  // Allows serial monitor output (check baud rate)
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN>,COLOR_ORDER,(leds, NUM_LEDS);  // ***For strips using Clock.
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {  // START MAIN LOOP

  // Set position and color of pixels
  leds[(positionWhite + delta + NUM_LEDS) % NUM_LEDS] = CRGB::White;
  leds[(positionRed + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Red;
  leds[(positionBlue + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Blue;

  // Show the pixels
  // NOTE:  Using FastLED.delay will automatically also "show" the pixels,
  // therefore no need for a FastLED.show imediately followed by FastLED.delay
  //FastLED.show();
  FastLED.delay(holdTime);  // Delay for a bit.
 
  // Set pixels back to Black for the next loop around.
  leds[(positionWhite + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Black;
  leds[(positionRed + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Black;
  leds[(positionBlue + delta + NUM_LEDS) % NUM_LEDS] = CRGB::Black;

  // Advance position based on delta, and rollover if needed.
  positionWhite = ((positionWhite + delta + NUM_LEDS) % NUM_LEDS);
  positionRed = ((positionRed + delta + NUM_LEDS) % NUM_LEDS);
  positionBlue = ((positionBlue + delta + NUM_LEDS) % NUM_LEDS);


  //Serial.print("position = "); Serial.println(positionWhite);

}  // END MAIN LOOP
