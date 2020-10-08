//***************************************************************
// Messing with "striped" color palettes.
//
// This example is based off of the FastLED ColorPalette example.
// See that exmaple for some more info on color palettes.
// https://github.com/FastLED/FastLED/blob/master/examples/ColorPalette/ColorPalette.ino
//
//
// Marc Miller, Oct 2020
//***************************************************************

#include <FastLED.h>

#define LED_PIN     11
#define CLK_PIN     13
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    32
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];
CRGB temp[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

uint8_t colorDelta = 8;  // determines the size of the "stripe" length/gap


//---------------------------------------------------------------
void setup() {
    Serial.begin(115200);  // allows serial monitor output (check baud rate)
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    SetupPalette_RandomStripes1();  // starting out with this palette
    currentBlending = LINEARBLEND;
    FastLED.clear();
    FastLED.show();
    Serial.println("Setup done. \n");
}


//---------------------------------------------------------------
void loop()
{
    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    EVERY_N_MILLISECONDS(20) {  //lower value = faster
      startIndex = startIndex + 1;  //higher + value = faster
    }
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);

}// end_main_loop



//---------------------------------------------------------------
void UpdateColorIndexDelta()
{
    colorDelta = random8(4,13);  //pick new random delta
    Serial.print("  New colorDelta: "); Serial.println(colorDelta);
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += colorDelta;
    }
}


void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { UpdateColorIndexDelta(); }
        if( secondHand ==  0)  { SetupPalette_RandomStripes1();         currentBlending = LINEARBLEND; }
        if( secondHand ==  6)  { SetupPalette_RandomStripes1();         currentBlending = LINEARBLEND; }
        if( secondHand == 12)  { SetupPalette_RandomStripes2();         currentBlending = LINEARBLEND; }
        if( secondHand == 18)  { SetupPalette_RandomStripes2();         currentBlending = LINEARBLEND; }
        if( secondHand == 24)  { SetupPalette_RandomStripes2();         currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupPalette_GradientStriped();        currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { SetupPalette_GradientRandomStriped();  currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { SetupPalette_GradientRandomStriped();  currentBlending = LINEARBLEND; }
        if( secondHand == 55)  { SetupPalette_GradientRandomStriped();  currentBlending = LINEARBLEND; }
        if( secondHand == 55)  { SetupPalette_GradientRandomStriped();  currentBlending = LINEARBLEND; }
    }
}


//---------------------------------------------------------------
// Custom Palettes
//---------------------------------------------------------------

void SetupPalette_RandomStripes1()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // pick a random color and set every fourth one to this color.
    CHSV randomColor = CHSV(random8(), 255, 255);
    currentPalette[0] = randomColor;
    currentPalette[4] = randomColor;
    currentPalette[8] = randomColor;
    currentPalette[12] = randomColor;    

    Serial.println("Running: SetupPalette_RandomStripes1"); 
}


void SetupPalette_RandomStripes2()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one with a few random colors.
    currentPalette[0] = CHSV(random8(), random8(100,255), 255);
    currentPalette[4] = CHSV(random8(), random8(100,255), 255);
    currentPalette[8] = currentPalette[0];  //setting the same as 0 for some uniformity
    currentPalette[12] = currentPalette[4];  //setting the same as 4 for some uniformity
    
    Serial.println("Running: SetupPalette_RandomStripes2"); 
}


void SetupPalette_GradientStriped()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);

    // Fill a temp CRGB array with specific colors using fill_gradient.
    //     fill_gradient works like this:
    //     fill_gradient(arrayToUse, startpos, startColor, endpos, endColor, blendType);
    CRGB temp[4];  //size of 4 since that's all we need for below.
    fill_gradient(temp, 0, CHSV(90,255,255) , 3, CHSV(135,255,255), SHORTEST_HUES);

    // Set every fourth one using colors from gradient in temp.
    currentPalette[0] = temp[0];
    currentPalette[4] = temp[1];
    currentPalette[8] = temp[2];
    currentPalette[12] = temp[3];

    Serial.println("Running: SetupPalette_GradientStriped"); 
}


void SetupPalette_GradientRandomStriped()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);

    // Fill a temp CRGB array with random colors using fill_gradient.
    //     fill_gradient works like this:
    //     fill_gradient(arrayToUse, startpos, startColor, endpos, endColor, blendType);
    CRGB temp[4];  //size of 4 since that's all we need for below.
    uint8_t randomHue = random8();
    fill_gradient(temp, 0, CHSV(randomHue, 255, 255) , 3, CHSV(randomHue + 60, 255, 255), SHORTEST_HUES);
    //For the endColor we are adding 60 so the two colors are
    //always relatively close together on the color wheel.

    // Set every fourth one using colors from gradient in temp.
    currentPalette[0] = temp[0];
    currentPalette[4] = temp[1];
    currentPalette[8] = temp[2];
    currentPalette[12] = temp[3];

    Serial.println("Running: SetupPalette_GradientRandomStriped"); 
}
