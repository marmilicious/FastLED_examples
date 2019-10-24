// There are multiple examples below showing the use of
//   fill_gradient, and fill_gradient_RGB
//
// Un-comment examples one at a time to experiment.
//
// Note1:
//   When filling all the way to the end always use NUM_LEDS-1.
//   (If you don't subtract 1 then it will break things.)
//
// Note2:
//   When using fill_gradient_RGB, do NOT include gradient direction
//   info (FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES)
//
// Marc Miller,  Nov 2016
//---------------------------------------------------------------

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define NUM_LEDS      32
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

CRGB leds[NUM_LEDS];

CHSV gsHSV(0, 255, 255); // grad start color in HSV format
CHSV geHSV(128, 255, 255); // grad end color in HSV format

CRGB gsRGB(255, 0, 0); // grad start color in RGB format
CRGB geRGB(0, 255, 0); // grad end color in RGB format

uint8_t num = 0;  // stores a number when animating grad


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000);  // startup delay
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

//---------------------------------------------------------------
void loop() {
    EVERY_N_MILLISECONDS(250) {
        num = num + 4;
        Serial.print(" num = ");
        Serial.println(num);

        /* ---------------Fill gradient using HSV format---------------
        * All three of these display the same output.
        *   The first inputs the color directly in HSV values.
        *   The second uses the HSV colors defined up above.
        *   The third uses the HSV colors defined up above and shows how to break out the h,s,v parts.
        * These gradients are static.
        */

        fill_gradient(leds, 0, CHSV(0, 255, 255), NUM_LEDS - 1, CHSV(128, 255, 255), SHORTEST_HUES);
        //fill_gradient(leds, 0, gsHSV, NUM_LEDS-1, geHSV, SHORTEST_HUES);
        //fill_gradient(leds, 0, CHSV(gsHSV.h,gsHSV.s,gsHSV.v), NUM_LEDS-1, CHSV(geHSV.h,geHSV.s,geHSV.v), SHORTEST_HUES);



        /*-----------
        * This shows how you could operate on the r,g,b parts of a previously defined
        *   HSV color, or only use some combination of the h, s, or v components.
        * This gradient is animated.
        */

        //fill_gradient(leds, 0, CHSV(gsHSV.h+num, random8(100,256), gsHSV.v), NUM_LEDS-1, CHSV(geHSV.h+num, geHSV.s, random8(180,256)), SHORTEST_HUES);




        /* ---------------Fill gradient using RGB format---------------
        * All three of these display the same output.
        *   The first inputs the color directly in RGB values.
        *   The second uses the RGB colors defined up above.
        *   The third uses the RGB colors defined up above and shows how to break out the h,s,v parts.
        * These gradients are static.
        * *Note* Do NOT use SHORTEST_HUES on the end.
        */

        //fill_gradient_RGB(leds, 0, CRGB(255,0,0), NUM_LEDS-1, CRGB(0,255,0));
        //fill_gradient_RGB(leds, 0, gsRGB, NUM_LEDS-1, geRGB);
        //fill_gradient_RGB(leds, 0, CRGB(gsRGB.r, gsRGB.g, gsRGB.b), NUM_LEDS-1, CRGB(geRGB.r, geRGB.g, geRGB.b));


        /*----------
        * This shows how you could operate on the r,g,b parts of a previously defined
        *   RGB color, or only use some combination of the r, g, or b components.
        * This gradient is animated.
        */

        //fill_gradient_RGB( leds, 0, CRGB(gsRGB.r+num, gsRGB.g, 255), NUM_LEDS-1, CRGB(0, geRGB.g+num, random8(0,64)) );



        FastLED.show();  //display the pixels
    }

}//end main loop
