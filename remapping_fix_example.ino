//***************************************************************
// Remapping to fix a wacky layout.
// This also will then also allow the use of CRGBSet.
// Marc Miller, Jan 2017
//***************************************************************
//
// Have this layout:       Want to operate on this layout:
//     2  3  4                3  4  5
//   1         5            2         6
//   0         6            1         7
//  11         7            0         8
//    10  9  8               11 10  9
//
//    14 15 16               15 16 17
//  13        17           14        18
//  12        18           13        19
//  23        19           12        20
//    22 21 20               23 22 21
//
//  etc..
//
//
//***************************************************************

#include "FastLED.h"
#define LED_TYPE      APA102
#define DATA_PIN      11
#define CLK_PIN       13
#define COLOR_ORDER   BGR
#define BRIGHTNESS    128

#define NumberOfDiamonds 3  //3
#define PixelsPerDiamond 12  //12
#define NUM_LEDS (NumberOfDiamonds * PixelsPerDiamond)

/* CRGB leds[NUM_LEDS];  ***Not using this.  Using CRGBArray instead.****/
CRGBArray<NUM_LEDS> leds;    // Operate on this array as if it had the correct layout.
CRGBArray<NUM_LEDS> actual;  // Display this array after mapping "leds" to "actual".

//Since we'll opperate on our nicely arranged "leds" array, we can make a CRGBSet for each diamond.
CRGBSet d0(  leds(PixelsPerDiamond * 0,  PixelsPerDiamond - 1 + (PixelsPerDiamond * 0)  )); //Define pixel range for diamond 0.
CRGBSet d1(  leds(PixelsPerDiamond * 1,  PixelsPerDiamond - 1 + (PixelsPerDiamond * 1)  )); //Define pixel range for diamond 1.
CRGBSet d2(  leds(PixelsPerDiamond * 2,  PixelsPerDiamond - 1 + (PixelsPerDiamond * 2)  )); //Define pixel range for diamond 2.
//etc..




//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // Allows serial monitor output (check baud rate)
    delay(3000); // 3 second delay for recovery

    /* *********************
       NOTE: Putting the CRGBArray "actual" in here, not "leds"***
    *  ********************* */
    // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(actual, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(actual, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}


//---------------------------------------------------------------
void loop() {

    EVERY_N_SECONDS(3) {
        static uint8_t hue;
        for (uint8_t n = 0; n < NumberOfDiamonds; n++) {
            fill_rainbow( &(leds[ (n * PixelsPerDiamond) ]), PixelsPerDiamond, hue + millis(), 8 );
        }
    }

    EVERY_N_SECONDS(12) {
        random16_add_entropy( random() );
        uint8_t randomColor = random8();  //pick a random color
        d0 = CHSV(randomColor, 170, 150);
        d1 = CHSV(randomColor + 64, 170, 150);
        d2 = CHSV(randomColor - 64, 170, 150);
    }

    remap();  //***Always run the remap function just before calling show().***
    FastLED.show();

}//end_main_loop


//---------------------------------------------------------------
// Function to remap "leds" array to "actual" array
void remap() {
    uint8_t mapped;
    uint8_t diamondNumber;
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        if (i % PixelsPerDiamond == 0) {
            diamondNumber = i / PixelsPerDiamond;
            mapped = (diamondNumber * PixelsPerDiamond) + 11;
            actual[i] = leds[mapped];
            //Serial.print("Diamond#: "); Serial.println(diamondNumber);
        } else {
            mapped = i - 1;
            actual[i] = leds[mapped];
        }
        //Serial.print("  For leds[i] = "); Serial.print(i); Serial.print("  actual[i] = "); Serial.println(mapped);
    }
}
