//***************************************************************
// Marquee fun (v3)
//  Pixel position down the strip comes from this formula:
//      pos = spacing * (i-1) + spacing
//  i starts at 0 and is incremented by +1 up to NUM_LEDS/spacing.
//
// Marc Miller, May 2016
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 8

uint16_t holdTime = 700;  // Milliseconds to hold position before advancing.
uint8_t spacing = 4;      // Sets pixel spacing. [Use 2 or greater]
int8_t delta = 1;         // Sets forward or backwards direction amount. (Can be negative.)
uint8_t width = 2;        // Can increase the number of pixels (width) of the chase. [1 or greater]

boolean fadingTail = 0;   // Add fading tail? [1=true, 0=falue]
uint8_t fadeRate = 150;   // How fast to fade out tail. [0-255]

uint8_t hue = 60;         // Starting color.
uint8_t hue2_shift = 50;  // Hue shift for secondary color.  Use 0 for no shift. [0-255]
boolean DEBUG = 0;        // Print some info to serial monitor. [1=true, 0=falue]

int16_t pos;              // Pixel position.
int8_t advance;           // Stores the advance amount.
uint8_t color;            // Stores a hue color.


//---------------------------------------------------------------
void setup() {
  //Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(3000);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {

  EVERY_N_SECONDS(40){  // Demo: Change direction every N seconds.
    delta = -1*delta;
  }

  //EVERY_N_SECONDS(10){  // Demo: Changing the pixel spacing every N seconds.
  //  spacing = spacing + 1;
  //  if (spacing == 9) { spacing = 2; }  // Reset spacing to 2
  //  if (spacing >4) { spacing = 8; }  // Jump spacing up to 8
  //}

  EVERY_N_SECONDS(10){  // Demo: Change the hue every N seconds.
    hue = hue + random8(30,61);  // Shift the hue to something new.
  }


  EVERY_N_MILLISECONDS(holdTime){  // Advance pixels to next position.
    if (DEBUG==1) {  // Print out some info if DEBUG is true.
      Serial.print("delta: "); Serial.print(delta);
      Serial.print("   spacing: "); Serial.print(spacing); Serial.print("    ");
    }
    for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
      for (uint8_t w = 0; w<width; w++){
        pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
        if ( w % 2== 0 ){  // Is w even or odd?
          color = hue;
        } else {
          color = hue + hue2_shift;
        }
        
        leds[pos] = CHSV(color,255,255);
      }
      
      if (DEBUG==1) {  // Print out lit pixels if DEBUG is true.
        Serial.print(" "); Serial.print(pos);
      }
    }
    if (DEBUG==1) { Serial.println(" "); }
    FastLED.show();
  
    // Fade out tail or set back to black for next loop around.
    if (fadingTail == 1) {
      fadeToBlackBy(leds, NUM_LEDS,fadeRate);
    } else {
      for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
        for (uint8_t w = 0; w<width; w++){
          pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
          leds[pos] = CRGB::Black;
        }
      }
    }
  
    // Advance pixel postion down strip, and rollover if needed.
    advance = (advance + delta + NUM_LEDS) % NUM_LEDS;
  }

}//end_main_loop
