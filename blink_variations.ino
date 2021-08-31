//***************************************************************
// Blink some pixels on and off.
// A variety of different ways to blink individual pixels.
// Something you might use for a beacon or indicator light or
// put in a scale model kit.
//
// Some are more straight forward then others.  Just having fun
// making up a variety of ways to blink a pixel.
//
// Be aware that pixel numbers are hard coded in these examples.
// Change as needed for your setup.
//
// Marc Miller, Aug 2021
//***************************************************************


#include "FastLED.h"
#define DATA_PIN    11
#define CLOCK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  128
CRGB leds[NUM_LEDS];


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output
  delay(1500); // startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.println("\nSetup done. \n");
}


//---------------------------------------------------------------
void loop() {


  EVERY_N_MILLISECONDS(1200) {  // change the state every 1200ms
    static boolean ps0;  // stores the pixel state
    ps0 = !ps0;  // toggle on/off state
    if (ps0 == 1) {
      leds[0] = CRGB::Yellow;
    } else {
      leds[0] = CRGB::Black;
    }
  }



  EVERY_N_MILLISECONDS(400) {
    // Is the pixel on? 
    if ( leds[1] ) {
      // yes, then turn it off
      leds[1] = CRGB(0,0,0);
    } else {
      // no, then turn it on
      leds[1] = CRGB::Green;
    }
  }



  // A blink with uneven on/off times
  EVERY_N_MILLISECONDS(333) {
    static boolean stateA = 0;
    static boolean stateB = 1;
    stateA = !stateA;
    if (stateA == 1) {
      stateB = !stateB;
    }
    if (stateA && stateB) {
      leds[2] = CHSV(210,220,255);
    } else {
      leds[2] = CHSV(192,255,140);
    }
  }



  // A three state blink
  EVERY_N_MILLISECONDS(250) {
    static uint8_t ps2;
    if (ps2 < 4) {
      leds[3] = CRGB::Black;
    } else {
      if ( ps2 % 2 == 0) {
        leds[3] = CRGB::Aqua;
      } else {
        leds[3] = CRGB::Blue;
      }
    }
    ps2++;
    if (ps2 == 9) { ps2 = 0; }  // reset
  }



  // Several experiments with variable timmers


  // This is a fun pattern that uses two pixels next to eachother.
  static boolean pMode = 0;
  static boolean flip = 0;
  EVERY_N_MILLISECONDS_I( timingA, 1) {
    // This initally defaults to 1 millisecond, but then will
    // imediately use the timingA.setPeriod times below.
    // You can name "timingA" anything you want.
    pMode = !pMode;
    if (pMode) {
      timingA.setPeriod(450);
    } else {
      timingA.setPeriod(1400);
    }
  }
  EVERY_N_MILLISECONDS(40) {
    if (pMode) {
      flip = !flip;
      if (flip) {
        leds[4] = CHSV(55,160,200);
        leds[5] = CHSV(0,0,0);
      } else {
        leds[4] = CHSV(0,0,0);
        leds[5] = CHSV(55,160,200);
      }
    } else {
      leds[4] = CHSV(0,0,0);
      leds[5] = CHSV(0,0,0);
    }
  }



  static boolean blink6;
  // vary the setPeriods to change on/off times
  EVERY_N_MILLISECONDS_I(timingB,1) {
    blink6 = !blink6;
  }
  if (blink6 == 1) {
    leds[6] = CRGB::Red;
    timingB.setPeriod(750);  // On time
  } else {
    leds[6] = CRGB::Black;
    timingB.setPeriod(250);  // Off time
  }



  // blink with a fadeout
  static boolean pulse = 0;
  EVERY_N_MILLISECONDS_I( timingFade, 1) {
    pulse = !pulse;
    if (pulse) {
      timingFade.setPeriod(700);  // time to hold before fading
      leds[7] = CHSV(40, 245, 255);
    } else {
      timingFade.setPeriod(1000);
    }
  }
  if (pulse == 0) {
    EVERY_N_MILLISECONDS(10) {
      leds[7].fadeToBlackBy(12);  // fade out
    }
  }



  // This one is probably overly complicated, but it came
  // from an older project where I needed to make a very
  // specific blink pattern.
  static boolean blinkState = LOW;
  static boolean blinkGate1 = LOW;
  static boolean blinkGate2 = HIGH;
  static int8_t blinkCount = -1;
  EVERY_N_MILLISECONDS_I(timingDbl, 500) {
    blinkCount++;
    if (blinkCount == 6) { blinkCount = 0; }  // reset count
    blinkGate2 = blinkCount;
    blinkState = !blinkState;
    blinkGate1 = !blinkGate1;
    if (blinkCount == 2 || blinkCount == 3) {
      timingDbl.setPeriod( 50 );
    } else if (blinkCount == 4) {
      timingDbl.setPeriod( 405 );
    } else {
      timingDbl.setPeriod( 167 );
    }
    // leds8 is the original blink pattern
    // leds9 is an interesting alternate version
    if (blinkState * blinkGate1 * blinkGate2) {
      leds[8] = CHSV(0,230,200);  // The original red blink
      leds[9] = CRGB::Black;
    } else {
      leds[8] = CHSV(0,0,0);
      leds[9] = CHSV(0,0,200);
    }
  }
  if (blinkCount == 5) {
    leds[9] = CHSV(0,0,0);
  }



  
  // Driving on/off with wave functions

  // Blinks with some ramping. Change bpm to change speed
  uint8_t valueBS = beatsin8(60,0,255);  // (bpm, low, high)
  if (valueBS > 50) {
    leds[10] = CHSV(100,170,valueBS);
  } else {
    leds[10] = CRGB::Black;
  }
  if (valueBS > 180) {
    leds[11] = CHSV(180,65,valueBS);
  } else {
    leds[11] = CRGB::Black;
  }



  uint8_t valueTW = triwave8(millis()/4);  // Divide millis by more or less to change speed.
  if (valueTW > 30) {  // The pixel will mostly be On.
    leds[12] = CHSV(150,255,255);
  } else {
    leds[12] = CRGB::Black;
  }
  if (valueTW > 230) {  // An inverted version where pixel will mostly be Off.
    leds[13] = CRGB::Yellow;
  } else {
    leds[13] = CRGB::Black;
  }



  // This was pleasingly simple.  Divide millis by more or less to change speed.
  uint8_t valueSW = squarewave8(millis()/6, 128);  // (input, pulsewidth[0-255])
  leds[14] = CHSV(120,255,valueSW);



  // This blink speeds up and then slows back down.
  static boolean blinkUp;
  const uint16_t bSlow = 350;
  const uint16_t bFast = 50;
  uint16_t bCurrent = beatsin16(5,0,65535);
  bCurrent = map(bCurrent,0,65535,bFast,bSlow);
  EVERY_N_MILLISECONDS_I(timingUp,bSlow) {
    blinkUp = !blinkUp;
    timingUp.setPeriod(bCurrent);
  }
  if (blinkUp == 1) {
    leds[15] = CHSV(160,145,200);
  } else {
    leds[15] = CRGB::Black;
  }


  
  FastLED.show();

  delay(1);  // this can be deleted as needed
}
