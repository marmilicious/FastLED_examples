
//***************************************************************
// Basic setup for testing out receiving MIDI on a Teensy 3.2
//
// Wiring is like this:
//   https://i.imgur.com/d4QRXhe.jpg
//
// Marc Miller, June 2018
//***************************************************************

#include <MIDI.h> // Add Midi Library
MIDI_CREATE_DEFAULT_INSTANCE();

#include "FastLED.h"  // Add FastLED library
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB  // GRB for Neopixel
#define DATA_PIN 11
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 32

int led = 13;  // pin for MCU's onboard LED

uint8_t count;
uint8_t hue;


//---------------------------------------------------------------
void blinkyblink() {
  digitalWrite(led, HIGH);  delay(50);
  digitalWrite(led, LOW);   delay(30);
  digitalWrite(led, HIGH);  delay(20);
  digitalWrite(led, LOW);
}


//---------------------------------------------------------------
void solidFill(uint16_t color) {
  if (color > 255) {
    fill_solid( leds, NUM_LEDS, CHSV(random8(),255,255) );  //pick random color
  } else {
    color = (uint8_t)color;  //convert 16bit to 8bit
    fill_solid( leds, NUM_LEDS, CHSV(color,255,255) );  //fill with sent hue
  }
  FastLED.show();
}

void spinColor(uint8_t color) {
  EVERY_N_MILLISECONDS(120) {
    fadeToBlackBy(leds, NUM_LEDS, 100);
    leds[count] = CHSV(color, random8(150,255), 255);
    FastLED.show();
    count++;
    if (count == NUM_LEDS) { count = 0; }
  }
}

void clearAll() {  // turn off all pixels
  FastLED.clear();
  FastLED.show();
  blinkyblink();  
}


//---------------------------------------------------------------
void HandleNoteOn(byte channel, byte pitch, byte velocity){
  if (velocity > 0){
    // check for specific notes
    if(pitch <= 50) { clearAll(); }
    else if(pitch == 51) { solidFill(888); }
    else if(pitch == 52) { solidFill(888); }
    else if(pitch == 53) { solidFill(hue); }
    else if(pitch == 54) { spinColor(0); }
    else if(pitch == 55) { solidFill(hue); }
    else if(pitch == 56) { spinColor(96); }
    else if(pitch == 57) { solidFill(hue); }
    else if(pitch == 58) { spinColor(160); }
    else if(pitch == 59) { solidFill(0); }
    else if(pitch == 60) { solidFill(96); }
    else if(pitch == 61) { clearAll(); }
    else if(pitch == 62) { solidFill(160); }
    else if(pitch == 63) { solidFill(888); }
    else if(pitch == 64) { solidFill(888); }
    else if(pitch == 65) {
      //if (velocity > 128) {
      //  try something extra here based on velocity;
      //} 
      solidFill(888);
    }
    else if(pitch >= 66) { clearAll(); }
  }
}


//---------------------------------------------------------------
void setup(){
  Serial1.begin(31250);  //using MIDI baud rate but could be something faster
  delay(1000);
  pinMode(led, OUTPUT);
  blinkyblink();  //check onboard LED

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB(32,0,0)); FastLED.show(); delay(333);
  fill_solid(leds, NUM_LEDS, CRGB(0,32,0)); FastLED.show(); delay(333);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,32)); FastLED.show(); delay(333);
  FastLED.clear();
  FastLED.show();
  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn); // Initiates HandleNoteOn function

  blinkyblink(); delay(100); blinkyblink(); delay(100); blinkyblink();
  Serial.println("Setup done. \n");
}//end_setup


//---------------------------------------------------------------
void loop(){
  MIDI.read();  // Tells Arduino to start listening for midi info on the serial RX pin.

  EVERY_N_MILLISECONDS(30) {
    hue++;
  }
  
}//end_main_loop


//---------------------------------------------------------------


