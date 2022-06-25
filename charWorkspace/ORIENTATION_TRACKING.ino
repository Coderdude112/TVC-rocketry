






void getTelemetry()
{
  //    Serial.print(F("Temperature = "));
  //    Serial.print(bmp.readTemperature());
  //    Serial.println(" *C");

  //    Serial.print(F("Pressure = "));
  //    Serial.print(bmp.readPressure());
  //    Serial.println(" Pa");

  //    Serial.print(F("Approx altitude = "));
  //    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
  //    Serial.println(" m");


  baroAlt = bmp.readAltitude(1013.25);
  trueAlt = baroAlt - baroOffset;

  if (trueAlt > maxAlt)
  {
    maxAlt = trueAlt;
  }

  lsm.read();  /* ask it to read in the data */

  /* Get a new sensor event */
  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp);

  //Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.println(" m/s^2");
  //Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
  //Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.print(" m/s^2 ");

  //Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" rad/s");
  //Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" rad/s");
  //Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.print(" rad/s");

  accelX = a.acceleration.y;
  accelY = a.acceleration.z;
  accelZ = a.acceleration.x;

  gyroX = g.gyro.y;
  gyroY = g.gyro.z;
  gyroZ = g.gyro.x;

  updateOrientation();

  EulerAngles angles = quatToEuler(orientation);

  yaw = angles.yaw * RAD_TO_DEG;
  pitch = angles.pitch * RAD_TO_DEG;
  roll = angles.roll * RAD_TO_DEG;


}






// Update rotation, orientation, etc
void updateOrientation() {
  newGyroUpdate = micros();
  gyro_dt = newGyroUpdate - lastGyroUpdate;
  lastGyroUpdate = newGyroUpdate;



  lsm.read();  /* ask it to read in the data */
  sensors_event_t a, m, g, temp;  /* Get a new sensor event */
  lsm.getEvent(&a, &m, &g, &temp);





  float gyro_dt_seconds = (float)gyro_dt / 1000000;

  // Body state conversion + time multiplication
  bX = (g.gyro.x - gxcalout) * gyro_dt_seconds;
  bY = (g.gyro.y - gycalout) * gyro_dt_seconds;
  bZ = (g.gyro.z - gzcalout) * gyro_dt_seconds;

  Quaternion newRotation = Quaternion::from_euler_rotation(bZ, -bY, bX);
  orientation *= newRotation; // Perform orientation rotation





}









EulerAngles quatToEuler(Quaternion q) {
  EulerAngles ret;

  float sinr_cosp = 2 * (q.a * q.b + q.c * q.d);
  float cosr_cosp = 1 - 2 * (q.b * q.b + q.c * q.c);
  ret.roll = atan2(sinr_cosp, cosr_cosp);

  float sinp = 2 * (q.a * q.c + - q.d * q.b);
  if (abs(sinp) >= 1)
    ret.pitch = copysign(PI / 2, sinp); // return 90 if out of range
  else
    ret.pitch = asin(sinp);

  float siny_cosp = 2 * (q.a * q.d + q.b * q.c);
  float cosy_cosp = 1 - 2 * (q.c * q.c + q.d * q.d);
  ret.yaw = atan2(siny_cosp, cosy_cosp);

  return ret;
}






void zero() {
  orientation.a = 1;
  orientation.b = 0;
  orientation.c = 0;
  orientation.d = 0;
};