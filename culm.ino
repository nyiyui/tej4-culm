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
  generic();
  //steps();
  return;
}

template <typename T> int signum(T val) {
  return (T(0) < val) - (val < T(0));
}

void generic() {
  static float prev;
  float p = coeff_proportion;
  float d = coeff_derivative;

  float delta = get_delta()/(LIGHT_LEN/2);
  Serial.print(delta);
  Serial.print("d");
  if (abs(delta) > 0.8) {
    motor_move2(0.2 * delta, -0.3, 1.0);
    delay(100);
  } else {
    float straight = 0.4*(1-abs(delta));
    straight = constrain(straight, 0.3, 1);
    motor_move2(p*delta + d*(delta-prev), straight, 1.0);
  }
  prev = delta;
  return;
}
