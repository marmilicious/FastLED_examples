// Test of chasing random target values, version A.
//
// Random target values are generated and assigned to random pixels.
// Pixel brightness is then increased or decreased toward the target.
//
// This version only varies the red channel.
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

int target[NUM_LEDS];  // Place to save some target values.
int count;
int delta;
int temp;
int lowCutoff = 50;


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
    if (leds[i].r < target[i]){  // less then the target, so fade up.
      delta = (target[i] - leds[i].r) / 5;
      if (leds[i].r + delta >= target[i]){
        leds[i].r = target[i];  // limit to target.
      }
      else { leds[i].r += delta; }
    }

    else {  // greater then the target, so fade down.
      delta = ((leds[i].r - target[i])/8) + 1;  // +1 so delta is always at least 1.
      if (leds[i].r - delta <= target[i]){
        leds[i].r = target[i];  // limit to target.
      }
      else { leds[i].r -= delta; }
    }
    if (i == 0){ temp = delta; }  // Save first pixel's delta number to print below.


    // Continously fade target to zero.
    if (target[i] > lowCutoff){
      target[i] -= 2;  // Fade by this ammount.
    }
    else { target[i] = 0; }  // If target is less then lowCutoff, clamp to zero.


  }//end of looping over pixels.
  
  FastLED.show();  // Display the leds[] array.

  // Print out info for led[0] to follow the numbers.
  Serial.print("c: "); Serial.print(count);
  Serial.print("\t target: "); Serial.print(target[0]);
  Serial.print("\t  led[0].r: "); Serial.print(leds[0].r);
  Serial.print("\t  delta: "); Serial.println(temp);
 
  count--;  // reduce count by one.
  delay(10);
  
}//end main loop
