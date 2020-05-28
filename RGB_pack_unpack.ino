//***************************************************************
// Example of packing R,G,B values into a single long (uint32_t)
// number and then unpacking that back to individual R,G,B values.
// 
// This could be useful if you want to receive R,G,B data as a
// single value per pixel and then unpack it to assign the
// R,G,B values to a pixel, or if you want to pack the R,G,B
// into a single number to send to some other program.
//
// Marc Miller, May 2020
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 100
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];


uint32_t packedRGB;  // place to store the combined R,G,B values

#define  RGB_PACKED(R,G,B) (uint32_t)( ((R<<16) | ((G)<<8) | (B) ))


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done.  \n");
}


//---------------------------------------------------------------
void loop() {

  EVERY_N_SECONDS(2) {
    
    // Assign random R,G,B values to first pixel
    leds[0] = CRGB( random8(), random8(), random8() );

    // Print out R,G,B values
    Serial.print("leds[0]: ");
    Serial.print(leds[0].r); Serial.print(" ");
    Serial.print(leds[0].g); Serial.print(" ");
    Serial.println(leds[0].b);

    // Pack R,G,B values into a single long number and print out
    packedRGB = RGB_PACKED( leds[0].r, leds[0].g, leds[0].b );
    Serial.print("packedRGB: "); Serial.println( packedRGB );

    // Unpack back to R,G,B values and assign to second pixel
    leds[1].r = (packedRGB >> 16) & 0xFF;
    leds[1].g = (packedRGB >> 8) & 0xFF;
    leds[1].b = packedRGB & 0xFF;

    // Print out R,G,B values to confirm they match first pixel's values
    Serial.print("leds[1]: ");
    Serial.print(leds[1].r); Serial.print(" ");
    Serial.print(leds[1].g); Serial.print(" ");
    Serial.println(leds[1].b);


    // Display the first two pixels to confirm they match.
    FastLED.show();

    Serial.println("\n");
  }
  
} // end_main_loop


