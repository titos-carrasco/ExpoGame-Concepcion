#include "FastLED.h"
 
#define NUM_LEDS 50
#define DATA_PIN 2
 
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B,DATA_PIN,GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(50);
}

void loop() 
{
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = CRGB::Yellow;
    FastLED.show();
    delay(200);
    leds[dot] = CRGB::Black;
    FastLED.show();
  }
}
