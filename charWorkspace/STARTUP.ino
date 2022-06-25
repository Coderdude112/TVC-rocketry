// Tests the TVC positioning at rocket startup
void servoDemo() {
  // Maximums for movement
  int tvcmin = -15;
  int tvcmax = 15;
  int servoWait = 100; // Delay between moves

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

// Handles setting up pin related stuff
void pinModes () {
  // Define pins as input or output
  pinMode(Rled, OUTPUT);
  pinMode(Gled, OUTPUT);
  pinMode(Bled, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(gpio1, OUTPUT);
  pinMode(gpio2, OUTPUT);

  // Attach the servos to pins
  servoX.attach(21);
  servoY.attach(20);
}

// Handles setting up the IMU
void setupSensor() {
  for (int accelThisReading = 0; accelThisReading < accelNumReadings; accelThisReading++) { // Takes the average of the acceleration data
    accelReadings[accelThisReading] = 0;
  }
  Serial.println("LSM9DS1 data read demo");

  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin()) {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }

  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G); // Sets the accelerometer range. Could also replace `LSM9DS1_ACCELRANGE_16G` with: `LSM9DS1_ACCELRANGE_2G`, `LSM9DS1_ACCELRANGE_4G`, or `LSM9DS1_ACCELRANGE_8G`
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS); // Sets the magnetometer sensitivity. Could also replace `LSM9DS1_MAGGAIN_4GAUSS` with: `LSM9DS1_MAGGAIN_8GAUSS`, `LSM9DS1_MAGGAIN_12GAUSS`, or `LSM9DS1_MAGGAIN_16GAUSS`
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS); // Sets the gyroscope range. Could also replace `LSM9DS1_GYROSCALE_245DPS` with: `LSM9DS1_GYROSCALE_500DPS` or `LSM9DS1_GYROSCALE_2000DPS`
}

void startBaro() {
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!")); // TODO: Remove the F()
    while (1) delay(10); // TODO: Remove the delay(10)
  }

  // Setup the BARO using data from the datasheet
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode
                  Adafruit_BMP280::SAMPLING_X2,     // Tempeture oversampling
                  Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                  Adafruit_BMP280::FILTER_X16,      // Filtering
                  Adafruit_BMP280::STANDBY_MS_125); // Standby time

  baroOffset = bmp.readAltitude(1013.25);
}
