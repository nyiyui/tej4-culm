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
  //halation();
  generic();
  //steps();
  return;
}

template <typename T> int signum(T val) {
  return (T(0) < val) - (val < T(0));
}

/*
void halation() {
  strip.setPixelColor(STATUS_MODE, 255, 0, 0);
  static float prev;

  float delta = get_delta()/(LIGHT_LEN/2);
  Serial.print(" ");
  Serial.print(delta);
  Serial.print("d");
  Serial.print(" ");
  Serial.print(delta-prev);
  Serial.print("deriv");
  float derivative = delta-prev;
  float dir = 0.5*delta;
  float dd = abs(delta);// max(abs(delta), 10*abs(derivative));
  float straight = -1.33*dd*dd+0.947;
  motor_move2(dir, straight, 1.0);
  prev = delta;
  delay(10);
  return;
}
*/

void generic() {
  strip.setPixelColor(STATUS_MODE, 0, 0, 255);
  static float prev;
  float d = coeff_derivative;

  float delta = get_delta()/(LIGHT_LEN/2);
  /*
  if (abs(delta) >= 1) {
    motor_move2(0, -1.0, 1.0);
    delay(10);
  }
  if (abs(delta) >= 1) {
    steps();
  }
  */
  //Serial.print(" ");
  //Serial.print(delta);
  //Serial.print("d");
  float derivative = delta-prev;
  float dir = 0.35*(delta*delta*delta+delta)+0.1*derivative;
  dir = constrain(dir, -1, 1);
  float straight = 0.8*(0.7-abs(delta));
  straight = max(0, straight);
  motor_move2(dir, straight, 1.0);
  prev = delta;
  delay(40);
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
