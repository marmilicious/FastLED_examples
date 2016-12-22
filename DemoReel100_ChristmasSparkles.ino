//***************************************************************
//  Random sparkle patterns with Christmas-ish colors.
//    Patterns:  christmasSparkles, christmasSparklesRB, christmasSparklesBP
//
//  This can be added to Mark Kriegsman's DemoReel100 as new patterns to run.
//  Copy lines 25 and 26 as well as the patterns below to your program.
//
//  Marc Miller, Dec 2016
//***************************************************************

#include "FastLED.h"
FASTLED_USING_NAMESPACE

#define DATA_PIN    11
#define CLK_PIN     13

#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND  120
CRGB leds[NUM_LEDS];

uint16_t ledsData[NUM_LEDS][4];  // array to store color data and an extra value
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

SimplePatternList gPatterns = { christmasSparkles, christmasSparklesRG, christmasSparkles, christmasSparklesBP };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//---------------------------------------------------------------
void loop() {
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  //FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND);  // slows the framerate to a modest value

  // do some periodic updates
  EVERY_N_SECONDS( 20 ) { nextPattern(); } // change patterns periodically

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
void christmasSparkles() {
  //"Background" color for non-sparkling pixels.
  CRGB sparkleBgColor = CHSV(50, 30, 40);  // dim white
  //CRGB sparkleBgColor = CHSV(96, 200, 30);  // dim green
  
  EVERY_N_MILLISECONDS(40){
    if( random8() < 60 ) {  // How much to sparkle!  Higher number is more.
      pick = random16(NUM_LEDS);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 35;  // Used to tag pixel as sparkling
        uint8_t randomPick = random8(5);
        if (randomPick == 0) {
          ledsData[pick][0] = 178;   // sparkle hue (blue)
          ledsData[pick][1] = 244;  // sparkle saturation
          ledsData[pick][2] = 210;  // sparkle value
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 10;  // sparkle hue (red)
          ledsData[pick][1] = 255;  // sparkle saturation
          ledsData[pick][2] = 240;  // sparkle value
        }
        if (randomPick == 2) {
          ledsData[pick][0] = 0;  // sparkle hue (white-ish)
          ledsData[pick][1] = 25;  // sparkle saturation
          ledsData[pick][2] = 255;  // sparkle value
        }
        if (randomPick == 3) {
          ledsData[pick][0] = 35;   // sparkle hue (orange)
          ledsData[pick][1] = 235;  // sparkle saturation
          ledsData[pick][2] = 245;  // sparkle value
        }
        if (randomPick == 4) {
          ledsData[pick][0] = 190;  // sparkle hue (purple)
          ledsData[pick][1] = 255;  // sparkle saturation
          ledsData[pick][2] = 238;  // sparkle value
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i=0; i < NUM_LEDS; i++) {
      if (ledsData[i][3] == 0) {  // if not sparkling, set to "back ground" color
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);  // Used to get approximate Hue
        EVERY_N_MILLISECONDS(38) { ledsData[i][0] = hsv.hue - 1; }  // slightly shift hue
        ledsData[i][2] = scale8(ledsData[i][2], 245);  // slowly darken
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;  // countdown sparkle tag
      }
    }
  }
}//end christmasSparkles


//---------------------------------------------------------------
void christmasSparklesRG() {  // Red and Green only
  //"Background" color for non-sparkling pixels.  Can be set to black for no bg color.
  CRGB sparkleBgColor = CHSV(0, 0, 0);  // black
  //CRGB sparkleBgColor = CHSV(50, 30, 30);  // dim white
 
  EVERY_N_MILLISECONDS(40){
    if( random8() < 110 ) {  // How much to sparkle!  Higher number is more.
      pick = random16(NUM_LEDS);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 65;  // Used to tag pixel as sparkling
        uint8_t randomPick = random8(2);
        if (randomPick == 0) {
          ledsData[pick][0] = 16;  // sparkle hue (red)
          ledsData[pick][1] = 253;  // sparkle saturation
          ledsData[pick][2] = 242;  // sparkle value
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 96;   // sparkle hue (green)
          ledsData[pick][1] = 230;  // sparkle saturation
          ledsData[pick][2] = 255;  // sparkle value
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i=0; i < NUM_LEDS; i++) {
      if (ledsData[i][3] == 0) {  // if not sparkling, set to "back ground" color
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);  // Used to get approximate Hue
        EVERY_N_MILLISECONDS(50) { ledsData[i][0] = hsv.hue - 1; }  // slightly shift hue
        ledsData[i][2] = scale8(ledsData[i][2], 253);  // slowly darken
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;  // countdown sparkle tag
      }
    }
  }
}//end christmasSparklesRG


//---------------------------------------------------------------
void christmasSparklesBP() {  // Blues and Purple only
  //"Background" color for non-sparkling pixels.
  CRGB sparkleBgColor = CHSV(96, 185, 30);  // green
  
  EVERY_N_MILLISECONDS(40){
    if( random8() < 170 ) {  // How much to sparkle!  Higher number is more.
      pick = random16(NUM_LEDS);
      if (ledsData[pick][3] == 0 ) {
        ledsData[pick][3] = 20;  // Used to tag pixel as sparkling
        uint8_t randomPick = random8(3);
        if (randomPick == 0) {
          ledsData[pick][0] = 165;   // sparkle hue (blue)
          ledsData[pick][1] = 180;  // sparkle saturation
          ledsData[pick][2] = 230;  // sparkle value
        }
        if (randomPick == 1) {
          ledsData[pick][0] = 200;  // sparkle hue (pink-light-purple)
          ledsData[pick][1] = 170;  // sparkle saturation
          ledsData[pick][2] = 240;  // sparkle value
        }
        if (randomPick == 2) {
          ledsData[pick][0] = 130;  // sparkle hue (light blue)
          ledsData[pick][1] = 200;  // sparkle saturation
          ledsData[pick][2] = 255;  // sparkle value
        }
        leds[pick] = CHSV(ledsData[pick][0], ledsData[pick][1], ledsData[pick][2]);
      }
    }
    for (uint16_t i=0; i < NUM_LEDS; i++) {
      if (ledsData[i][3] == 0) {  // if not sparkling, set to "back ground" color
        leds[i] = sparkleBgColor;
      } else {
        CHSV hsv = rgb2hsv_approximate(leds[i]);  // Used to get approximate Hue
        EVERY_N_MILLISECONDS(20) { ledsData[i][0] = hsv.hue - 1; }  // slightly shift hue
        ledsData[i][2] = scale8(ledsData[i][2], 242);  // slowly darken
        leds[i] = CHSV(ledsData[i][0], ledsData[i][1], ledsData[i][2]);
        ledsData[i][3] = ledsData[i][3] - 1;  // countdown sparkle tag
      }
    }
  }
}//end christmasSparklesBP


//---------------------------------------------------------------
