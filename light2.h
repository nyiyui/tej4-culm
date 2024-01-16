/*
 * light2.h contains a hardware abstraction layer (HAL) for the light sensor. It allows to easily interface with the light sensor.
 * Ken Shibata
 * Date: 2024-01-16
 * Teacher: Mr. Wong
 */
// turn on LIGHT_DEBUG to show debug output
//#define LIGHT_DEBUG
#define LIGHT_LEN 4
int light_pins[4]         = { A4,  A3,  A2,  A1  };
int light_thresholds[4]   = { 960, 960, 960, 960 };
int light_uppers[4]       = { 950, 950, 950, 950 };
int light_lowers[4]       = { 940, 940, 940, 930 };
int light_raws[4]         = { 0 };
bool light_values[4]      = { 0 };
float light_normalized[4] = { 0 };

// light_setup() sets up the LED lights.
void light_setup() {
  for (int i = 0; i < LIGHT_LEN; i ++) {
    pinMode(light_pins[i], INPUT);
  }
}

// normalize() normalizes s onto the range from l to t and t to u.
// l = lower
// t = threshold
// u = upper
// The following must be true: l < t < u and l < s < u.
float normalize(float l, float t, float u, float s) {
  if (s > t)
    return (s-t)/(u-t);
  if (s == t)
    return 0;
  if (s < t)
    return (s-l)/(t-l)-1;
}

// light_read() reads the sensor values and records than in the light_raws, light_values, and light_normalized global variables.
void light_read() {
  for (int i = 0; i < LIGHT_LEN; i ++) {
    int raw = analogRead(light_pins[i]);
#    ifdef LIGHT_DEBUG
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(raw);
    Serial.print(" ; ");
#    endif
    light_raws[i] = raw;
    light_values[i] = raw >= light_thresholds[i];
    light_normalized[i] = normalize(light_lowers[i], light_thresholds[i], light_uppers[i], light_raws[i]);
    int value = light_normalized[i] * 255;
    value = abs(value);
    if (light_values[i])
      strip.setPixelColor(i, 0, value, 0);
    else
      strip.setPixelColor(i, value, 0, 0);
  }
  strip.show();
#  ifdef LIGHT_DEBUG
  Serial.println();
#  endif
}

// light_calibration_mode() implements a "calibration mode" for the lights which prints the raw values of the light (constantly).
void light_calibration_mode() {
  while (true) {
    if (Serial.available()) {
      int ctl = Serial.read();
      if (ctl == 'q') {
        Serial.println("quit");
        delay(500);
      }
    }
    light_read();
    Serial.print(light_raws[0]);
    Serial.print(" ");
    Serial.print(light_raws[1]);
    Serial.print(" ");
    Serial.print(light_raws[2]);
    Serial.print(" ");
    Serial.print(light_raws[3]);
    Serial.print(" ");
    Serial.println(light_raws[4]);
    delay(100);
  }
}

// get_delta() computes the error function (for use in PID control) from light_values and light_normalized.
// The function computes the relative distance of the line from the theoretical centre of evenly-spaced sensors.
float get_delta() {
  // find right edge of line
  const int offset = LIGHT_LEN/2;
#  ifdef LIGHT_DEBUG
  for (int i = 0; i < LIGHT_LEN; i ++) {
    Serial.print(light_values[i]);
  }
#  endif
  for (int i = LIGHT_LEN-1; i >= 0; i --) {
    if (!light_values[i]) {
      if (i >= LIGHT_LEN) {
        strip.fill(255, 0, 0);
        strip.show();
        Serial.println("bounds overflow 1");
        while (1) {}
      }
      return 2*(-light_normalized[i]) + i - offset;
    }
  }
  return -offset;
}
