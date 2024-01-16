/*
 * status.h contains code for the status LEDs.
 * Ken Shibata
 * Date: 2024-01-16
 * Teacher: Mr. Wong
 */
#include <Adafruit_NeoPixel.h>

#define STATUS_LED_PIN 9
#define STATUS_LED_COUNT 9
#define STATUS_MODE 5
#define STATUS_SUBMODE 6

// strip is the object for controlling the lights.
Adafruit_NeoPixel strip(STATUS_LED_PIN, STATUS_LED_COUNT);

// light_setup() sets up the status LED lights.
void status_setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(64);
}
