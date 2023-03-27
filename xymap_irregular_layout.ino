//***************************************************************
// xymap example
// Original mapping concept for irregular layout by focalintent.
//
// Useful for mapping in a matrix like way across irregular
// shapped layouts, or layouts that have very large gaps
// between areas/blocks of pixels and you don't want to
// make a massive virtual matrix with lots of null pixels.
//
// This example has a diamond shapped layout like this:
// https://imgur.com/R3KkCOM
// Rather then make a virtual width * height matrix of
// 11x11 (121 pixels) to hold all the pysical pixels, an
// array is used to store only the individual 20 pixel
// positions.
//
// Wokwi link:  https://wokwi.com/projects/357297885896056833
//
//
// Marc Miller, Feb 2023
//***************************************************************

#include <FastLED.h>
#define DATA_PIN    11
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  255


#define NUM_LEDS 20
CRGB leds[NUM_LEDS];

struct PT { uint8_t x, y;};

// The number of xy pairs here needs to be equal to NUM_LEDS
PT xy[] = { {5,0}, {6,1}, {7,2}, {8,3}, {9,4}, {10,5}, {9,6}, {8,7}, {7,8}, {6,9}, {5,10}, {4,9}, {3,8}, {2,7}, {1,6}, {0,5}, {1,4}, {2,3}, {3,2}, {4,1} };
#define gridWidth 11
#define gridHeight 11

static uint8_t hue, sat;


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1500); // startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done. \n");
}


//---------------------------------------------------------------
void loop() {

  // Was used to test/check pixel layout
  // static uint8_t i;
  // EVERY_N_MILLISECONDS(300) {
  //   leds[i++ % NUM_LEDS] = CHSV(hue,128,255);
  //   FastLED.show();
  //   hue = hue + 15;
  // }

  EVERY_N_MILLISECONDS(33) {
    hue++;
  }

  static uint8_t Xpos;
  EVERY_N_MILLISECONDS(160) {
    Xpos++;
    if (Xpos == 5*gridWidth) {Xpos = 0;}
  }


  for(int i = 0; i < NUM_LEDS; i++) {
    static uint8_t mappedHue;
    mappedHue = map( xy[i].y, 0, gridHeight, hue, hue+160 );

    sat = 255;
    if (xy[i].x == Xpos-1 || xy[i].x == Xpos+1) {
      sat = 90;
    }
    if (xy[i].x == Xpos) {
      sat = 40;
    }

    leds[i] = CHSV( mappedHue, sat, 255 ) ;
  }

  FastLED.show();

}
