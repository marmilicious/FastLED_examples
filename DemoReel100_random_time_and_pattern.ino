//***************************************************************
//  This is an modified version of the FastLED example file
//  DemoReel100.ino with these two main changes.
//  1) The cycle time to go to the next pattern is now random,
//     and
//  2) The next pattern to display is randomly picked from the
//     pattern list instead of linearly going through the list.
//
// Mark Kriegsman's original example here:
// https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino
//
//
//  Marc Miller, March 2024
//***************************************************************

#include "FastLED.h"
#define DATA_PIN 11
#define CLOCK_PIN 13
#define LED_TYPE LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS 32
uint8_t BRIGHTNESS = 128;
CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND  120

uint16_t cycleTime = 20;  // Number of seconds before next pattern. [random]

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

#define ARRAY_SIZE(A) ( sizeof(A) / sizeof((A)[0]) )

#define floatingPin 14  // an ANALOG pin with Nothing connected to it (used for random number generation)


//===============================================================
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  pinMode(floatingPin, INPUT);  // set analog pin as an input (used for random number generation)

  Serial.println(F("Setup done.\n"));  
}


//---------------------------------------------------------------
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();

SimplePatternList gPatterns = {
  rainbow,
  rainbowWithGlitter,
  confetti,
  sinelon,
  juggle,
  juggle, juggle, juggle, juggle, juggle, juggle,
  bpm
};

// Do you like one pattern more then others? If so you can add it
// in the above list multiple times like "juggle" above. This way
// it will randomly be picked more often.


//===============================================================
void loop()
{
  EVERY_N_MILLISECONDS_I(timingEntropy,1) {
    for (uint8_t i = 0; i < 32; i++) {
      random16_add_entropy (analogRead(floatingPin));  // randomize the random  :o
      timingEntropy.setPeriod(667);  // set time to get new randomness
    }
  }

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 


  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

  // not using this next line anymore. Use below for random pattern change and timing
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  EVERY_N_SECONDS_I( timingPattern, 1) {
    nextPattern();  // Change patterns periodically
    timingPattern.setPeriod(cycleTime);  // set time to run pattern
  }

}//end main void loop



//---------------------------------------------------------------
void nextPattern()
{
  // Not using this anymore
  // add one to the current pattern number, and wrap around at the end
  //gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);

  // the above line has been replaced with this to randomly choose a pattern
  gCurrentPatternNumber = random8( ARRAY_SIZE(gPatterns) );  // pick a random pattern

  // this added to randomly choose how long until the next pattern change.
  cycleTime = random16(15,46);  // random number from 15 to 45 [seconds]

  Serial.print("gCurrentPatternNumber = "); Serial.print(gCurrentPatternNumber);
  Serial.print("\t cycleTime = "); Serial.println(cycleTime);
}


//===============================================================
// All the patterns to choose from

//---------------------------------------------------------------
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

//---------------------------------------------------------------
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

//---------------------------------------------------------------
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

//---------------------------------------------------------------
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

//---------------------------------------------------------------
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

//---------------------------------------------------------------
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}



