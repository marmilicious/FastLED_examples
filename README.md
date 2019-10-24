# FastLED_examples

FastLED Library info can be be found here: [fastled.io](http://fastled.io/)

And be sure to check out the wiki: [github.com/FastLED/FastLED/wiki/Overview](https://github.com/FastLED/FastLED/wiki/Overview)


Download the current FastLED library from: [github.com/FastLED/FastLED](https://github.com/FastLED/FastLED)
(Use "Download Zip" link on right side.)




-Marc Miller, 
Febuary 2015




## How to use these examples

1. Install the FastLED library:
	- Open the Arduino editor
	- Choose Tools, Manage Libraries
	- In the search box, enter `fastled` (capitalization doesn't matter)
	- Click Install
2. [Wire up and connect your pixel strip](https://github.com/FastLED/FastLED/wiki/Wiring-leds) to your Arduino, taking note of the data pin (and clock pin, if appropriate for your strip) that you used.
3. Determining the [color order of your pixels](https://github.com/FastLED/FastLED/wiki/Rgb-calibration):
	- In the Arduino editor, choose File, Examples, FastLED, RGBCalibrate
	- Define DATA_PIN to the pin that data is connected to.
	- If you're using a 4-wire pixel strip (e.g. an APA102), define the CLOCK_PIN
	- Uncomment the line in setup that corresponds to the LED chipset that you are using. (Note that they all explicitly specify the RGB order as RGB)
	- Compile/upload/run the sketch
	- You should see six leds on.  If the RGB ordering is correct, you should see 1 red led, 2 green leds, and 3 blue leds.  If you see different colors, the count of each color tells you what the position for that color in the rgb orering should be.  So, for example, if you see 1 Blue, and 2 Red, and 3 Green leds then the rgb ordering should be BRG (Blue, Red, Green).
4. Now you're ready to try these examples. Open an ino file. The Arduino editor will prompt you that it has to be in a folder. Click OK and it will move the file for you.
5. Edit the defines at the top to match your configuration (pixel type, data pin, clock pin, etc.)

	```c
	#include "FastLED.h"
	#define LED_TYPE      APA102
	#define DATA_PIN      11
	#define CLK_PIN       13
	#define NUM_LEDS      32
	#define COLOR_ORDER   BGR
	#define BRIGHTNESS    128
	
	CRGB leds[NUM_LEDS];
	
	```
6. In the `setup` function, if you're using a 3-pin pixel strip (e.g. a WS2811) you'll need to uncomment the first `FastLED.addLeds` statement, and comment out the other. For 4-pin pixels, leave as-is.

	```c
	  // FOR 3-PIN LED STRIPS, EDIT THE CODE TO LOOK LIKE THIS:
	  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	  // FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	
	```
7. Compile/upload/run the sketch