#include <Adafruit_NeoPixel.h>

#define STATUS_LED_PIN 9
#define STATUS_LED_COUNT 9
#define STATUS_MODE 5
#define STATUS_SUBMODE 6

Adafruit_NeoPixel strip(STATUS_LED_PIN, STATUS_LED_COUNT);

void status_setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(64);
}
