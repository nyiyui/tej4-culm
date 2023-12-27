float coeff_proportion = 0.05;
float coeff_integral = 0;
float coeff_derivative = 0.01;
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
        Serial.print("get_delta = ");
        Serial.println(d);
        if (d > 0)
          strip.setPixelColor(7, 0, d/2*255, 0);
        else
          strip.setPixelColor(7, d/2*255, 0, 0);
        if (Serial.available()) {
          int ctl = Serial.read();
          if (ctl == 'p') {
            break;
          } else if (ctl == 'c') {
            motor_write(0, 0);
            motor_write(1, 0);
            light_calibration_mode();
          } else if (ctl == 'R') {
            coeff_proportion += diff;
            Serial.print("coeff_proportion = ");
            Serial.println(coeff_proportion);
          } else if (ctl == 'r') {
            coeff_proportion -= diff;
            Serial.print("coeff_proportion = ");
            Serial.println(coeff_proportion);
          } else if (ctl == 'I') {
            coeff_integral += diff;
            Serial.print("coeff_integral = ");
            Serial.println(coeff_integral);
          } else if (ctl == 'i') {
            coeff_integral -= diff;
            Serial.print("coeff_integral = ");
            Serial.println(coeff_integral);
          } else if (ctl == 'D') {
            coeff_derivative += diff;
            Serial.print("coeff_derivative = ");
            Serial.println(coeff_derivative);
          } else if (ctl == 'd') {
            coeff_derivative -= diff;
            Serial.print("coeff_derivative = ");
            Serial.println(coeff_derivative);
          }
        }
      }
      Serial.println("resume");
    } else if (ctl == 'c') {
      motor_write(0, 0);
      motor_write(1, 0);
      light_calibration_mode();
    } else if (ctl == 'R') {
      coeff_proportion += diff;
      Serial.print("coeff_proportion = ");
      Serial.println(coeff_proportion);
    } else if (ctl == 'r') {
      coeff_proportion -= diff;
      Serial.print("coeff_proportion = ");
      Serial.println(coeff_proportion);
    } else if (ctl == 'I') {
      coeff_integral += diff;
      Serial.print("coeff_integral = ");
      Serial.println(coeff_integral);
    } else if (ctl == 'i') {
      coeff_integral -= diff;
      Serial.print("coeff_integral = ");
      Serial.println(coeff_integral);
    } else if (ctl == 'D') {
      coeff_derivative += diff;
      Serial.print("coeff_derivative = ");
      Serial.println(coeff_derivative);
    } else if (ctl == 'd') {
      coeff_derivative -= diff;
      Serial.print("coeff_derivative = ");
      Serial.println(coeff_derivative);
    } else if (ctl == 'P') {
      deltaBase += diff;
      Serial.print("deltaBase = ");
      Serial.println(deltaBase);
    } else if (ctl == 'p') {
      deltaBase -= diff;
      Serial.print("deltaBase = ");
      Serial.println(deltaBase);
    }
  }
}

