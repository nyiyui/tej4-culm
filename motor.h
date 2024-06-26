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
  if (power == 0) {
    digitalWrite(m.pin_enable, LOW);
    digitalWrite(m.pin_a, LOW);
    digitalWrite(m.pin_b, LOW);
    digitalWrite(m.pin_enable, HIGH);
  } else {
    digitalWrite(m.pin_enable, LOW);
    digitalWrite(m.pin_a, power > 0);
    digitalWrite(m.pin_b, power < 0);
    analogWrite(m.pin_enable, abs(power));
  }
}

float motor_coeffLeft = 101;
float motor_coeffRight = 140;

float motor_coeffLeft_normal = motor_coeffLeft*255/max(motor_coeffLeft, motor_coeffRight);
float motor_coeffRight_normal = motor_coeffRight*255/max(motor_coeffLeft, motor_coeffRight);

#define L 0
#define R 1
void motor_move(float bias) {
  int left = motor_coeffLeft*bias;
  int right = motor_coeffRight*(1-bias);
  motor_write(L, left);
  motor_write(R, right);
}

void motor_move2(float dir, float straight, float coeff) {
  // dir is [-1, 1] (0 = straight)
  Serial.print("motor_move2 ");
  Serial.print(dir);
  Serial.print(" ");
  Serial.print(straight);
  Serial.print(" ");
  Serial.println(coeff);
  float left = dir + straight*(1-abs(dir));
  float right = -dir + straight*(1-abs(dir));
  motor_write(L, coeff*motor_coeffLeft_normal*left);
  motor_write(R, coeff*motor_coeffRight_normal*right);
}
#undef L
#undef R
