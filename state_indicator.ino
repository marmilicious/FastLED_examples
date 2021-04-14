//***************************************************************
/*
Machine State Indicator example
Marc Miller, Aug 2017

Creating this sketch was inspired by this post:
https://plus.google.com/u/0/+MichaelAndresen/posts/1xVDuFPsxUc

Different colors or color patterns can visually indicate to an
operator what state(s) a machine is in.  Machine states might
include: Warming up, Ready, Running, Paused, Waiting, Error, etc.

This example reads two signals coming from the machine to be
monitored and then updates an addressable RGB LED display. The
signal from the machine could be a digital logic signal, a PWM
signal, or an analog signal.  This example is setup to read a
logic signal (which will change the display of the first pixel
only), and also read an analog signal (which will change the
display of the remainder of the pixels).


The FastLED library is used for ease of programming the addressable
LEDs.  Here are links to the FastLED wiki, code, and reddit group:
  https://github.com/FastLED/FastLED/wiki/Frequently-Asked-Questions
  https://github.com/FastLED/FastLED
  http://fastled.io/+
  
**Note of caution** - Make sure the voltage level received by
the Arduino is NOT a negative DC voltage, OR above the max input
voltage the pin can handle, usually 3.3 or 5 volts depending on
the microcontroller, otherwise the pin might be destroyed.

Video of this code running:
https://youtu.be/aNB4iK3_Y_s

*/
//***************************************************************

#include "FastLED.h"        //library for controlling pixels
#define LED_TYPE WS2812B    //pixel type
#define DATA_PIN 11         //pin on Arduino for sending data to LEDs
#define COLOR_ORDER GRB     //color order can vary depending on LED manufacturer
#define NUM_LEDS 12         //number of pixels
CRGB leds[NUM_LEDS];

#define INPUT1 4            //input pin for logic signal from machine
#define INPUT2 A0           //input pin for analog (variable) signal from machine

boolean  VALUE1;            //the current value of INPUT1
uint16_t VALUE2;            //the current value of INPUT2
uint8_t  STATE;             //which switch case to display


//A testing mode allows colors or patterns to be tested without actually
//being hooked up and receiving signals from an external source.
//While in testing mode random values will be picked and used instead of
//values read from the inputs.  Random times are also picked.
boolean TESTING_MODE = 0;   // [0 = read real inputs,  1 = simulate inputs]


//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);  // Allows serial monitor output (check baud rate)
  delay(1500);  // Startup delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(64);  //LED master brightness [0-255]
  FastLED.clear();
}


//---------------------------------------------------------------
// Function to read all inputs and set appropriate values
void read_inputs() {  

  if (TESTING_MODE == 1) {
    EVERY_N_MILLISECONDS_I(t1,2) {
      t1.setPeriod(random16(3500,6000));
      VALUE1 = !VALUE1;
    }
    EVERY_N_MILLISECONDS_I(t2,2) {
      t2.setPeriod(random16(4500,8000));
      VALUE2 = random16(0,1024);
    }
    
  } else {  //not in testing mode
    //read the digital input
    VALUE1 = digitalRead(INPUT1);
  
    //read the analog input
    VALUE2 = analogRead(INPUT2);
  }


  //set switch case STATE variable based on VALUE2
  if (VALUE2 <= 200)                    { STATE = 1; }
  if (VALUE2  > 200 && VALUE2 <= 400)   { STATE = 2; }
  if (VALUE2  > 400 && VALUE2 <= 600)   { STATE = 3; }
  if (VALUE2  > 600 && VALUE2 <= 800)   { STATE = 4; }
  if (VALUE2  > 800 && VALUE2 <= 1000)  { STATE = 5; }
  if (VALUE2  > 1000)                   { STATE = 0; }
  // For this example if VALUE2 is above 1000 it will cause the "default" case to run.
  
  // Uncomment these print statements to monitor values on the serial monitor.
  //Serial.print(" VALUE1: "); Serial.print(VALUE1);
  //Serial.print("\t\tVALUE2: "); Serial.print(VALUE2);
  //Serial.print("  -->  STATE: "); Serial.println(STATE);

}//end of read_inputs


//---------------------------------------------------------------
void loop() {

  //Periodically run the function to read all inputs
  EVERY_N_MILLISECONDS_I(timingObjC, 2){
    timingObjC.setPeriod(500);  //how often to check [milliseconds]
    read_inputs();
  }


  //set the display of pixel 0
  //Note: this example has pixel 0 hard coded in the two functions being called below.
  if (VALUE1 == 1) {
    red_blink();
  } else {
    green_solid();
  }


  //set the display for the rest of the pixels
  switch (STATE) {
    case 1: { yellow_solid(); }
      break;
    case 2: { orange_spiral(); }
      break;
    case 3: { purple_marquee(); }
      break;
    case 4: { blue_fadeout(); }
      break;
    case 5: { aqua_double_flash(); }
      break;

    // If nothing matches a case # above then run this optional default.
    default: { white_pulse(); }
      break;
  }//end of switch cases

  FastLED.show();  //Display the pixel data

}//end of main loop



//---------------------------------------------------------------
//Example functions to make various color/patterns
//---------------------------------------------------------------
//Green solid
//Note:  In this example it's hard coded to pixel 0
void green_solid(){
  leds[0] = CRGB::Green;
}


//---------------------------------------------------------------
//Red blinking
//Note:  In this example it's hard coded to pixel 0
void red_blink() {
  static boolean blinking;
  EVERY_N_MILLISECONDS_I(timingObjB,2) {
    blinking = !blinking;
  }
  if (blinking == 1) {
    leds[0] = CRGB::Red;
    timingObjB.setPeriod(500);  //value sets blink On length [milliseconds]
  } else {
    leds[0] = CRGB::Black;
    timingObjB.setPeriod(250);  //value sets blink Off length [milliseconds]
  }
}


//---------------------------------------------------------------
//Yellow solid
void yellow_solid() {
  for (uint8_t i=1; i<NUM_LEDS; i++) {
    leds[i] = CHSV(66,245,220);
  }
}


//---------------------------------------------------------------
//Blue, then fade out
void blue_fadeout() {
  EVERY_N_MILLISECONDS_I(timingObjF, 2) {
    timingObjF.setPeriod(3000);  //how long before re-setting brightness [milliseconds]
    for (uint8_t i=1; i<NUM_LEDS; i++) {
      leds[i] = CHSV(160,200,200);
    }
  }
  EVERY_N_MILLISECONDS(45) {  //value is how often to fade
    for (uint8_t i=1; i<NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(8);  //value is amount to fade by
    }
  }
}


//---------------------------------------------------------------
//Purple marquee effect
void purple_marquee() {
  static boolean toggle;
  EVERY_N_MILLISECONDS_I(timingObjM, 2) {
    timingObjM.setPeriod(375);  //value sets toggle rate [milliseconds]
    toggle = !toggle;
  }
  //fill_solid(leds+1, NUM_LEDS-1, CHSV(185,220,170));
  fill_solid(leds+1, NUM_LEDS-1, CRGB::Black);
  for (uint8_t i=1+toggle; i<NUM_LEDS; i=i+2) {
    leds[i] = CHSV(195,255,220);
  }
}


//---------------------------------------------------------------
//Orange spiral effect
void orange_spiral() {
  static uint8_t pos = 1;
  EVERY_N_MILLISECONDS(40) {  //value sets movment speed
    for (uint8_t i=1; i<NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(25);  //value is amount to fade by
    }
    leds[pos] = CHSV(28,255,240);
    pos = pos + 1;
    if (pos == NUM_LEDS) { pos = 1; } //reset
  }
}


//---------------------------------------------------------------
//White pulsing effect
void white_pulse() {
  EVERY_N_MILLISECONDS(20) {  //value sets pulse speed
    static uint16_t dV;
    uint8_t val = (sin8(dV/8)/2)+127;
    for (uint8_t i=1; i<NUM_LEDS; i++) {
      leds[i] = CHSV(0,0,val);
      dV++;
      if (dV == (256*8)) { dV = 0; }  //reset
    }
  }
}


//---------------------------------------------------------------
//Aqua double flash
void aqua_double_flash() {
  static boolean blinkGate1 = LOW;
  static boolean blinkGate2 = HIGH;
  static boolean blinking;
  static uint8_t blinkCount = 0;
  EVERY_N_MILLISECONDS_I(timingObjDf, 2){
    blinkGate2 = blinkCount;
    blinkGate1 = !blinkGate1;
    if (blinkGate1 * blinkGate2 == HIGH) {
      for (uint8_t i=1; i<NUM_LEDS; i++) {
        leds[i] = CHSV(130,255,150);
      }
    } else {
      for (uint8_t i=1; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
    }
    if (blinkCount == 2 || blinkCount == 3) {
      timingObjDf.setPeriod( 50 );
    } else if (blinkCount == 4) {
      timingObjDf.setPeriod( 405 );
    } else {
      timingObjDf.setPeriod( 167 );
    }
    blinkCount++;
    if (blinkCount == 6) { blinkCount = 0; }  // reset count
  }
}


//---------------------------------------------------------------
