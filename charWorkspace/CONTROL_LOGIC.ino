void servoOutput() {
  unsigned long now = millis();
  float timeChange = (double)(now - lastTime);

  // Do some PID math
  errSumX += (pitch * (timeChange / 1000));
  float dErrX = (pitch - lastErrX) / timeChange;
  errSumY += (yaw * (timeChange / 1000));
  float dErrY = (yaw - lastErrY) / timeChange;
  errSumZ += (gyroZ * (timeChange / 1000));
  float dErrZ = (gyroZ - lastErrZ) / timeChange;
  xOut = pitch * kpx + kix * errSumX + kdx * dErrX;
  yOut = yaw * kpy + kiy * errSumY + kdy * dErrY;
  zOut = gyroZ * kpz + kiz * errSumZ + kdz * dErrZ;
  zOutI += (zOut * (timeChange / 1000));


  // Line up the actual servo controls based on how much the rocket has rolled in flight
  float cs = cos(roll * (3.1415 / 180));
  float sn = sin(roll * (3.1415 / 180));
  yOutPR = xOut * cs - yOut * sn;
  xOutPR = yOut * sn + xOut * cs;

  // Ensure the output values yOutPR and xOutPR are between -5 and 5
  yOutPR =  constrain(-yOutPR, -5, 5);
  xOutPR =  constrain(xOutPR, -5, 5);

  // Set the actual servo positions
  servoX.write((xOutPR * gearingX) + xMid);
  servoY.write((yOutPR * gearingY) + yMid);

  // Control the reaction wheel
  if (zOutI > 0) {
    analogWrite(pwm2, abs(zOutI));
    analogWrite(pwm1, 0);
  }

  if (zOutI <= 0) {
    analogWrite(pwm2, 0);
    analogWrite(pwm1, abs(zOutI));
  }
}
