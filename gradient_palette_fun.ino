//***************************************************************
// Messing with gradient palettes.
// Fills strip with full palette, then randomly picks a pixel
// and pulses it to a random color from the palette, and then
// returns the pixel back to it's start color.
//
// Uses Mark Kriegsman's fadeTowardColor function from:
// https://gist.github.com/kriegsman/d0a5ed3c8f38c64adcb4837dafb6e690
//
// Marc Miller, April 2020
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define COLOR_ORDER GRB
#define BRIGHTNESS 255

#define NUM_LEDS 32
CRGB leds[NUM_LEDS];

bool nextPalette = false;  // trigger for advancing to next palette index
uint8_t gIndex = 0;  // startup palette index number (in gPal)
static uint8_t randomIndex;
static uint8_t randomPixel;
CRGB randomColor;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


//---------------------------------------------------------------

DEFINE_GRADIENT_PALETTE( A_gp ) {
  0,   5,  32, 68,
  56,   9, 28, 44,
  119, 69, 22, 12,
  153, 129, 62, 44,
  188, 232, 108, 97,
  255, 203, 98, 114
};

DEFINE_GRADIENT_PALETTE( B_gp ) {
  0,   7, 77, 190,
  31,  21, 112, 216,
  63,  53, 149, 207,
  95,  90, 160, 172,
  127, 123, 180, 192,
  159, 186, 186, 127,
  191, 167, 159, 50,
  223, 125, 117, 74,
  255, 55, 72,  92
};

DEFINE_GRADIENT_PALETTE( C_gp ) {
  0,  10, 199, 26,
  73, 205, 255,  8,
  255, 255, 11, 71
};

DEFINE_GRADIENT_PALETTE( D_gp ) {
  0, 123, 119, 99,
  65, 101, 50, 70,
  107, 81, 31, 24,
  158, 103, 23,  5,
  183, 149, 65,  22,
  214, 205, 185, 90,
  255, 245, 214, 150
};

DEFINE_GRADIENT_PALETTE( E_gp ) {
  0,  18,  7, 230,
  255, 224, 10,  9
};

DEFINE_GRADIENT_PALETTE( F_gp ) {
  0,   0, 33, 255,
  127,  42, 33, 45,
  240, 225, 33,  15,
  255, 255, 33,  0
};

DEFINE_GRADIENT_PALETTE( G_gp ) {
  0, 255,  0, 255,
  33, 255,  2, 178,
  68, 255, 13, 115,
  102, 255, 33, 71,
  135, 255, 62, 37,
  170, 255, 104, 15,
  204, 255, 156,  4,
  237, 255, 217,  1,
  255, 255, 255,  0
};

DEFINE_GRADIENT_PALETTE( H_gp ) {
  0,  16, 82, 255,
  63,  91, 156, 103,
  127, 255, 255, 25,
  191, 255, 75, 18,
  255, 255,  3, 12
};

DEFINE_GRADIENT_PALETTE( I_gp ) {
  0,   0,  22, 80,
  63,  30,  11, 34,
  127, 100,  1,  2,
  191, 157, 66,  1,
  255, 200, 255,  0
};

DEFINE_GRADIENT_PALETTE( J_gp ) {
  0,  10, 62, 123,
  36, 56, 130, 103,
  87, 153, 225, 85,
  100, 199, 217, 68,
  107, 255, 207, 54,
  115, 247, 152, 57,
  255, 229, 233, 42
};

DEFINE_GRADIENT_PALETTE( K_gp ) {
  0, 255, 164, 49,
  40, 200, 141, 72,
  87, 145, 111, 135,
  178, 33, 41, 72,
  255,   1, 10, 22
};

CRGBPalette16 gPal[] = {
  A_gp,
  B_gp,
  C_gp,
  D_gp,
  E_gp,
  F_gp,
  G_gp,
  H_gp,
  I_gp,
  J_gp,
  K_gp,
};


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(3000);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  // blank out pixels
  FastLED.show(); 
  Serial.print("Number of palettes in gPal: "); Serial.println(ARRAY_SIZE(gPal));
  Serial.print("Startup gIndex: "); Serial.println(gIndex);
  Serial.println("Setup done.\n\n");
}


//---------------------------------------------------------------
void loop() {
  EVERY_N_SECONDS(20) {
    // Pick a new palette.
    // This Every N timer section could be replaced with a button or other trigger
    nextPalette = true;
    Serial.println("Time for next palette...");
  }

  if (nextPalette == true) {
    // Next palette has been triggered.  Pick a new random palette in gPal and fill strip.
    gIndex = random8(ARRAY_SIZE(gPal)); //gIndex + 1;  // go to next palette
    Serial.print("  New gIndex: "); Serial.println(gIndex);
    Serial.println("");
    nextPalette = false;  // reset trigger
    fill_palette(leds, NUM_LEDS, 0, (255/NUM_LEDS-1), gPal[gIndex], 255, LINEARBLEND);
  }


  EVERY_N_MILLISECONDS(500) {
    // Every so often pick a random location on the strip and assign colors from the current palette
    random16_add_entropy(random16() + analogRead(A0));
    randomPixel = random8(0,NUM_LEDS-2);  // pick a random pixel (0 to NUM_LED-2)

    //randomIndex = random8(0,254);  // pick random index (0-253) from current palette
    randomIndex = map(randomPixel,0,NUM_LEDS-2,0,253) + random8(108,148);  // random index + random shifted position

    // Assign pixel data to three consecutive pixels
    randomColor = ColorFromPalette( gPal[gIndex], randomIndex ,255, LINEARBLEND);
    leds[randomPixel] = nblend( leds[randomPixel], randomColor, 140);
    
    randomColor = ColorFromPalette( gPal[gIndex], randomIndex+1 ,255, LINEARBLEND);
    leds[randomPixel+1] = blend( leds[randomPixel+1], randomColor, 255);
    leds[randomPixel+1] += CRGB(30,30,30);  // boost brightness

    randomColor = ColorFromPalette( gPal[gIndex], randomIndex+2 ,255, LINEARBLEND);
    leds[randomPixel+2] = blend( leds[randomPixel+2], randomColor, 140);
  }


  EVERY_N_MILLISECONDS(15) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      CRGB curColor = leds[i];  // grab current color 
      CRGB palColor = ColorFromPalette( gPal[gIndex], i*(255/NUM_LEDS-1) );
      leds[i] = fadeTowardColor(curColor, palColor, 2);
    }
  }

  FastLED.show();

}//end_main_loop



//---------------------------------------------------------------
// Function from Mark's example...
//
// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
CRGB fadeTowardColor( CRGB& cur, CRGB& target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
  return cur;
}
// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
 
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
  }
}

