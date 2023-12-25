#include <Adafruit_NeoPixel.h>

#define STATUS_LED_PIN 9
#define STATUS_LED_COUNT 9

Adafruit_NeoPixel strip(STATUS_LED_PIN, STATUS_LED_COUNT);

void status_setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(64);
}
