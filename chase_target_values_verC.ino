// Test of chasing random target values, version C.  (Slowed down)
//
// Random target values are generated and assigned to random pixels.
// Pixel brightness is then increased or decreased toward the target.
//
// This version incorporates a hue shift based on the pixel value.
// The high and low color choices can be changed below in the variables.
// Saturation is kept at full, but it could also easily be tweaked.
//
//
// Marc Miller,  Jan 2016
//---------------------------------------------------------------


#include "FastLED.h"
#define LED_TYPE LPD8806  // *Update to your strip type.  NEOPIXEL, APA102, LPD8806, etc..
#define COLOR_ORDER GRB
#define DATA_PIN 11         // *Set this to your data pin.
#define CLOCK_PIN 13
#define NUM_LEDS 32        // *Update to the number of pixels in your strip.

#define MASTER_BRIGHTNESS 140

CRGB leds[NUM_LEDS];
CHSV leds_vu[NUM_LEDS];  // FastLED array using HSV.

uint8_t target[NUM_LEDS];  // Array to store target (brightness) values.
uint8_t delta;  // Amount to increase/decrease (brightness) value by.
uint8_t pixel;  // Which pixel to operate on.
uint8_t temp;  // Used for testing.
uint8_t highCutoff = 230;  // High value where hue ramp ends.
uint8_t lowCutoff = 20;  // Low value cutoff before flooring to zero.
uint8_t ccc = 0;  // color combination choice (ccc)

// Initial color combinations choice (ccc = 0)
uint8_t hue_high = 0;  // red for high values with,
uint8_t hue_low = 96;  // green for low values.


//---------------------------------------------------------------
void setup() { 
  Serial.begin(115200);  // Allows serial output (check baud rate)
  delay(3000);  // 3 second delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() { 
  // Assign new random target values after a bit.
  EVERY_N_MILLISECONDS(2000) {
    for (int i=0; i < ( NUM_LEDS / 2 ); i++){  // Only do about half the pixels.
      pixel = random8(NUM_LEDS);  // Pick random pixel.
      target[pixel] = random8();  // Assign random value.
      while (target[pixel] > 90 && target[pixel] < 160) {  // Avoid some middle values, pick again.
        target[pixel] = random8();  // Assign another random value.
      }
      if (target[pixel] < lowCutoff){
        target[pixel] = 0;  // Force low values to clamp to zero.
      }
    }
    Serial.println("*New targets assigned!*");
  }//end assign new target values


  EVERY_N_MILLISECONDS(50) {
    for (uint8_t i=0; i < NUM_LEDS; i++){
      // Check current values against target values.
      if (leds_vu[i].value < target[i]){  // less then the target, so fade up.
        delta = ((target[i] - leds_vu[i].value) / 8) + 1;
        if (leds_vu[i].value + delta >= target[i]){
          leds_vu[i].value = target[i];  // limit to target.
        }
        else {
          leds_vu[i].value = leds_vu[i].value + delta;
        }
      }
      else {  // greater then the target, so fade down.
        delta = ((leds_vu[i].value - target[i]) / 8) + 1;  // +1 so delta is always at least 1.
        if (leds_vu[i].value - delta <= target[i]){
          leds_vu[i].value = target[i];  // limit to target.
        }
        else {
          leds_vu[i].value = leds_vu[i].value - delta;
        }
      }
      if (i == 0){ temp = delta; }  // Save first pixel's delta to print below.
  
      // Tweak hue color based on brightness.
      int c_hue = constrain(leds_vu[i].value,lowCutoff,highCutoff);
      leds_vu[i].hue = map(c_hue, lowCutoff, highCutoff, hue_low, hue_high);
                    // map(valueIn, fromLow,   fromHigh,   toLow,   toHigh)
  
      // Set saturation.
      leds_vu[i].saturation = 255;
  
      // Copy the HSV leds_vu[] data to the leds[] array.
      leds[i] = leds_vu[i];
        // FastLED will automatically convert HSV to RGB data.  Converting from HSV
        // to RGB is very fast and also accurate.  It is also possible to convert from
        // RGB to HSV, but it is not automatic, not as acurate, and not as fast.
  
    }//end of looping over all the pixels checking targets.
  }//end EVERY_N*

  // Continuosly fade target to zero.
  EVERY_N_MILLISECONDS(20) {
    for (int j=0; j < NUM_LEDS; j++){
      if (target[j] > lowCutoff){
        target[j] -= 1;  // Fade by this amount.
      }
      else {
        target[j] = 0;  // If target less then lowCutoff, clamp to zero.
      }
    }
  }//end of continuously fading target down.

  // Print out info for led[0] to understand how the numbers are changing.
  EVERY_N_MILLISECONDS(50) {
    //Serial.print("  target = "); Serial.print(target[0]);
    ////Serial.print("\t  leds_vu[0].hue = "); Serial.print(leds_vu[0].hue);
    //Serial.print("\t  leds_vu[0].val = "); Serial.print(leds_vu[0].value);
    //Serial.print("\t  delta = "); Serial.println(temp);
  } 

  FastLED.show();  // Display the leds[] array.

  EVERY_N_SECONDS(10) {  // Pick a new color combination choice (ccc).
    ccc =  random(4);
    if (ccc == 0) {
      hue_high = 0;  // red for high values
      hue_low = 96;  // green for low values
    }
    if (ccc == 1) {
      hue_high = 96;  // green for high values
      hue_low = 192;  // purple for low values
    }
    if (ccc == 2) {
      hue_high = 105;  // green for high values
      hue_low = 15;  // red for low values
    }
    if (ccc == 3) {
      hue_high = 82;  // yellow-green for high values
      hue_low = 140;  // aqua for low values
    }
    Serial.print("*Color combination choice done!*  ccc: "); Serial.println(ccc);
  }//end EVERY_N* ccc

}//end main loop

