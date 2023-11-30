//***************************************************************
// This sketch uses fill_solid to fill randomly sized blocks of
// pixels.
//
// This would fill 4 pixels red, starting at leds[7]
//    fill_solid( &(leds[7]), 4, CRGB::Red );
//
// If we use variables for the start position and how many
// pixels to fill we can randomly change things as we loop.
//
//
// When using fill_solid like above one needs to be very careful
// not to fill past the end of the CRGB leds array. In other words,
// make sure you don't try to write data past leds[NUM_LED - 1].
// Writing RGB data to a pixel that don't exist causes bad things
// to happen in memory. "Bad things" could mean very random
// infrequent display errors, very visible incorrect display
// errors, and/or the controller locking up.
//
//
// Marc Miller,  November 2023
//***************************************************************

#include <FastLED.h>
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 128
CRGB leds[NUM_LEDS];

uint8_t hue;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);  // ***For strips using Clock.
  //FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  // Set all pixels to black
  Serial.println("Setup done.\n");
}


//---------------------------------------------------------------
void loop() {

  uint8_t fill_size = random8(3,7);  // random fill size from 3 to 6 pixels.

  EVERY_N_MILLISECONDS(1500) {
    uint8_t pos = 0;  // reset position to begining of strip

    while (pos < NUM_LEDS-1) {
      // uint8_t fill_size = random8(2,9);  // uncomment for varying sizes throughout

      if ((pos + fill_size) > NUM_LEDS-1) {  // check we don't go past end of CRGB array!
        fill_size = NUM_LEDS - pos;  // adjust fill_size if needed
      }

      hue = hue + random8(64,129);
      fill_solid( &(leds[pos]), fill_size, CHSV(hue, random8(120,220), 255) );

      pos = pos + fill_size;  // set next start position
    }

  }

  FastLED.show();
  delay(1);

}//end_main_loop
