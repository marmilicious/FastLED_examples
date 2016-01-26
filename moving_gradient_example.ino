//***************************************************************
// Moving gradient example
// It will wrap around, and the direction can be reversed.
// This is done a bit different then you might expect as I'm
// filling the gradient once in setup() and saving the colors
// in a seperate CRGB array.  Then coping those into the main
// leds array as needed when moving it around.
//
// Marc Miller, Jan 2016
//***************************************************************

#include "FastLED.h"
#define LED_TYPE LPD8806
#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS 32
#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB leds[NUM_LEDS];

// User variables
CHSV gradStartColor(0,255,255);  // Gradient start color.
CHSV gradEndColor(161,255,255);  // Gradient end color.
uint8_t gradStartPos = 0;  // Starting position of the gradient.
#define gradLength 8  // How many pixels (in total) is the grad from start to end.
int8_t gradDelta = 1;  // 1 or -1.  (Negative value reverses direction.)

// If you wanted to move your gradient 32 pixels in 120 seconds, then:
// 120sec / 32pixel = 3.75sec
// 3.75sec x 1000miliseconds/sec = 3750milliseconds
#define gradMoveDelay 3750  // How fast to move the gradient (in Milliseconds)

CRGB grad[gradLength];  // A place to save the gradient colors. (Don't edit this)


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check your baud rate)
  delay(3000);  // Startup delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLOCK_PIN,COLOR_ORDER>(leds, NUM_LEDS);  // ***For strips using Clock.
  //FastLED.addLeds<LED_TYPE,DATA_PIN>(leds, NUM_LEDS);  // ***For Clock-less strips.
  FastLED.setBrightness(BRIGHTNESS);

  //fill the array that stores the gradient colors.
  fill_gradient(grad, gradStartPos, gradStartColor, gradStartPos+gradLength-1, gradEndColor, SHORTEST_HUES);

} //end setup


//---------------------------------------------------------------
void loop() { //START MAIN LOOP
  EVERY_N_MILLISECONDS(gradMoveDelay) {
    uint8_t count = 0;
    for (uint8_t i = gradStartPos; i < gradStartPos+gradLength; i++) {
      leds[i % NUM_LEDS] = grad[count];
      count++;
    }
    FastLED.show();  // Display the pixels.
    FastLED.clear();  // Clear the strip to not leave behind lit pixels as grad moves.
    
    gradStartPos = gradStartPos + gradDelta;  // Update start position.
    if ( (gradStartPos > NUM_LEDS-1) || (gradStartPos < 0) ) {  // Check if outside NUM_LEDS range
      gradStartPos = gradStartPos % NUM_LEDS;  // Loop around as needed.
    }

  } //end EVERY_N gradMoveDelay
  
} //END MAIN LOOP
