void simple_follow() {
  while (true) {
    inject();
    light_read();
    static float integral = 0;

    // float delta = normalize(light_lowers[0], light_thresholds[0], light_uppers[0], light_raws[0]);
    //float delta = 2*(light_normalized[0]+1) + light_normalized[1] + 2*(light_normalized[2]-1);
    //float delta = 0;
    //if (light_is(true, true, true)) {
    //  delta = -1;
    //} else if (light_is(false, true, true)) {
    //  delta = 0;
    //} else if (light_is(false, false, true)) {
    //  delta = 1;
    //} else if (light_is(false, false, false)) {
    //  delta = 2;
    //}
    float delta = -2*(light_normalized[0]) + light_normalized[1] + 2*(light_normalized[2]);
    //delta = delta * delta * delta;
    //delta = -1*delta;
    if (delta > 3) delta *= 4;
    if (delta < -3) delta *= 4;
    float proportion = delta;
    integral += delta;
    integral *= 0.9;
    static float prev = delta;
    float derivative = prev-delta;
    //float val = 0.5 - 0.05*proportion - 0.001*integral - 0.001*derivative;
    //float val = 0.5 - 0.1*proportion - 0*integral - 0.01*derivative;
    // https://www.reddit.com/r/Unity3D/comments/exw4ux/please_help_with_pid_controller_bouncing_back_and/
    //float val = 0.5 - 0.09*proportion - 0*integral - 0.01*derivative;
    float val = 0.05*proportion - 0*integral - 0.0006*derivative;
    if (val < 0) val = 0;
    if (val > 1.0) val = 1.0;
    Serial.print(" raw");
    Serial.print(light_raws[0]);
    Serial.print(" delta");
    Serial.print(delta);
    Serial.print(" p");
    Serial.print(proportion);
    Serial.print(" i");
    Serial.print(integral);
    Serial.print(" d");
    Serial.print(derivative);
    Serial.print(" move ");
    Serial.println(val);
    motor_move2(val, 0.5);
  }
}
