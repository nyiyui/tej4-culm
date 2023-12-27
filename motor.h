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

float motor_coeffLeft_normal = motor_coeffLeft*255/max(motor_coeffLeft, motor_coeffRight);
float motor_coeffRight_normal = motor_coeffRight*255/max(motor_coeffLeft, motor_coeffRight);

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

void motor_move2(float dir, float straight, float coeff) {
  // dir is [-1, 1] (0 = straight)
  float left = dir + straight*(1-abs(dir));
  float right = -dir + straight*(1-abs(dir));
  // Serial.print(dir);
  // Serial.print("dir ; ");
  // Serial.print(abs(dir));
  // Serial.print("abs ; ");
  // Serial.print(1-abs(dir));
  // Serial.print("s ; ");
  // Serial.print(dir+1-abs(dir));
  // Serial.print("l ; ");
  // Serial.print(-dir+1-abs(dir));
  // Serial.print("r ; ");
  // Serial.print(left);
  // Serial.print(" ; ");
  // Serial.print(right);
  // Serial.print(" ; ");
  motor_write(L, coeff*motor_coeffLeft_normal*left);
  motor_write(R, coeff*motor_coeffRight_normal*right);
}
#undef L
#undef R
