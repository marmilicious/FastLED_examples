//***************************************************************
// Basic palette example using FastLED palettes and two custom
// palettes.
//
// FastLED includes these pre-defined palettes:
//   RainbowColors_p, RainbowStripeColors_p, OceanColors_p,
//   CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
//
// Marc Miller,  Dec 2019
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 100
CRGB leds[NUM_LEDS];

uint8_t startIndex;
uint8_t colorIndex;
uint8_t pal_choice;
CRGBPalette16 currentPalette( RainbowColors_p );

void SetupGoldenPalette()
{
  // custom colors for palette
  CRGB gold1 = CHSV( 30, 180, 255);
  CRGB gold2 = CHSV( 40, 220, 255);
  CRGB gold3 = CHSV( 60, 200, 255);
  CRGB gold4 = CHSV( 45, 120, 255);

  // fill the custom palette
  currentPalette = CRGBPalette16( 
    gold1, gold2, gold3, gold2,
    gold3, gold4, gold3, gold2,
    gold1, gold2, gold3, gold4,
    gold3, gold2, gold1, gold2 );
}

void SetupRandomPalette()
{
  // pick some random colors 
  CRGB color1 = CHSV(random8(150,200), random8(150,255), random8(180,255));
  CRGB color2 = CHSV(random8(0,70), random8(150,255), random8(180,255));
  CRGB color3 = CHSV(random8(20,80), random8(150,255), random8(180,255));

  // make the random palette
  currentPalette = CRGBPalette16( 
    color1, color1, color1, color2,
    color2, color2, color3, color3,
    color2, color1, color1, color2,
    color3, color3, color2, color2 );
}


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500);  // startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {
  // every few seconds choose a new palette
  EVERY_N_SECONDS(5) {

    // cycle through your palette choices in order
    //pal_choice++;
    //if (pal_choice == 4) {
    //  pal_choice = 0;  // reset to 0
    //}

    // choose a random palette
    pal_choice = random8(4);

    Serial.print("palette choice: "); Serial.println(pal_choice);
    
    if (pal_choice == 0) {
      currentPalette = RainbowColors_p;
    }
    if (pal_choice == 1) {
      SetupGoldenPalette();
    }
    if (pal_choice == 2) {
      currentPalette = LavaColors_p;
    }
    if (pal_choice == 3) {
      SetupRandomPalette();
    }
  }
  

  EVERY_N_MILLISECONDS(10) {
    colorIndex = startIndex;
    for( int i = 0; i < NUM_LEDS; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])
      leds[i] = ColorFromPalette( currentPalette, colorIndex, 255, LINEARBLEND);
      colorIndex = colorIndex + 10;  //how fast to advance through palette
    }

    FastLED.show();
    startIndex = startIndex + 1;  //how fast to move the palette down the strip
  }//end_every_n
  
}//end_main_loop

