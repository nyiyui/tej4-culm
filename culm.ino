#include "status.h"
#include "light2.h"
#include "motor.h"
#include "inject.h"

// ensure LDR is on lower side of chassis
void setup() {
  status_setup();
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("mizuki v0.1");

  Serial.print("Using ");
  Serial.print(MOTORS_LEN);
  Serial.println(" motor(s):");
  for (size_t i = 0; i < MOTORS_LEN; i ++) {
    struct motor m = motors[i];
    Serial.print("i:");
    Serial.print(i);
    Serial.print(" en:");
    Serial.print(m.pin_enable);
    Serial.print(" a:");
    Serial.print(m.pin_a);
    Serial.print(" b:");
    Serial.println(m.pin_b);
  }
  for (size_t i = 0; i < MOTORS_LEN; i ++) {
    struct motor m = motors[i];
    pinMode(m.pin_enable, OUTPUT);
    pinMode(m.pin_a, OUTPUT);
    pinMode(m.pin_b, OUTPUT);
  }
  light_setup();
  Serial.println("[ OK ] pin setup: done");
}

void loop() {
  inject();
  light_read();
  halation();
  //generic();
  //steps();
  return;
}

template <typename T> int signum(T val) {
  return (T(0) < val) - (val < T(0));
}

void halation() {
  strip.setPixelColor(STATUS_MODE, 255, 0, 0);
  static float prev;

  float delta = get_delta()/(LIGHT_LEN/2);
  Serial.print(" ");
  Serial.print(delta);
  Serial.print("d");
  float derivative = delta-prev;
  float dir = 0.4*(0.3*delta*delta*delta + delta);
  float straight = 0.6*(0.8-abs(delta));
  motor_move2(dir, straight, 1.0);
  prev = delta;
  return;
}

void generic() {
  strip.setPixelColor(STATUS_MODE, 0, 0, 255);
  static float prev;
  float d = coeff_derivative;

  float delta = get_delta()/(LIGHT_LEN/2);
  if (abs(delta) >= 1 || abs(delta-prev) > 0.5) {
    steps();
  }
  Serial.print(" ");
  Serial.print(delta);
  Serial.print("d");
  float p = pow(1.4, abs(delta)) - 0.9;
  //float straight = pow(0.516698, abs(delta)*abs(delta)) - 0.554843;
  float derivative = delta-prev;
  float straight = pow(0.00000154966, abs(delta)*abs(delta));
  float dir = p*delta + d*(delta-prev);
  const float cumMin = 0.3;
  if (abs(straight) + abs(dir) < cumMin) {
    if (abs(straight) > abs(dir)) {
      straight = constrain(straight, cumMin, 1);
    } else {
      dir = constrain(dir, cumMin, 1);
    }
  }
  motor_move2(dir, straight, 1.0);
  prev = delta;
  return;
}

void steps() {
  strip.setPixelColor(STATUS_MODE, 255, 255, 255);
  strip.show();
  float deltaInitial = light_normalized[2];
  float history = abs(deltaInitial);
  float prev = deltaInitial;
  while (true) {
    Serial.println(history);
    {
      float brightness = 255*abs(history);
      strip.setPixelColor(STATUS_SUBMODE, brightness, brightness, brightness);
      strip.show();
    }
    inject();
    light_read();
    float delta = get_delta()/(LIGHT_LEN/2);
    float dir = 0.4*delta + 0.05*(delta-prev);
    float straight = -0.167*abs(delta)*abs(delta) - 0.183*abs(delta) + 0.4;
    straight *= 1.5;
    straight = abs(straight);
    motor_move2(dir, straight, 1.0);
    history = 0.9*history + 0.1*(delta);
    if (abs(history) < 0.2) {
      motor_move2(0, 0, 0);
      return;
    }
    prev = delta;
  }
}
