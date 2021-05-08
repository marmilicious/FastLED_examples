//***************************************************************
// Simulated Television bounce lighting
//
// Simlulates the light coming from a TV at night, as if it
// was coming from another part of the room or through a door
// or window.
//
// Basics for the shifting matrix colors was based on a Mark
// Kreigsman example.
// http://pastebin.com/RQCPVyXf  
// Building on that example, I added all the randomness to
// vary the color change rate, saturation, and brightness.
//
//
// Marc Miller, April 2021
//***************************************************************


#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 128


const uint8_t kMatrixWidth  = 8;
const uint8_t kMatrixHeight = 4;
const bool    kMatrixSerpentineLayout = false;
uint8_t pixelHue, hueAdjusted;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

uint8_t sat = 160;  // overall base saturation
uint8_t satMax = 210;  // max saturation
uint8_t val = 255;  // starting value
float rate = 0.30;  // starting rate
uint8_t pos;
static boolean varyRate;  // used to switch things on/off
static boolean varyBrightness;
static boolean varySaturation;



//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1500);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("\nSetup done.\n");
}


//---------------------------------------------------------------
void loop() {

  EVERY_N_MILLISECONDS(100) {
    random16_add_entropy (analogRead(A0));
  }
  
  uint32_t ms = millis()*rate;  // This is what affects the speed (lower is slower)
  
  int32_t yHueDelta32 = ((int32_t)cos16( ms * 27 ) * (350 / kMatrixWidth));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * 39 ) * (310 / kMatrixHeight));
  DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);

  FastLED.show();

    
  EVERY_N_SECONDS_I(timingObj, 1) {
    uint8_t percent = random8(100);
    if (percent > 40) {
      varyBrightness = 1;
      Serial.println("Vary brightness is On.");
      timingObj.setPeriod(random8(1,11));
    } else {
      varyBrightness = 0;
      Serial.println("Vary brightness is Off.");
      timingObj.setPeriod(random8(10,26));
    }
  }

  EVERY_N_SECONDS_I(timingObj2, 5) {
    uint8_t percent = random8(100);
    if (percent > 35) {
      varySaturation = 1;
      Serial.println("\tVary saturation is On.");
      timingObj2.setPeriod(random8(2,5));
    } else {
      varySaturation = 0;
      Serial.print("\tVary saturation is Off.  ");
      Serial.println(sat);
      timingObj2.setPeriod(random8(3,9));
    }
  }

  EVERY_N_MILLISECONDS_I(timingObj3, 4200) {
    uint8_t num = random8(45);
    if (num > 37) {
      rate = float(num) / 100.0;
      varyRate = 1;
      Serial.print("\t\tVary RATE is ON.  ");
      Serial.println(rate);
      timingObj3.setPeriod(random16(500,1200));
    } else {
      num = random8(6,28);
      rate = float(num) / 100.0;
      varyRate = 0;
      Serial.print("\t\tVary rate is Off.  ");
      Serial.println(rate);
      timingObj3.setPeriod(random16(2500,8000));
    }
  }

}


//---------------------------------------------------------------
void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    pixelHue = lineStartHue;      
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;

      pos = XY(x, y);

      static uint8_t valX = val;
      if (varyBrightness) {
        valX = beatsin8(23, 150, 255);
        if (valX > 220) {
          valX = 255;
        }
      } else {
        valX = random8(val-50,val);
      }

      // extra desaturating for some colors
      if (pixelHue > 185 && pixelHue < 240) {
        if (sat > 210) {
          sat = sat - 100;
        }
      }
      if (pixelHue > 90 && pixelHue < 110) {
        if (sat > 220) {
          sat = sat - 60;
        }
      }
      
      leds[pos]  = CHSV( pixelHue, sat, valX);

      if (varySaturation) {
        CRGB color = leds[pos];
        uint8_t val = color.getLuma();
        CRGB mono = CRGB( val, val, val);
        uint8_t satMix = beatsin8(20, 120, 255);
        if (satMix > 220) {
          satMix = satMax;  // set to max saturation
        } else {
          satMix = sat;
        }
        leds[pos] = blend( color, mono, satMix); 
      }

    }
  }
}


//---------------------------------------------------------------
// Helper function that translates from x, y into an index into the LED array
// Handles both 'row order' and 'serpentine' pixel layouts.
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  } else {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}


//---------------------------------------------------------------
// TODO:
// - Play with the timing of things some more.
// - Make hot chocolate.
