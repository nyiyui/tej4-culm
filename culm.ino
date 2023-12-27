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
  follow();
  return;
}

template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

void follow() {
  const float straightCoeff = 0.3;

  static float prev;
  strip.setPixelColor(STATUS_MODE, 255, 0, 0);
  static float integral = 0;

  float delta_raw = get_delta();
  float delta = signum(delta_raw) * pow(deltaBase, delta_raw) - 1;
  //float delta = delta_raw;
  float proportion = delta;
  integral += delta;
  integral *= 0.9;
  float derivative = prev-delta;
  float dir = coeff_proportion*proportion + coeff_integral*integral + coeff_derivative*derivative;
  prev = delta;
  float straight = straightCoeff*(1-delta/2);
  straight = constrain(straight, 0.2, 1);
  if (straight < 0.2) straight = 0.2;
  if (light_is(false, false, false)) {
    dir = 0.8;
    straight = 0;
    motor_move2(dir, straight, 1.0);
    delay(200);
    return;
  }
  dir = constrain(dir, -1, 1);
  Serial.print("delta = ");
  Serial.print(delta);
  Serial.print(" ; straight = ");
  Serial.print(straight);
  Serial.print(" ; dir = ");
  Serial.println(dir);
  motor_move2(dir, straight, 1.0);
  delay(100);
}
