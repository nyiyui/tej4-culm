#define LED_LEFT_PIN 10
#define LED_RIGHT_PIN 11
#include "light2.h"
#include "motor.h"
#include "inject.h"
#include "simple.h"

// ensure LDR is on lower side of chassis
void setup() {
  pinMode(READY_LED_PIN, OUTPUT);
  pinMode(LED_LEFT_PIN, OUTPUT);
  pinMode(LED_RIGHT_PIN, OUTPUT);
  digitalWrite(READY_LED_PIN, HIGH);
  digitalWrite(LED_LEFT_PIN, HIGH);
  digitalWrite(LED_RIGHT_PIN, HIGH);
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
  simple_follow();
  return;
  follow();
  if (light_is(false, false, false)) {
    turn_90();
  }
  if (light_is(true, true, true)) {
    turn_180();
  }
  return;
  // following the right edge
  if (light_is(false, true, true)) {
    motor_move(0.5);
  } else if (light_is(false, false, true)) {
    motor_move(0.6);
  //} else if (light_is(false, false, false)) {
  //delay(1000);
  //  motor_move(1.0);
  //  unsigned long until = millis() + 3000;
  //  while (true) {
  //    unsigned long now = millis();
  //    light_read();
  //    Serial.print(now); // TODO here:
  //    Serial.print(" > ");
  //    Serial.print(until);
  //    Serial.println("...");
  //    delay(1);
  //    if (light_is(true, false, true) || now > until) break;
  //  }
  //  motor_move(0.5);
  //} else if (light_is(true, true, true)) {
  //  motor_write(0, 0);
  //  motor_write(1, 0);
  //  delay(1000);
  //  motor_move(1.0);
  //  Serial.print("start");
  //  while (true) {
  //    light_read();
  //    Serial.print(light_is(true, false, true));
  //    Serial.println("...");
  //    delay(1);
  //    if (light_is(true, false, true)) {
  //      delay(1000);
  //      Serial.print("aiya");
  //    }
  //  }
  //  motor_write(0, 0);
  //  motor_write(1, 0);
  //  delay(1000);
  //  motor_move(0.5);
  } else if (light_is(true, false, true)) {
    motor_move(0.6);
  } else {
    motor_move(0.5);
  }
  delay(10);
}

void follow() {
  static float integral = 0;

  // float delta = normalize(light_lowers[0], light_thresholds[0], light_uppers[0], light_raws[0]);
  float delta = light_normalized[1] - light_normalized[0];
  float proportion = delta;
  integral += delta;
  integral *= 0.9;
  static float prev = delta;
  float derivative = prev-delta;
  //float val = 0.5 - 0.05*proportion - 0.001*integral - 0.001*derivative;
  //float val = 0.5 - 0.1*proportion - 0*integral - 0.01*derivative;
  // https://www.reddit.com/r/Unity3D/comments/exw4ux/please_help_with_pid_controller_bouncing_back_and/
  //float val = 0.5 - 0.09*proportion - 0*integral - 0.01*derivative;
  float val = 0.5 - 0.05*proportion - 0*integral - 0.0006*derivative;
  if (val < 0) val = 0;
  if (val > 1.0) val = 1.0;
  Serial.print(" raw");
  Serial.print(light_raws[0]);
  Serial.print(" delta");
  Serial.print(delta);
  Serial.print(" p");
  Serial.print(proportion);
  Serial.print(" i");
  Serial.print(integral);
  Serial.print(" d");
  Serial.print(derivative);
  Serial.print(" move ");
  Serial.println(val);
  motor_move(val);
}

void turn_180() {
  while (true) {
    inject();
    light_read();
    bool all_white = light_is(true, true, true);
    if (all_white) {
      motor_write(0, -motor_coeffLeft*0.5);
      motor_write(1, motor_coeffRight*0.5);
    } else {
      motor_write(0, 0);
      motor_write(1, 0);
      while (true) {}
    }
  delay(1000);
  return;
  }
}

void turn_90() {
  motor_write(0, -motor_coeffLeft);
  motor_write(1, -motor_coeffRight);
  delay(200);
  motor_move(1.0);
  while (true) {
    inject();
    light_read();
    if (light_is(true, false, true)) {
      motor_write(0, motor_coeffLeft);
      motor_write(1, -motor_coeffRight);
    } else if (light_is(false, true, true)) {
      motor_write(0, 0);
      motor_write(1, 0);
      return;
    }
    delay(10);
  }/*
  motor_write(0, 36);
  motor_write(1, -36);
  delay(2000);
  motor_write(1, 0);
  motor_write(0, 0);*/
}
