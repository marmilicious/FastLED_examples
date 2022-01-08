//***************************************************************
// This is Mark Kriegsman's FastLED DemoReel100 example with
// a modificaiton to use a button for changing patterns.  The
// timer used for picking a new pattern has been commented out
// and there is a button check inside the main loop now.
//
// Search for "button" to find the various button related
// code additions.
//
// You can view the serial monitor to see a message when the
// button is pressed.
//
// Marc Miller, March 2017
//   Updated Jan 2020 - for JC_button library updates
//***************************************************************

#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          128
#define FRAMES_PER_SECOND  120


//---------------------------------------------------------------
// This uses JChristensen's Button Library from:
//   https://github.com/JChristensen/JC_Button
#include "JC_Button.h"    // Include Button library
const uint8_t buttonPin = 4;  // Set digital pin used with debounced pushbutton
Button myButton(buttonPin, 50, true, true);  // Declare the button  (pin, dbTime, puEnable, invert)


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // short delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  
  myButton.begin();  // initialize the button object

  Serial.println("Setup done.\n");
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { applause, fillAndCC, blinkyblink1, blinkyblink2, spewFour, spew, confetti_GB, rainbow, confetti, sinelon, juggle };
SimplePatternList gPatterns = { twoDots, fillAndCC, blinkyblink2, spewFour, spew, confetti_GB, rainbow, confetti, sinelon, juggle };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


//---------------------------------------------------------------
void loop()
{
  // BUTTON STUFF
  // Not using this timer to change patterns any more.  Instead check the button.
  //   EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  readbutton();  // check for button press
  
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}//end_main_loop

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


//---------------------------------------------------------------
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

//////////////////////////
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 12);
}

//////////////////////////
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), random8(128,200), random8(48,255));
}


//////////////////////////
void confetti_GB()
{
  // random colored speckles, Green and Blue hues only
  // Green = 96, Blue = 160
  uint8_t p = 25;  // What percentage of the time to make speckles.  [Range 0-100]

  fadeToBlackBy( leds, NUM_LEDS, 10);
  if (random8(100) < p) {
    int pos = random16(NUM_LEDS);
    uint8_t hue = random8(2);  // randomly chooses a 0 or 1
    if (hue == 0) {
      hue = random8(92,111);  // pick a hue somewhere in the green zone
    } else {
      hue = random8(156,165);  // pick a hue somewhere in the blue zone
    }
    leds[pos] += CHSV( hue, random8(200,240), 255);
  }
}//end confetti_GB


//////////////////////////
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 12);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

//////////////////////////
void juggle() {
  // four colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 4; i++) {
    leds[beatsin16( i+5, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}



//////////////////////////
void spew() {
  const uint16_t spewSpeed = 100;  // rate of advance
  static boolean spewing = 0;  // pixels are On(1) or Off(0)
  static uint8_t count = 1;  // how many to light (or not light)
  static uint8_t temp = count;
  static uint8_t hue = random8();
  EVERY_N_MILLISECONDS(spewSpeed){
    if (count == 0) {
      spewing = !spewing;
      if (spewing == 1) { count = random8(2,5); }  // random number for On pixels
      else { count = random8(1,8); }  // random number for Off pixels
      temp = count;
      //gHue = gHue - 30;
      hue = random8();
    }
    for (uint8_t i = NUM_LEDS-1; i > 0; i--) {
      leds[i] = leds[i-1];  // shift data down the line by one pixel
    }
    if (spewing == 1) {  // new pixels are On
      if (temp == count) {
        leds[0] = CHSV(hue-5, 215, 255);  // for first dot
        //leds[0] = CHSV(gHue-5, 215, 255);  // for first dot
      } else {
        leds[0] = CHSV(hue, 255, 255/(1+((temp-count)*2)) );  // for following dots
        //leds[0] = CHSV(gHue, 255, 255/(1+((temp-count)*2)) );  // for following dots
      }
    } else {  // new pixels are Off
      leds[0] = CHSV(0,0,0);  // set pixel 0 to black
    }
    count = count - 1;  // reduce count by one.
  }//end every_n
}//end spew

//////////////////////////
void spewFour() {
  //Similar to the abouve "spew", but split up into four sections,
  //specifically designed for a 8x4 matrix with Z-layout.
  const uint16_t spewSpeed = 100;  // rate of advance
  static uint8_t spewing[4] = {0,0,0,0};  // pixels are On(1) or Off(0)
  static uint8_t count[4] = {1,1,1,1};  // how many to light (or not light)
  static uint8_t temp[4] = {count[0],count[1],count[2],count[3]};
  static uint8_t hue[4] = {random8(), random8(), random8(), random8()};
  EVERY_N_MILLISECONDS(spewSpeed){
    for (uint8_t j=0; j<4; j++) {
      if (count[j] == 0) {
        if (spewing[j] == 0) {
          spewing[j] = 1;
        } else {
          spewing[j] = 0;
        }
        if (spewing[j] == 1) { count[j] = random8(2,5); }  // random number for On pixels
        else { count[j] = random8(1,8); }  // random number for Off pixels
        temp[j] = count[j];
        EVERY_N_SECONDS(2){  // hue going across is constant for awhile
          hue[j] = random8();
        }
      }
      for (uint8_t i = 7; i > 0; i--) {
        leds[(j*8)+i] = leds[(j*8)+i-1];  // shift data down the line by one pixel
      }
      if (spewing[j] == 1) {  // new pixels are On
        if (temp[j] == count[j]) {
          leds[((j+1)*8)-8] = CHSV(hue[j]-5, 215, 255);  // for first dot
        } else {
          leds[((j+1)*8)-8] = CHSV(hue[j], 255, 255/(1+((temp[j]-count[j])*2)) );  // for following dots
        }
      } else {  // new pixels are Off
        leds[((j+1)*8)-8] = CHSV(0,0,0);  // set pixel 0 to black
      }
      count[j] = count[j] - 1;  // reduce count by one.
    }//end for loop
  }//end every_n
}//end spewFour


//////////////////////////
void blinkyblink1() {
  static boolean dataIncoming = LOW;
  static boolean blinkGate1 = LOW;
  static boolean blinkGate2 = HIGH;
  static int8_t count = -1;

  EVERY_N_MILLISECONDS_I( timingObj, 250 ) {
    count++;
    if (count == 6) { count = 0; }
    blinkGate2 = count;
    dataIncoming = !dataIncoming;
    blinkGate1 = !blinkGate1;
    //Serial.print("c: "); Serial.print(count); Serial.print("\t");
    //Serial.print(dataIncoming); Serial.print("  "); Serial.print(blinkGate1);
    //Serial.print("\t"); Serial.print(dataIncoming * blinkGate1 * 255 * blinkGate2);
    //Serial.print("\tb: "); Serial.print(blinkGate2); Serial.println(" ");
    FastLED.clear();
    leds[0] = CHSV(gHue,0,dataIncoming*blinkGate1*255 * blinkGate2);
    if (count == 2 || count == 3) {
      timingObj.setPeriod( 50 );
    } else if (count == 4) {
      timingObj.setPeriod( 405 );
    } else {
      timingObj.setPeriod( 165 );
    }
  }
}//end_blinkyblink1


//////////////////////////
void blinkyblink2() {
  static boolean dataIncoming = LOW;
  static boolean blinkGate1 = LOW;
  static boolean blinkGate2 = HIGH;
  static int8_t count = -1;
  static int8_t P;

  EVERY_N_MILLISECONDS_I( timingObj, 250 ) {
    count++;
    if (count == 8) {
      count = 0;
      P = random8(NUM_LEDS);
    }
    blinkGate2 = count;
    dataIncoming = !dataIncoming;
    blinkGate1 = !blinkGate1;
    //Serial.print("c: "); Serial.print(count); Serial.print("\t");
    //Serial.print(dataIncoming); Serial.print("  "); Serial.print(blinkGate1);
    //Serial.print("\t"); Serial.print(dataIncoming * blinkGate1 * 255 * blinkGate2);
    //Serial.print("\tb: "); Serial.print(blinkGate2); Serial.println(" ");
    FastLED.clear();
    leds[P] = CHSV(gHue,255,dataIncoming*blinkGate1*255 * blinkGate2);
    if (count == 6) {
      timingObj.setPeriod( 250 );
    } else if (count == 7) {
      timingObj.setPeriod( 500 );
    } else {
      timingObj.setPeriod( 25 );
    }
  }
}//end_blinkyblink2


//////////////////////////
void fillAndCC() {
  static int16_t pos = 0;  // position along strip
  static int8_t delta = 3;  // delta (can be negative, and/or odd numbers)
  static uint8_t hue = 0;  // hue to display
  EVERY_N_MILLISECONDS(50) {
    leds[pos] = CHSV(hue,255,255);
    pos = (pos + delta + NUM_LEDS) % NUM_LEDS;
    if (delta >= 0 && pos == 0) {  //going forward
      hue = hue + random8(42,128);
    }
    if (delta < 0 && pos == NUM_LEDS-1) {  //going backward
      hue = hue + random8(42,128);
    }
  }
}//fillAndCC



//////////////////////////
void applause()    //originally by Mark K.
{
//Number of loops is increased based on number of pixels.
//Fade ammount drops as pixels increase.
static uint8_t appAmmount = (NUM_LEDS/32);
  for (uint8_t i=0; i<appAmmount; i++) {
    static uint16_t lastPixel = 0;
    fadeToBlackBy( leds, NUM_LEDS, 32/appAmmount );  //was 32
    leds[lastPixel] = CHSV(random8(HUE_BLUE,HUE_PURPLE),255,255);
    lastPixel = random16(NUM_LEDS);
    leds[lastPixel] = CRGB::White;
  }
}


//////////////////////////
void twoDots() {
  static uint8_t pos;  //used to keep track of position
  EVERY_N_MILLISECONDS(70) {
    fadeToBlackBy( leds, NUM_LEDS, 200);  //fade all the pixels some
    leds[pos] = CHSV(gHue, random8(170,230), 255);
    leds[(pos+5) % NUM_LEDS] = CHSV(gHue+64, random8(170,230), 255);
    pos = pos + 1;  //advance position
    
    //This following check is very important.  Do not go past the last pixel!
    if (pos == NUM_LEDS) { pos = 0; }  //reset to beginning
    //Trying to write data to non-existent pixels causes bad things.
  }
}//end_twoDots



//---------------------------------------------------------------

//---------Function to read the button and do something----------
//   This section could be changed to use a potentiometer,
//   encoder, or something else to trigger a pattern change.
//   Depending on what was used, the nextPattern() function
//   might need to be updated as well to have things work
//   the way you intend.

void readbutton() {
  myButton.read();
  if(myButton.wasPressed()) {
    Serial.println("Button pressed!  Next pattern...   ");
    nextPattern();  // Change to the next pattern

    //Flash pixel zero white as a visual that button was pressed.
    leds[0] = CHSV(0,0,255);  //Set first pixel color white
    FastLED.show();  //Update display
    delay(100);  //Short pause so we can see leds[0] flash on
    leds[0] = CRGB::Black;  //Set first pixel off
    FastLED.show();
    delay(100);
  }
}//end_readbutton


//---------------------------------------------------------------
