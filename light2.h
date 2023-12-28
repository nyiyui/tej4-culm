// old in-front-of-axle LEDs
// #define LIGHT_LEN 3
// int light_pins[3]         = { A2, A4, A3 };
// int light_thresholds[3]   = { 800, 670, 750 };
// int light_uppers[3]       = { 890, 720, 770 };
// int light_lowers[3]       = { 630, 530, 710 };
// int light_raws[3]         = { 0 };
// bool light_values[3]      = { 0 };
// float light_normalized[3] = { 0 };

#define LIGHT_LEN 5
int light_pins[5]         = { A4,  A3,  A2,  A1  };
int light_thresholds[5]   = { 960, 960, 960, 960 };
int light_uppers[5]       = { 950, 950, 950, 950 };
int light_lowers[5]       = { 940, 930, 940, 930 };
int light_raws[5]         = { 0 };
bool light_values[5]      = { 0 };
float light_normalized[5] = { 0 };

// left
//   blank = 550
//   black = 840
//   white pla-board = 900
// right
//   blank = 490
//   black = 750
//   white pla-board = 900

void light_setup() {
  for (int i = 0; i < LIGHT_LEN; i ++) {
    pinMode(light_pins[i], INPUT);
  }
}

float normalize(float l, float t, float u, float s) {
  // assume l < t < u and l < s < u
  if (s > t) {
    // Serial.print(s);
    // Serial.print(">");
    // Serial.print(t);
    return (s-t)/(u-t);
  }
  if (s == t) {
    // Serial.print("=");
    return 0;
  }
  if (s < t) {
    // Serial.print(s);
    // Serial.print("<");
    // Serial.print(t);
    return (s-l)/(t-l)-1;
  }
}

void light_read() {
  for (int i = 0; i < LIGHT_LEN; i ++) {
    int raw = analogRead(light_pins[i]);
    // Serial.print(i);
    // Serial.print(" = ");
    // Serial.print(raw);
    // Serial.print(" ; ");
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
  //Serial.println();
}

bool light_is(int a, int b, int c) {
  if (light_values[0] != a && a != -1) return false;
  if (light_values[1] != b && b != -1) return false;
  if (light_values[2] != c && c != -1) return false;
  return true;
}

// #define light_is(a, b, c) light_values[0] == a && light_values[1] == b && light_values[2] == c

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

float get_delta() {
  // find right edge of line
  const int offset = LIGHT_LEN/2;
  for (int i = 0; i < LIGHT_LEN; i ++) {
    Serial.print(light_values[i]);
  }
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
