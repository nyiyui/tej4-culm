/*
 * culm.ino contains the main algorithm controlling the robot.
 * Ken Shibata
 * Date: 2024-01-16
 * Teacher: Mr. Wong
 */
#include "status.h"
#include "light2.h"
#include "motor.h"
#include "inject.h"

// ensure LDR is on lower side of chassis

// setup() sets up the various sensors and modules.
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

// loop() calls generic() and other helper functions.
void loop() {
  inject();
  light_read();
  generic();
  return;
}

// generic() is a generic algorithm to follow the line.
void generic() {
  strip.setPixelColor(STATUS_MODE, 0, 0, 255);
  static float prev;

  float delta = get_delta()/(LIGHT_LEN/2);
  float derivative = delta-prev;
  float dir = 0.35*(1.1*delta*delta*delta+delta)+0.1*derivative;
  dir = constrain(dir, -1, 1);
  float straight = 0.9*(0.7-abs(delta));
  straight = max(0, straight);
  straight = min(0.8, straight);
  motor_move2(dir, straight, 1.0);
  prev = delta;
  delay(40);
  return;
}
