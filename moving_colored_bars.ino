//***************************************************************
// Moving colored bars
// Originally based on code by Richard Bailey, Feb. 2017.
//
// Create a color pattern to be scrolled and repeated down the
// strip. The rate of travel and length of a colored bar can be
// specified. Play with different lengths for different looks.
//
//
// Marc Miller, Dec 2017
//              Aug 2022 - redone to (hopefully) fix a bug,
//                         renamed variables and simplified code
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  64
CRGBArray<NUM_LEDS> leds;


// Controls how fast to travel down the strip
#define FRAME_DELAY 80  // [milliseconds]


// Specify the colors in the order you would like. You can
// create a longer section of color by listing it several
// times (such as Green in the second example here).
// Black can also be used to make a gap between colors.
//
long colorPalette[] = {CRGB::Green,CRGB::Cyan,CRGB::Purple,CRGB::Cyan};
//long colorPalette[] = {CRGB::Green,CRGB::Green,CRGB::Orange,CRGB::Red};
//long colorPalette[] = {CRGB::Black,CRGB::Blue,CRGB::Yellow};
//long colorPalette[] = {CRGB::Green,CRGB::Grey,CRGB::Red,CRGB::Grey,CRGB::Red,CRGB::Grey};
//long colorPalette[] = {CRGB::Red,CRGB::White,CRGB::Blue,CRGB::Black,CRGB::Black};

// This auto calculates the number of colored bars you specified.
const int numberofColors = sizeof(colorPalette)/sizeof(long);  


// Length of a color bar 
int colorBarLength = 5;  // [number of pixels]
//int colorBarLength = NUM_LEDS-1;  // The full strip length per color
//int colorBarLength = NUM_LEDS/2;  // Half the strip length per color
//int colorBarLength = NUM_LEDS/numberofColors;  // Strip length divided by number of colors


int paletteIndex = 0;
int barPosition = 1;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.print("Number of colors: "); Serial.println(numberofColors);
  Serial.print("colorBarLength: "); Serial.println(colorBarLength);
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop() {

  EVERY_N_MILLISECONDS(FRAME_DELAY) {
    //Serial.print("barPosition: "); Serial.print(barPosition);
    //Serial.print("\tpaletteIndex: "); Serial.println(paletteIndex);

    // move current pixel data over one position
    for (uint16_t x = 0; x < NUM_LEDS-1; x++)
    {
      leds[x] = leds[x+1];
    }

    // assign new data at end of line
    leds[NUM_LEDS-1] = colorPalette[paletteIndex];

    barPosition++;

    // check and reset things as needed
    if ( (barPosition > colorBarLength) && (paletteIndex == numberofColors-1) )
    {
      barPosition = 1;
      //Serial.println("  barPosition reset");
      paletteIndex = 0;
      //Serial.println("  paletteIndex reset");
    }
    else if (barPosition > colorBarLength)
    {
      barPosition = 1;
      //Serial.println("  barPosition reset");
      paletteIndex = paletteIndex + 1;
    }
    
  }//end_EVERY_N

  FastLED.show();

}//end_main_loop
