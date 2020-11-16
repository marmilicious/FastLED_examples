// Read an incoming string of "r,g,b" values (ex. 0,255,64 ) and
// convert the input to a CRGB color and display.
//
// *Note*:  There are NO checks in this example to confirm the
// incoming color string is valid.  For example, entering values
// above 255, more then 3 values, letters, etc.  The incoming
// string should be entered in the form of "r,g,b" (without quotes).
// Entering the string in a different format may hang the program.
//
// Open the serial monitor and enter a color string to update
// the display.
//
//
// Marc Miller,  Nov 2020
//***************************************************************

#include "FastLED.h"
#define DATA_PIN 11
#define CLOCK_PIN 13
#define LED_TYPE LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS 32
#define BRIGHTNESS 42
CRGB leds[NUM_LEDS];

String colorString("0,255,64");  // holds incoming string of "r,g,b" values
CRGB newColor;


//===============================================================
void setup() {
  Serial.begin(115200);
  delay(1500);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  string2CRGB(colorString);  // run during setup to display some initial color

  fill_solid(leds, NUM_LEDS, newColor);
  FastLED.show();

  Serial.println("Setup done.\n");  
}  //end of setup
  

//===============================================================
void loop() {

  if (Serial.available() > 0) {
    colorString = Serial.readString();  // read "r,g,b" string

    string2CRGB(colorString);  // convert string to CRGB color
    
    fill_solid(leds, NUM_LEDS, newColor);
    FastLED.show();
  }

  delay(100);

}  // end main loop


//---------------------------------------------------------------
// function to read string of "r,g,b" values and convert to a CRGB color
void string2CRGB(String colorString) {

  String colors[3];
  uint8_t p = 0;

  for (uint8_t i = 0; i < colorString.length(); i++) {
    if (colorString[i] == ',') {
      p++;
    } else {
    colors[p] = colors[p] + colorString[i];
    }
  }

  newColor = CRGB(colors[0].toInt(), colors[1].toInt(), colors[2].toInt() );

  Serial.print("Input colorString: "); Serial.println(colorString);
  //Serial.print("Length of colorString: "); Serial.println(colorString.length());

  //Serial.print("newColor (CRGB values):  ");
  //Serial.print(newColor.r);  Serial.print(",");
  //Serial.print(newColor.g);  Serial.print(",");
  //Serial.println(newColor.b);
  //Serial.println(" ");
}
