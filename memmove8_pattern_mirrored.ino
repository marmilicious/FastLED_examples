//***************************************************************
// Messing around based on someone's question about reversing
// the order of a range of pixels.
// Builds on my memmove8_pattern_copy.ino example.  Please see
// that sketch for more info and comments.
//
// Marc Miller, April 2020
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 100
#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

const uint8_t patternMinLength = 3;
const uint8_t patternMaxLength = 12;
CRGB mirroredLeds[patternMaxLength];

uint8_t hue;

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

  repeatingPatternMirrored();
  FastLED.show();
  
}//end_main_loop


//---------------------------------------------------------------
void repeatingPatternMirrored() {
  static uint8_t patternLength = 4;  // This changes [random number between pattern Min and Max length]
  static uint8_t pRepeat = NUM_LEDS / patternLength;
  static uint8_t pDiff = NUM_LEDS % patternLength;

  EVERY_N_SECONDS(10) {
    random16_add_entropy(random16() + analogRead(A0));
    patternLength = random8(patternMinLength,patternMaxLength+1);
    pRepeat = NUM_LEDS / patternLength;
    pDiff = NUM_LEDS % patternLength;
    Serial.print("patternLength: "); Serial.println(patternLength);
    hue = hue + random8(100,160);
  }

  fill_rainbow( leds, patternLength, hue + (millis()/40), 10 );
  leds[0] += CHSV(hue+128,255,190);  // optional visual to help see repeat of pattern
  memmove8( &mirroredLeds[0], &leds[0], (patternLength)*sizeof(CRGB) );  // copy pattern to mirroredLeds array

  // mirror pixels in CRGB mirroredLeds array
  for (uint8_t i=0; i<patternLength/2; i++) {
    CRGB temp = mirroredLeds[i];
    mirroredLeds[i] = mirroredLeds[patternLength-1-i];
    mirroredLeds[patternLength-1-i] = temp;
  }
  
  for (uint8_t i=0; i < pRepeat; i++) {
    if ( mod8(i,2) == 0) {
      memmove8( &leds[i*patternLength], &leds[0], (patternLength)*sizeof(CRGB) );
    } else {
      memmove8( &leds[i*patternLength], &mirroredLeds[0], (patternLength)*sizeof(CRGB) );
    }
  }

  if (pDiff != 0) {  // copy the remaining pixels
    if ( mod8(pRepeat,2) == 0) {
      memmove8( &leds[NUM_LEDS - pDiff], &leds[0], (pDiff)*sizeof(CRGB) );
    } else {
      memmove8( &leds[NUM_LEDS - pDiff], &mirroredLeds[0], (pDiff)*sizeof(CRGB) );
    }
  }

}//end_CopyPixels

