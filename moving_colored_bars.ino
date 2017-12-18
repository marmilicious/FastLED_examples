//***************************************************************
// Moving colored bars
// Original code by Richard Bailey,  Feb. 2017
//
// Modified to allow a few other options (which might be
// useful for Chrismas!)
// You'll probably need to adjust stuff for longer strips.  I only
// tested with a tiny 32 pixel setup.
//
// Marc Miller, Dec 2017
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  64
CRGBArray<NUM_LEDS> leds;


#define FRAME_DELAY 80  //How fast to move (milliseconds)


/* Specify your colors and the order of those colors.
   You can create a longer section of a single color by
   listing it several times (such as green in this first
   example)
   Note: I used Grey instead of White here so it's not
   as bright compared to the other colors.
*/
long colorPallet[] = {CRGB::Green,CRGB::Grey,CRGB::Red,CRGB::Grey,CRGB::Red,CRGB::Grey,CRGB::Red,CRGB::Green,CRGB::Green,CRGB::Green};
//long colorPallet[] = {CRGB::Black,CRGB::Green,CRGB::Red};
//long colorPallet[] = {CRGB::Green,CRGB::Red};
//long colorPallet[] = {CRGB::Black,CRGB::Blue,CRGB::Yellow,CRGB::Purple};  //Black can be used too of course.


const int numberofColors = sizeof(colorPallet)/sizeof(int);  //Auto calculate your specified number of color bars


/* Play with setting the colorBarLength in different ways for slightly different effects. */
int colorBarLength = 10;  //Some specific length
//int colorBarLength = NUM_LEDS-1;  //The full strip length per color
//int colorBarLength = NUM_LEDS/2;  //Half the strip length per color
//int colorBarLength = NUM_LEDS/numberofColors;  //Strip length divided by number of colors



int frameCounter;  //These don't need to be changed
int palletPosition;
int colorBarPosition = 1;
bool clearLEDS = false;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop() {

  EVERY_N_MILLISECONDS(FRAME_DELAY) {

    for (int x=0; x<NUM_LEDS-1; x++)
    {
      leds[x] = leds[x+1];
    }
    if (clearLEDS)
    {
      leds[NUM_LEDS-1] = colorPallet[palletPosition];
    }
    if ((colorBarPosition <= colorBarLength) && !clearLEDS)
    {
      leds[NUM_LEDS-1] = colorPallet[palletPosition];
      colorBarPosition++;
    }
    if ((palletPosition == numberofColors-1) && (colorBarPosition > colorBarLength) && !clearLEDS)
    {
      leds[NUM_LEDS-1]=colorPallet[palletPosition];
      palletPosition = 0;
      colorBarPosition = 1;
      clearLEDS= true;
    }
    if ((colorBarPosition > colorBarLength) && !clearLEDS)
    {
      colorBarPosition = 1;
      palletPosition = palletPosition+1;
    }
    //if (clearLEDS && !leds(0,NUM_LEDS-1))  //Not using this for of test any more
    if (clearLEDS && leds[0]==(CRGB)(colorPallet[numberofColors-1]))  //restarts as soon as last color makes it past the end
    {
      //Serial.print( leds[0].r );  Serial.print("\t"), Serial.print( leds[0].g ); Serial.print("\t"), Serial.println( leds[0].b );  //Print out RGB colors it's triggering on
      clearLEDS = false;
    }

  }//end_EVERY_N

  FastLED.show();

}//end_main_loop


//---------------------------------------------------------------
//TODO:
//  Add option to run in reverse direction
//  Make some hot chocolate

