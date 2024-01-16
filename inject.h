/*
 * inject.h contains procedures to allow pausing the robot during operation.
 * Ken Shibata
 * Date: 2024-01-16
 * Teacher: Mr. Wong
 */
float coeff_derivative = 0.05;
float deltaBase = 1.3;
const float diff = 0.01;

void inject() {
  if (Serial.available()) {
    int ctl = Serial.read();
    if (ctl == 'p') {
      Serial.println("pause");
      motor_write(0, 0);
      motor_write(1, 0);
      while (true) {
        light_read();
        float d = get_delta();
        Serial.print("delta = ");
        Serial.println(d);
        if (d > 0)
          strip.setPixelColor(7, 0, d/3*255, 0);
        else
          strip.setPixelColor(7, d/3*255, 0, 0);
        if (Serial.available()) {
          int ctl = Serial.read();
          if (ctl == 'p') {
            break;
          } else if (ctl == 'c') {
            motor_write(0, 0);
            motor_write(1, 0);
            light_calibration_mode();
          } else if (ctl == 'D') {
            coeff_derivative += diff;
            Serial.print("coeff_derivative = ");
            Serial.println(coeff_derivative);
          } else if (ctl == 'd') {
            coeff_derivative -= diff;
            Serial.print("coeff_derivative = ");
            Serial.println(coeff_derivative);
          } else if (ctl == 'h') {
            motor_move2(-0.3, 0, 1.0);
          } else if (ctl == 'j') {
            motor_move2(0, -0.3, 1.0);
          } else if (ctl == 'k') {
            motor_move2(0, 0.3, 1.0);
          } else if (ctl == 'l') {
            motor_move2(0.3, 0, 1.0);
          } else if (ctl == 's') {
            motor_move2(0, 0, 1.0);
          }
        }
      }
      Serial.println("resume");
    }
  }
}

