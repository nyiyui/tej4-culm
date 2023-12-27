#include "status.h"
#include "light2.h"
#include "motor.h"
#include "inject.h"
#include "simple.h"

// ensure LDR is on lower side of chassis
void setup() {
  status_setup();
  Serial.begin(9600);
  strip.fill(255, 255, 255);
  strip.show();
  delay(1000);
  strip.fill(0, 255, 0);
  strip.show();
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
  if (light_is(false, false, false)) {
    turn_90();
  }
  if (light_is(true, true, true)) {
    turn_180();
  }
  return;
}

void follow() {
  strip.setPixelColor(STATUS_MODE, 255, 0, 0);
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
  strip.setPixelColor(STATUS_MODE, 0, 255, 0);
  unsigned long latestRight = millis();
  while (true) {
    inject();
    light_read();
    float delta = light_normalized[2];
    Serial.print("delta = ");
    Serial.println(delta);
    if (delta > 0)
      strip.setPixelColor(STATUS_SUBMODE, 0, (int) (delta * 255), 0);
    else if (delta < 0)
      strip.setPixelColor(STATUS_SUBMODE, (int) (delta * 255), 0, 0);
    if (delta > 0) { // on white, turn left-ish
      float power = abs(delta);
      if (power < 0.5) power = 0.5;
      motor_write(0, power * -0.4 * motor_coeffLeft_normal);
      motor_write(1, power *  1.0 * motor_coeffRight_normal);
      // if this goes on for more than 500ms, then return
      if (latestRight <= millis() - 500) {
        return;
      }
    } else if (delta <= 0) { // on black, turn right
      float power = abs(delta);
      if (power < 0.5) power = 0.5;
      motor_write(0, power *  0.4 * motor_coeffLeft_normal);
      motor_write(1, power * -1.0 * motor_coeffRight_normal);
      latestRight = millis();
    }
    delay(100);
  }
}

void turn_90() {
  strip.setPixelColor(STATUS_MODE, 0, 0, 255);
  unsigned long latestRight = millis();
  while (true) {
    inject();
    light_read();
    float delta = light_normalized[2];
    Serial.print("delta = ");
    Serial.println(delta);
    if (delta > 0)
      strip.setPixelColor(STATUS_SUBMODE, 0, (int) (delta * 255), 0);
    else if (delta < 0)
      strip.setPixelColor(STATUS_SUBMODE, (int) (delta * 255), 0, 0);
    if (delta > 0) { // on white, turn left-ish
      float power = abs(delta);
      if (power < 0.5) power = 0.5;
      motor_write(0, power * 0.9 * motor_coeffLeft);
      motor_write(1, power * 1.0 * motor_coeffRight);
      // if this goes on for more than 500ms, then return
      if (latestRight <= millis() - 500) {
        return;
      }
    } else if (delta <= 0) { // on black, turn right
      float power = abs(delta);
      if (power < 0.5) power = 0.5;
      motor_write(0, power *  0.9 * motor_coeffLeft);
      motor_write(1, power * -1.0 * motor_coeffRight);
      latestRight = millis();
    }
    delay(100);
  }
}
