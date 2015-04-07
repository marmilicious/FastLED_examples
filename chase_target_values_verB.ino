// Test of chasing random target values, version B.
//
// Random target values are generated and assigned to random pixels.
// Pixel brightness is then increased or decreased toward the target.
//
// This version incorporates a hue shift based on the pixel value.
// The high and low color choices can be changed below in the variables.
// Saturation is kept at full, but it could also easily be tweaked.
//
// Marc Miller,  April 2015
//---------------------------------------------------------------

#include "FastLED.h"
#define LED_TYPE NEOPIXEL  // *Update to your strip type.  NEOPIXEL, APA102, LPD8806, etc..
#define DATA_PIN 6         // *Set this to your data pin.
#define NUM_LEDS 12        // *Update to the number of pixels in your strip.
//#define COLOR_ORDER BGR
#define MASTER_BRIGHTNESS 255

CRGB leds[NUM_LEDS];
CHSV leds_vu[NUM_LEDS];  // FastLED array using HSV.

int target[NUM_LEDS];  // Array to store target brightness values.
int count;
int delta;
int temp;
int highCutoff = 200;
int lowCutoff = 30;

// ***** A few color combinations to try out: *****
int hue_high = 0;  // red for high values with,
int hue_low = 96;  // green for low values.
    // or
//int hue_high = 96;  // green for high values with,
//int hue_low = 192;  // purple for low values.
    // or
//int hue_high = 84;  // yellow-green for high values.
//int hue_low = 56;  // orange for low values.


//---------------------------------------------------------------
void setup() { 
  delay(1500);
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  Serial.begin(115200);  // Allows serial output (check baud rate)
}

//---------------------------------------------------------------
void loop() { 
  // Assign random target values whenever count is zero.
  if (count == 0){
    for (int i=0; i < NUM_LEDS; i++){
      target[random8(NUM_LEDS)] = random8();  // Pick random pixels, assign random values.
      if (target[i] < lowCutoff){
        target[i] = 0;  // Force low values to clamp to zero.
      }
    }
    count = random8(35,70);  // Pick a new count value.
    Serial.println("*New targets assigned!*");
  }


  for (int i=0; i < NUM_LEDS; i++){
    // Check current values against target values.
    if (leds_vu[i].value < target[i]){  // less then the target, so fade up.
      delta = (leds_vu[i].value - target[i]) / 5;
      if (leds_vu[i].value + delta >= target[i]){
        leds_vu[i].value = target[i];  // limit to target.
      }
      else {
        leds_vu[i].value += delta;
      }
    }

    else {  // greater then the target, so fade down.
      delta = ((leds_vu[i].value - target[i])/8) + 1;  // +1 so delta is always at least 1.
      if (leds_vu[i].value - delta <= target[i]){
        leds_vu[i].value = target[i];  // limit to target.
      }
      else {
        leds_vu[i].value -= delta;
      }
    }
    if (i == 0){ temp = delta; }  // Save first pixel's delta number to print below.

    // Tweak hue color based on brightness.
    int c_hue = constrain(leds_vu[i].value,lowCutoff,highCutoff);
    leds_vu[i].hue = map(c_hue, lowCutoff, highCutoff, hue_low, hue_high);
                  // map(valueIn, fromLow,   fromHigh,   toLow,   toHigh)

    leds_vu[i].saturation = 255;  // Set saturation to full.

    // Copy the HSV leds_vu[] data to the leds[] array.
    leds[i] = leds_vu[i];
      // FastLED will automatically convert HSV to RGB data.  Converting from HSV
      // to RGB is very fast and also accurate.  It is possible to convert from
      // RGB to HSV, but it is not automatic, not totally acurate, and not as fast.

    // Continously fade target to zero.
    if (target[i] > lowCutoff){
      target[i] -= 2;  // Fade by this ammount.
    }
    else {
      target[i] = 0;  // If target less then lowCutoff, clamp to zero.
    }

  }//end of looping over pixels.
  
  FastLED.show();  // Display the leds[] array.

  // Print out info for led[0] to understand how the numbers are changing.
  Serial.print("c: "); Serial.print(count);
  Serial.print("\t target: "); Serial.print(target[0]);
  Serial.print("\t  leds_vu[0].hue: "); Serial.print(leds_vu[0].hue);
  Serial.print("\t  leds_vu[0].value: "); Serial.print(leds_vu[0].value);
  Serial.print("\t  delta: "); Serial.println(temp);
 
  count--;  // reduce count by one.
  delay(10);
  
}//end main loop
