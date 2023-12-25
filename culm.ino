#include "status.h"
#include "light2.h"
#include "motor.h"
#include "inject.h"
#include "simple.h"

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
  // simple_follow();
  // return;
  follow();
  Serial.println("forward");
  if (light_is(false, false, false)) {
    turn_90();
  }
  if (light_is(true, true, true)) {
    turn_180();
  }
  return;
}

void follow() {
  static float integral = 0;

  float delta = light_normalized[1] - light_normalized[0];
  float proportion = delta;
  integral += delta;
  integral *= 0.9;
  static float prev = delta;
  float derivative = prev-delta;
  // https://www.reddit.com/r/Unity3D/comments/exw4ux/please_help_with_pid_controller_bouncing_back_and/
  //float val = 0.5 - 0.09*proportion - 0*integral - 0.01*derivative;
  float val = 0.5 - 0.05*proportion - 0*integral - 0.0006*derivative;
  if (val < 0) val = 0;
  if (val > 1.0) val = 1.0;
  // Serial.print(" raw");
  // Serial.print(light_raws[0]);
  // Serial.print(" delta");
  // Serial.print(delta);
  // Serial.print(" p");
  // Serial.print(proportion);
  // Serial.print(" i");
  // Serial.print(integral);
  // Serial.print(" d");
  // Serial.print(derivative);
  // Serial.print(" move ");
  // Serial.println(val);
  motor_move(val);
}

void turn_180() {
  while (true) {
    Serial.print("uturn ");
    inject();
    light_read();
    bool all_white = light_is(true, true, true);
    Serial.println(all_white);
    if (all_white) {
      motor_write(0, -motor_coeffLeft*0.6);
      motor_write(1, motor_coeffRight*0.5);
    } else {
      motor_write(0, 0);
      motor_write(1, 0);
      return;
    }
  }
}

// Proposed turn_90 algorithm
// - use the same as u-turn
// - to detect if we're on a straight line, record the last 100 ms of movement, and see if we're moving more right or light → if the left/right are almost the same, assume we are on straight line and finish
void turn_90() {
  motor_write(0, -motor_coeffLeft);
  motor_write(1, -motor_coeffRight);
  delay(100);
  motor_move(1.0);
  while (true) {
    Serial.println("turnright");
    inject();
    light_read();
    if (light_is(true, true, true)) {
      motor_write(0, -motor_coeffLeft);
      motor_write(1, motor_coeffRight);
      delay(100);
      return;
    } else {
      motor_write(0, motor_coeffLeft);
      motor_write(1, -motor_coeffRight);
    }
    delay(10);
  }
}
