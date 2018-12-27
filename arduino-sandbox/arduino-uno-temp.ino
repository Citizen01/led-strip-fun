#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN D3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 300
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 20
#define FRAMES_PER_SECOND 60

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup()
{
  delay(3000); // 3 second delay for recovery

  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  run();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  //EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

int gCurrentLedPos = 0; // Index number of which pattern is current

int mod(int x, int y) { return x < 0 ? ((x + 1) % y) + y - 1 : x % y; }

int nextLed() { return gCurrentLedPos = (gCurrentLedPos + 1) % NUM_LEDS; }

void run()
{
  nextLed();
  fadeToBlackBy(leds, NUM_LEDS, 10);
  leds[gCurrentLedPos] = CRGB::Red;
}
