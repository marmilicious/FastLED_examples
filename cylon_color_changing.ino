//***************************************************************
// This is a Cylon effect that doesn't use delay().  The color
// of the moving dot is picked from a set of custom colors
// stored in an array.
//
// There are two different examples in the main loop.
// Un-comment one at a time to try each out.  One changes color
// based on a timer, and the other changes color whenever the
// dot reaches the end of the stirp.
// The color change could also be based on something such as
// a potentiometer, distance sensor, button, sine wave, random, etc.
//
// Marc Miller, Nov 2018
//              Dec 2023 - updated color timing, fade rates
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];


// Custom colors to choose from
CRGB colorArray[] = {
  CRGB(0,255,0),    //green
  CRGB(180,180,180),  //a dim white
  CHSV(0,255,255),  //red
  CRGB::Grey,
};

uint8_t numberOfColors = sizeof(colorArray) / sizeof(colorArray[0]);
uint8_t colorChoice;  //the current color choice
uint8_t pos;  //dot position


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(2000);  // Startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);  // ***For strips using Clock.
  //FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {

  //Comment or un-comment one of these examples to try each out.
  //cylon_cc1();
  cylon_cc2();
  
  FastLED.show();
  
}//end_main_loop



//---------------------------------------------------------------
void colorChange(boolean r) {
  // Change the color.
  // If r = 0, the next color in the array is picked and then
  // it cycles back around to the first color in the array.
  // If r = 1 then a random color is picked from the array.

  if (r == 0) {
    colorChoice = colorChoice + 1;
    if (colorChoice >= numberOfColors) { colorChoice = 0; }
    //Serial.print("cycling: "); Serial.println(colorChoice);
  } else {
    colorChoice = random8(numberOfColors);
    //Serial.print("random: "); Serial.println(colorChoice);
  }
  
}


//---------------------------------------------------------------
void cylon_cc1() {
  //cylon effect with color change based on time
  
  static uint8_t delta = 1;  //direction of travel

  EVERY_N_MILLISECONDS(450) {
    colorChange(0);  //set next color
  }

  EVERY_N_MILLISECONDS(60) {
    fadeToBlackBy(leds, NUM_LEDS, 15);
    leds[pos] = colorArray[colorChoice];
    pos = pos + delta;
    if (pos == NUM_LEDS-1) { delta = -1; }  //change direction
    if (pos == 0) { delta = 1; }  //change direction
  }

}//end_cylon_cc1


//---------------------------------------------------------------
void cylon_cc2() {
  //cylon effect with color change when end of strip is reached
  
  static uint8_t delta = 1;  //direction of travel

  EVERY_N_MILLISECONDS(60) {
    fadeToBlackBy(leds, NUM_LEDS, 15);
    leds[pos] = colorArray[colorChoice];
    pos = pos + delta;
    if (pos == NUM_LEDS-1) {
      delta = -1;  //change direction
      colorChange(1);  //random next color
    }
    if (pos == 0) {
      delta = 1;  //change direction
      colorChange(1);  //random next color
    }
  }

}//end_cylon_cc2

