//***************************************************************
// Three moving pixels example
//     Uses modulo, %, to make pixel position "loop" around and
//     stay in valid pixel range.
//
// Marc Miller, Feb 2015.  Updated Jan 2016.
//***************************************************************


#include "FastLED.h"
#define LED_TYPE NEOPIXEL  // Strip type: NEOPIXEL, APA102, LPD8806, etc.
#define DATA_PIN 6
//#define CLOCK_PIN 13
#define NUM_LEDS 12
//#define COLOR_ORDER BGR
#define MASTER_BRIGHTNESS 100  // Master brightness (Range is 0-255)
CRGB leds[NUM_LEDS];

int16_t positionRed = 0;    // Set initial start position of Red pixel
int16_t positionWhite = 4;  // Set initial start position of White pixel
int16_t positionBlue = 8;   // Set initial start position of Blue pixel
int8_t delta = 1;           // Using a negative value will move pixels backwards.
uint16_t holdTime = 1000;   // Milliseconds to hold position before advancing


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(3000);  // Startup delay
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN>,COLOR_ORDER,(leds, NUM_LEDS);  // ***For strips using Clock.
  FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(MASTER_BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {  // START MAIN LOOP

  EVERY_N_MILLISECONDS(holdTime) {

    // Set pixel color
    leds[positionRed] = CRGB::Red;
    leds[positionWhite] = CRGB::White;
    leds[positionBlue] = CRGB::Blue;
  
    // Show the pixels
    FastLED.show();
    //delay(holdTime);  // Delay for a bit.
   
    // Set pixels back to Black for the next loop around.
    leds[positionRed] = CRGB::Black;
    leds[positionWhite] = CRGB::Black;
    leds[positionBlue] = CRGB::Black;
  
    // Set new position, moving (forward or backward) by delta.
    // NUM_LEDS is added to the position before doing the modulo
    // to cover cases where delta is a negative value.
    positionRed = (positionRed + delta + NUM_LEDS) % NUM_LEDS;
    positionWhite = (positionWhite + delta + NUM_LEDS) % NUM_LEDS;
    positionBlue = (positionBlue + delta + NUM_LEDS) % NUM_LEDS;

    // Print out position values to see what's happening.
    //Serial.print("pos R = "); Serial.print(positionRed);
    //Serial.print("\t  pos W = "); Serial.print(positionWhite);
    //Serial.print("\t  pos B = "); Serial.println(positionBlue);
  }

} // END MAIN LOOP
