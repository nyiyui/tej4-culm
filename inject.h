void inject() {
  if (Serial.available()) {
    int ctl = Serial.read();
    if (ctl == 'p') {
      Serial.println("pause");
      motor_write(0, 0);
      motor_write(1, 0);
      while (true) {
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

