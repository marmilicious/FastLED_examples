//***************************************************************
//  Two rainbow glitter patterns with color glitter instead of
//    white glitter.  One picks random colors, the other slightly
//    shifts the hue of whatever pixel is randomly selected.
//
//  These can be added to Mark Kreigman's DemoReel100 example as
//    additional patterns to run.  Copy lines 25 and 26 as well
//    as the two patterns below to your DemoReel100 program.
//
//  Marc Miller, Oct 2016
//***************************************************************

#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define DATA_PIN    11
#define CLK_PIN     13
#define LED_TYPE    APA102
#define COLOR_ORDER BGR
#define NUM_LEDS    38
#define BRIGHTNESS  96
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];

uint16_t ledsData[NUM_LEDS][4];  // array to store RGB data and an extra value
uint16_t pick;  // stores a temporary pixel number


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(3000); // 3 second delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();

SimplePatternList gPatterns = { rainbowShiftedGlitter, rainbowRandomGlitter };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


//---------------------------------------------------------------
void loop() {
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND);  // slows the framerate to a modest value

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}


//---------------------------------------------------------------
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}



//===============================================================
// The different patterns to choose from...
//===============================================================
//---------------------------------------------------------------
void rainbowRandomGlitter() {
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  if( random8() < 45 ) {  // How often to glitter things up!  Higher number is more often.
    pick = random16(NUM_LEDS);
    if (ledsData[pick][3] == 0 ) {
      ledsData[pick][3] = 40;  // Used to tag pixel and determine glitter time
      ledsData[pick][0] = random8();  // Pick random rgb values
      ledsData[pick][1] = random8();
      ledsData[pick][2] = random8();
    }
  }
  for (uint16_t i=0; i < NUM_LEDS; i++) {
    if (ledsData[i][3] == 0) {
      leds[i].fadeToBlackBy(180);  // Fade down non-glittering pixels so glitter will show up more
    } else {
      //leds[i] = CRGB(ledsData[i][0], ledsData[i][1], ledsData[i][2]);  // Plug in rgb values
      leds[i].fadeToBlackBy(8);  // Slowly fade down
      ledsData[i][0] = leds[i].r;  // Store the rgb values back in ledsData array
      ledsData[i][1] = leds[i].g;
      ledsData[i][2] = leds[i].b;
      ledsData[i][3] = ledsData[i][3] - 1;
    }
  }
}//end rainbowRandomGlitter


//---------------------------------------------------------------
void rainbowShiftedGlitter() {
  fill_rainbow( leds, NUM_LEDS, gHue, 3);
  if( random8() < 30 ) {  // How often to glitter things up!  Higher number is more often.
    pick = random16(NUM_LEDS);
    if (ledsData[pick][3] == 0 ) {
      ledsData[pick][3] = 35;  // Used to tag pixel and determine glitter time
      CRGB rgb(leds[pick].r, leds[pick].g, leds[pick].b);
      CHSV hsv = rgb2hsv_approximate(rgb);  // Used to get approx Hue
      leds[pick] = CHSV(hsv.hue-50, 255, 255);  // Color shift Hue on glitter pixel
      ledsData[pick][0] = leds[pick].r;  // Store rgb values back in ledsData array
      ledsData[pick][1] = leds[pick].g;
      ledsData[pick][2] = leds[pick].b;
    }
  }
  for (uint16_t i=0; i < NUM_LEDS; i++) {
    if (ledsData[i][3] == 0) {
      leds[i].fadeToBlackBy(180);  // Fade down non-glittering pixels so glitter will show up more
    } else {
      leds[i] = CRGB(ledsData[i][0], ledsData[i][1], ledsData[i][2]);  // Plug in rgb values
      leds[i].fadeToBlackBy(7);  // Slowly fade down
      ledsData[i][0] = leds[i].r;  // Store the rgb values back in ledsData array
      ledsData[i][1] = leds[i].g;
      ledsData[i][2] = leds[i].b;
      ledsData[i][3] = ledsData[i][3] - 1;
    }
  }
}//end rainbowShiftedGlitter



//---------------------------------------------------------------
