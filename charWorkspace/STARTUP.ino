
void servoDemo()
{

  int tvcmin = -15;
  int tvcmax = 15;
  int servoWait = 100;

  servoX.write(xMid);
  servoY.write(yMid);
  delay(servoWait);
  servoX.write(xMid);
  servoY.write(tvcmin + yMid);
  delay(servoWait);
  servoX.write(tvcmax + xMid);
  servoY.write(tvcmin + yMid);
  delay(servoWait);
  servoX.write(tvcmax + xMid);
  servoY.write(yMid);
  delay(servoWait);
  servoX.write(tvcmax + xMid);
  servoY.write(tvcmax + yMid);
  delay(servoWait);
  servoX.write(xMid);
  servoY.write(tvcmax + yMid);
  delay(servoWait);
  servoX.write(tvcmin + xMid);
  servoY.write(tvcmax + yMid);
  delay(servoWait);
  servoX.write(tvcmin + xMid);
  servoY.write(yMid);
  delay(servoWait);
  servoX.write(tvcmin + xMid);
  servoY.write(tvcmin + yMid);
  delay(servoWait);
  servoX.write(xMid);
  servoY.write(tvcmin + yMid);
  delay(servoWait);
  servoX.write(xMid);
  servoY.write(yMid);
  delay(servoWait);

}







void pinModes ()
{
  pinMode(Rled, OUTPUT);
  pinMode(Gled, OUTPUT);
  pinMode(Bled, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(gpio1, OUTPUT);
  pinMode(gpio2, OUTPUT);
  servoX.attach(21);
  servoY.attach(20);

}

void setupSensor()
{


  for (int accelThisReading = 0; accelThisReading < accelNumReadings; accelThisReading++) {
    accelReadings[accelThisReading] = 0;
  }






  Serial.println("LSM9DS1 data read demo");

  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");



  // 1.) Set the accelerometer range
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);

  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}







void startBaro()
{


  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_125); /* Standby time. */




  baroOffset = bmp.readAltitude(1013.25);
}
