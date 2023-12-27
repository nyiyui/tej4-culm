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
          }
        }
      }
      Serial.println("resume");
    } else if (ctl == 'c') {
      motor_write(0, 0);
      motor_write(1, 0);
      light_calibration_mode();
    }
  }
}

