struct motor {
  int pin_enable;
  int pin_a;
  int pin_b;
};

#define MOTORS_LEN 2
struct motor motors[] = {
  { .pin_enable = 3, .pin_a = 2, .pin_b = 4 },
  { .pin_enable = 5, .pin_a = 6, .pin_b = 7 },
};

#define READY_LED_PIN 9

void motor_write(int i, int power) {
  if (i == 1) power = -power;
  power = -power;
  struct motor m = motors[i];
  // The motor should be either hi-Z or our desired state, never braking
  digitalWrite(m.pin_enable, LOW);
  digitalWrite(m.pin_a, power > 0);
  digitalWrite(m.pin_b, power < 0);
  analogWrite(m.pin_enable, abs(power));
}

float motor_coeffLeft = 101;
float motor_coeffRight = 128;

#define L 0
#define R 1
void motor_move(float bias) {
  int left = motor_coeffLeft*bias;
  int right = motor_coeffRight*(1-bias);
  //Serial.print("motor_move ");
  //Serial.print(bias);
  //Serial.print(" ");
  //Serial.print(left);
  //Serial.print(" ");
  //Serial.println(right);
  motor_write(L, left);
  motor_write(R, right);
}

void motor_move2(float bias, float coeff) {
  int left = coeff*motor_coeffLeft*bias;
  int right = coeff*motor_coeffRight*(1-bias);
  //Serial.print("motor_move ");
  //Serial.print(bias);
  //Serial.print(" ");
  //Serial.print(left);
  //Serial.print(" ");
  //Serial.println(right);
  motor_write(L, left);
  motor_write(R, right);
}
#undef L
#undef R
