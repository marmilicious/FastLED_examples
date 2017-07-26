//***************************************************************
// Brake lights triggerd by a button example.
//
// Open the serial monitor to see some feedback as the button is
// pressed, held, and released.
//
// This is the hardware debounced button wiring setup I use:
// http://i.imgur.com/hOgQwjX.jpg
// It does not include the capacitor typically included in a
// debounce setup, but this has always worked fine for me.
//
// Marc Miller, July 2017
//***************************************************************


//---------------------------------------------------------------
// This sketch uses JChristensen's Button Library from:
//   https://github.com/JChristensen/Button
//
#include "Button.h"    // Include the Button library

// Set the digital pin for the debounced pushbutton, and declare the button
const uint8_t brakeButtonPin = 4;
Button brakeButton(brakeButtonPin, true, true, 50);


//---------------------------------------------------------------
#include "FastLED.h"
#define DATA_PIN    11
#define CLK_PIN   13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    8
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 255

uint8_t BrakeStatus = 0;  // Status of brake lights.  [0=Off, 1=Activated, 2=On]

CHSV black (0,0,0);  // black
CHSV brakeRed (0,255,255);  // red full brightness
CHSV brakeDimRed (0,255,32);  // dim red
CHSV accent (185,120,70);  // alternate accent color

// setup pixel groups
byte center[] = {3,4};  // center ring
byte first[] = {2,3,4,5};  // first ring
byte second[] = {1,2,5,6};  // second ring
byte third[] = {0,1,6,7};  // third ring


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500); // short delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}


//---------------------------------------------------------------
void loop()
{
  readbutton();  // check the button

  if (BrakeStatus == 0) { brakeOff(); }
  if (BrakeStatus == 1) { brakeActivated(); }
  if (BrakeStatus == 2) { brakeOn(); }
  FastLED.show();

}//end_main_loop




//---------------------------------------------------------------
// Functions

//---------------------------------------------------------------
//Check the button and set the appropriate flag
void readbutton() {
  brakeButton.read();  // check the state of the brake button

  if(brakeButton.isPressed()) {
    BrakeStatus = 2;
    EVERY_N_MILLISECONDS(250){
      Serial.print(BrakeStatus); Serial.println("  Brake is On...");
    }
  } else {
    BrakeStatus = 0;
    EVERY_N_MILLISECONDS(500){
      Serial.print(BrakeStatus); Serial.println("  Brake Off");
    }
  }

  if(brakeButton.wasPressed()) {
    BrakeStatus = 1;
    Serial.print(BrakeStatus); Serial.println("  Running brake activated sequence.");
  }

}//end_readbutton


//---------------------------------------------------------------
// Brake lights activated sequence
void brakeActivated(){

  for (uint8_t i=0; i<(sizeof(third)/sizeof((third)[0])); i++) {
   leds[third[i]] = black;
  }
  for (uint8_t i=0; i<(sizeof(center)/sizeof((center)[0])); i++) {
   leds[center[i]] = brakeRed;
  }
  FastLED.show();
  delay(200);


  for (uint8_t i=0; i<(sizeof(first)/sizeof((first)[0])); i++) {
    leds[first[i]] = brakeRed;
  }
  FastLED.show();
  delay(200);


  for (uint8_t i=0; i<(sizeof(first)/sizeof((first)[0])); i++) {
   leds[first[i]] = accent;
  }
  for (uint8_t i=0; i<(sizeof(second)/sizeof((second)[0])); i++) {
    leds[second[i]] = brakeRed;
  }
  FastLED.show();
  delay(200);


  for (uint8_t i=0; i<(sizeof(first)/sizeof((first)[0])); i++) {
   leds[first[i]] = black;
  }
  for (uint8_t i=0; i<(sizeof(second)/sizeof((second)[0])); i++) {
    leds[second[i]] = accent;
  }
  for (uint8_t i=0; i<(sizeof(third)/sizeof((third)[0])); i++) {
    leds[third[i]] = brakeRed;
  }
  FastLED.show();
  delay(200);

}//end_brakeLightsOn


//---------------------------------------------------------------
// Brake lights On
void brakeOn(){
  fill_solid(leds,NUM_LEDS,CRGB::Black);
  for (uint8_t i=0; i<(sizeof(third)/sizeof((third)[0])); i++) {
    leds[third[i]] = brakeRed;
  }
}


//---------------------------------------------------------------
// Brake lights Off
void brakeOff(){
  fill_solid(leds,NUM_LEDS,CRGB::Black);
  for (uint8_t i=0; i<(sizeof(third)/sizeof((third)[0])); i++) {
    leds[third[i]] = brakeDimRed;  // use dim red
  }
}

