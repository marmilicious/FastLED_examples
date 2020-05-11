//***************************************************************
// This example shows one way to run a pattern for roughly a few
// minutes, every 10 minutes, and then turn it off and wait
// another 10 minutes.  Global brightness was used to fade the
// display in and out to make things a bit smoother.
//
// It is not very time accurate since it uses millis and does
// not account for the fade in/out time.  Use a  RTC (real time
// clock) if accurate and specific clock times are needed, such
// as the DS3231.  https://www.adafruit.com/product/3013
//
// EVERY_N_SECONDS can be replaced with EVERY_N_MINUTES 
// or EVERY_N_HOURS as needed for the "wait" and "run" time
// sections.
//
//
// This example runs a slightly modified version of the Pacifica
// pattern by Mark Kriegsman.  Please see original code here:
//  https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino
//
//
// Marc Miller, May 2020
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

uint8_t patternState = 1;  // 0=waiting, 1=fadeUp, 2=running, 3=fadeDown
uint8_t brightness = 0;
unsigned long startTime;
unsigned long currentTime;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(2000); // 2 second delay for boot recovery, and a moment of silence
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection( TypicalLEDStrip );
  FastLED.setMaxPowerInVoltsAndMilliamps( 5, 2000);
  FastLED.setBrightness(brightness);
  FastLED.clear();
  FastLED.show();
  Serial.println("Setup done.  \n");
}
  

//---------------------------------------------------------------
void loop() {

  EVERY_N_MINUTES(10) {
    patternState = 1;  // Time to start running the pattern
    Serial.println("    Time to FadeUp...");
  }

  if (patternState == 1) {
    //fadeIn
    EVERY_N_MILLISECONDS(60) {
      brightness = brightness + 1;
      if (brightness == 255) {
        patternState = 2;  // time to run pattern for a bit
        startTime = millis();
        Serial.println("    FadeUp complete.\n    Running...");
      }
      FastLED.setBrightness(brightness);
    }
  }

  if (patternState == 2) {
    // run the pattern
    // 1 min * 60sec/min * 1000ms/sec = 60000 ms
    const uint32_t runTime = 60000;  // number of ms to run pattern
    currentTime = millis();
    if ( (currentTime - startTime) > runTime ) {
      patternState = 3;  // time to fade out
      Serial.println("    Time to FadeDown...");
    }
  }

  if (patternState == 3) {
    // fadeOut
    EVERY_N_MILLISECONDS(120) {
      brightness = brightness - 1;
      if (brightness == 0) {
        Serial.println("    FadeDown complete.\n  Waiting...");
        patternState = 0;  // back to waiting state
        FastLED.clear();
        FastLED.show();
      }
      FastLED.setBrightness(brightness);
    }    
  }
  
  if (patternState > 0) {
    // run the pattern
    EVERY_N_MILLISECONDS( 20) {
      pacifica_loop();
      FastLED.show();
    }
  }

  EVERY_N_MINUTES(1) {
    if (patternState == 0) {
      Serial.println("  Waiting...");
    }
  }
  
} //end_main_loop




//---------------------------------------------------------------
// Please see original Pacifica code by Mark Kriegsman here:
//  https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino

CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}
