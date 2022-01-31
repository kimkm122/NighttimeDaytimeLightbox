#include <FastLED.h>

#define NUM_LEDS  80
#define LED_PIN   6
#define SENSE_PIN A0
#define DAY_THRESH 700    // Photoresistor value to change to daytime
#define NIGHT_THRESH 150  // Photoresistor value to change to nighttime

CRGB source1[NUM_LEDS];
CRGB source2[NUM_LEDS];
CRGB output[NUM_LEDS];

uint8_t blendAmount = 0;
uint8_t patternCounter = 0;
uint8_t source1Pattern = 0;
uint8_t source2Pattern = 1;
bool useSource1 = false;

uint8_t currentPattern = 0;
uint8_t newPattern = 0;

void setup() {
  // set up LEDs
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(output, NUM_LEDS);
  FastLED.setBrightness(150);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setTemperature(Halogen);

  //set up Photoresistor
  pinMode(SENSE_PIN, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  
  EVERY_N_MILLISECONDS(10) {
    blend(source1, source2, output, NUM_LEDS, blendAmount);   // Blend between the two sources

    if (useSource1) {
      if (blendAmount < 255) blendAmount++;                   // Blend 'up' to source 2
    } else {
      if (blendAmount > 0) blendAmount--;                     // Blend 'down' to source 1
    }
  }

  EVERY_N_MILLISECONDS(1000) {
    int value = analogRead(SENSE_PIN);                          // Read Photoresistor
    //Serial.println(value);
    if (value > DAY_THRESH) newPattern = 0;
    else if (value > NIGHT_THRESH) newPattern = 1;
    else newPattern = 2;
    if (currentPattern != newPattern){
      changePattern();                                        // Change pattern if light value changes
    }
  }

  runPattern(source1Pattern, source1);                  // Run both patterns simultaneously
  runPattern(source2Pattern, source2);
  
  FastLED.show();
}

void changePattern() {
  if (useSource1) source1Pattern = newPattern;
  else source2Pattern = newPattern;

  useSource1 = !useSource1;
  currentPattern = newPattern;
}

void runPattern(uint8_t pattern, CRGB *LEDArray) {
  switch (pattern) {
    case 0:
      setMidday(LEDArray);
      break;
    case 1:
      setSunrise(LEDArray);
      break;
    case 2:
      setNighttime(LEDArray);
      break;
  }
}

//------- light patterns -------//

void setMidday(CRGB *LEDarray) {
  fill_gradient_RGB(LEDarray, 0, CRGB(0, 255, 255), 24, CRGB(0, 255, 255));   // City
  fill_gradient_RGB(LEDarray, 25, CRGB(255, 255, 0), 49,CRGB(255, 255, 0));   // Horizon
  fill_gradient_RGB(LEDarray, 50, CRGB(255, 255, 80), 74, CRGB(255, 255, 80));  // Sky
  fill_gradient_RGB(LEDarray, 75, CRGB(255, 255, 0), 79,CRGB(255, 255, 0));   // Sun
}

void setSunrise(CRGB *LEDarray) {
  fill_gradient_RGB(LEDarray, 0, CRGB(0, 215, 25), 24, CRGB(0, 215, 25));   // City
  fill_gradient_RGB(LEDarray, 25, CRGB(255, 0, 10), 49, CRGB(255, 0, 10));   // Horizon
  fill_gradient_RGB(LEDarray, 50, CRGB(225, 5, 140), 74, CRGB(225, 5, 140));  // Sky
  fill_gradient_RGB(LEDarray, 75, CRGB(255, 0, 10), 79, CRGB(255, 0, 10));   // Sun
}

void setNighttime(CRGB *LEDarray) {
  fill_gradient_RGB(LEDarray, 0, CRGB(245, 165, 45), 24, CRGB(245, 165, 45));   // City
  fill_gradient_RGB(LEDarray, 25, CRGB(0, 155, 255), 49, CRGB(0, 155, 255));   // Horizon
  fill_gradient_RGB(LEDarray, 50, CRGB(125, 0, 255), 74, CRGB(125, 0, 255));  // Sky
  fill_gradient_RGB(LEDarray, 75, CRGB(0, 155, 255), 79, CRGB(0, 155, 255));   // Sun
