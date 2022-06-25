


void startUp()
{



  while (!(calCount >= 250))
  {


    state = 1;
    redFlash();

    lsm.read();  /* ask it to read in the data */

    /* Get a new sensor event */
    sensors_event_t a, m, g, temp;

    lsm.getEvent(&a, &m, &g, &temp);


    if (calCount < 1000) {

      calCount = calCount + 1;

      gxcal = gxcal + g.gyro.x;
      gycal = gycal + g.gyro.y;
      gzcal = gzcal + g.gyro.z;

      //Serial.println(" ");
      //Serial.println(calCount);
      //    Serial.println(gxcal);
      //Serial.println(gycal);
      //Serial.println(gzcal);
      
    }

    gxcalout = gxcal / calCount;
    gycalout = gycal / calCount;
    gzcalout = gzcal / calCount;



  }





  //!!find pad orientation
  //!!calibrate gyro
  //!!calibrate baro
    servoDemo();
}

void padIdle()
{

  launchDetect();
  greenFlash();
  logging();
  
}


void poweredFlight()
{
  launchDetect();
  abortDetect();
  if (accelAverage < 4) {
    burnoutTime = flightTime;
    state = 3;

  }


  //burnoutTimer();



  yellowFlash();


/*
  EulerAngles angles = quatToEuler(orientation);

  yaw = angles.yaw * RAD_TO_DEG;
  pitch = angles.pitch * RAD_TO_DEG;
  roll = angles.roll * RAD_TO_DEG;


*/

  servoOutput();

  logging();
}


void unpoweredAscent()
{
  cyanFlash();
  apogeeDetect();
  apogeeTimer();
  logging();
}

void ballisticDescent()
{
  whiteFlash();


}



void chuteDescent()
{





  if (runOnce == 0)
  {
    pyroTime = flightTime;
    runOnce = 1;
    // Serial.print("HIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
  }

  if (flightTime - pyroTime >= 1000)
  {
    pyroState = false;
    digitalWrite(gpio1, LOW);
  }
  else
  {
    pyroState = true;
    digitalWrite(gpio1, HIGH);
  }


  servoX.write(xMid);
  servoY.write(yMid);
  zOut = 0;

  blueFlash();
  if (flightTime >= 30000) {
    state = 6;
  }

  logging();
}

void landed()
{
  pyroState = false;
  digitalWrite(gpio1, LOW);
  magentaFlash();
  myFile.flush();
  myFile.close();
  //  Serial.print("closed");
}

void safed()
{
  cyanGreenFlash();
}





void batLow()
{
  redYellowFlash();
}







void sdFail()
{
  redYellowFlash();
}










void stateDetect()
{


  if (state == 0) {
    startUp();

  }


  if (state == 1) {
    padIdle();

  }


  if (state == 2) {
    poweredFlight();

  }


  if (state == 3) {
    unpoweredAscent();

  }


  if (state == 4) {
    ballisticDescent();

  }


  if (state == 5) {
    chuteDescent();

  }


  if (state == 6) {
    landed();
  }


  if (state == 7) {
    safed();

  }

  if (state == 8) {
    batLow();

  }
  if (state == 9) {
    sdFail();

  }



  
}







void apogeeDetect()
{
  if (trueAlt + 1 <= maxAlt)
  {
    state = 5;
  }
}




void apogeeTimer()
{
  if (flightTime - burnoutTime >= burnoutDelay)
  {
    state = 5;
  }
}







void burnoutTimer()
{
  if (flightTime >= 3000);
  {
    state = 5;

    servoX.write(xMid);
    servoY.write(yMid);
       
  }
}





void launchDetect()
{
  // subtract the last reading:
  accelTotal = accelTotal - accelReadings[accelReadIndex];
  // read from the sensor:
  accelReadings[accelReadIndex] = accelZ;
  // add the reading to the total:
  accelTotal = accelTotal + accelReadings[accelReadIndex];
  // advance to the next position in the array:
  accelReadIndex = accelReadIndex + 1;

  // if we're at the end of the array...
  if (accelReadIndex >= accelNumReadings) {
    // ...wrap around to the beginning:
    accelReadIndex = 0;
  }

  // calculate the average:
  accelAverage = accelTotal / accelNumReadings;
  // send it to the computer as ASCII digits

  if (accelAverage >= launchAcceleration) {
    state = 2;
    if (zeroed == false)
    {
      zeroed = true;
      zero();
      launchTime = dTime;
    }


  }





}




void abortDetect()
{
  if ((yaw >= 30) or (yaw <= -30) or (pitch >= 30) or (pitch <= -30))
  {
    state = 5;
 
          servoX.write(xMid);
    servoY.write(yMid);
  }
}
