//***************************************************************
// Duplicate pixel data from a virtual small array to blocks of
// pixels in a larger physical array. In other word, each block
// of pixels in the larger physical array gets it's color from
// one of the corresponding pixels in the small virtual array.
//
// Keep the smaller virtual array named "leds" for ease of
// reusing existing chunks of code for generating patterns.
//
// Name the array for the physical display "show", and update
// FastLED.addLeds line in setup to reflect this.
//
// Generate patterns into leds as usual. Copy pixel data from
// leds to show, then call show() to update the physical display.
//
//
// Marc Miller,  Jan 2023
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLOCK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define BRIGHTNESS  255

#define SHOW_LEDS   32  // the total number of physcial pixles that actually get displayed
CRGB show[SHOW_LEDS];   // the array that gets displayed

#define PPBLOCK     4   // the number of Pixels Per Block
#define NUM_LEDS    (SHOW_LEDS/PPBLOCK)   // the number of blocks, which will be the size of the virtual array
CRGB leds[NUM_LEDS];   // the array where patterns are generated


uint8_t hue;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output
  delay(2000); // startup delay

  // Use the CRGB array show[SHOW_LEDS] here, as that is what actually physically exists and is displayed.
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(show, SHOW_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  Serial.print("Total physical pixels: ");
  Serial.println(SHOW_LEDS);

  Serial.print("Pixels Per Block: ");
  Serial.println(PPBLOCK);

  Serial.print("NUM_LEDS in virtual array (ie. the number of blocks): ");
  Serial.println(NUM_LEDS);

  Serial.println("\nSetup done. \n");
}


//---------------------------------------------------------------
void loop() { 

  EVERY_N_MILLISECONDS(200) {
    hue++;
  }

  fill_gradient(leds, NUM_LEDS, CHSV(hue,255,255), CHSV(hue+64,255,255),LONGEST_HUES);
  uint8_t blockPos = beatsin8(6,0,NUM_LEDS+1);
  //Serial.println(blockPos);
  if (blockPos > 0 && blockPos < NUM_LEDS+1) {
    uint8_t val = leds[blockPos-1].getAverageLight();
    leds[blockPos-1] = CRGB(val,val,val);
  }


  COPY_LEDS2SHOW();  // copy data from leds array to show array
  FastLED.show();    // update the display on the physical pixels

}//end main loop


//---------------------------------------------------------------
void COPY_LEDS2SHOW() {
  // copy pixel data from leds to show array
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    fill_solid(&show[(i*PPBLOCK)], PPBLOCK, leds[i]);  //fill start position, number of pixels to fill, fill color
  }
}



