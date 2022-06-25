void startUp() {
    while (!(calCount >= 250)) { // Some sort of calibration
        state = 1;
        redFlash();

        lsm.read();  // ask it to read in the data
        sensors_event_t a, m, g, temp; // Get a new sensor event
        lsm.getEvent(&a, &m, &g, &temp);

        if (calCount < 1000) {

            calCount = calCount + 1;

            gxcal = gxcal + g.gyro.x;
            gycal = gycal + g.gyro.y;
            gzcal = gzcal + g.gyro.z;
        }

        gxcalout = gxcal / calCount;
        gycalout = gycal / calCount;
        gzcalout = gzcal / calCount;
    }

    // @TODO: find pad orientation
    servoDemo();
}

void padIdle() {
    launchDetect();
    greenFlash();
    logging();
}


void poweredFlight() {
    launchDetect(); // Acts as a way to take the running average of the acceleration
    abortDetect();

    // Detect burnout
    if (accelAverage < 4) {
        burnoutTime = flightTime;
        state = 3;
    }

    yellowFlash();


    /* EulerAngles angles = quatToEuler(orientation);
     *
     * yaw = angles.yaw * RAD_TO_DEG;
     * pitch = angles.pitch * RAD_TO_DEG;
     * roll = angles.roll * RAD_TO_DEG;
     */

    servoOutput();
    logging();
}

void unpoweredAscent() {
    cyanFlash();
    apogeeDetect();
    apogeeTimer();
    logging();
}

void ballisticDescent() {
    whiteFlash();
}

void chuteDescent() {
    // Turn on the pyro charge
    if (runOnce == 0) {
        pyroTime = flightTime;
        runOnce = 1;
    }

    // Keep the pyro charge lines on for 1 sec
    if (flightTime - pyroTime >= 1000) {
        pyroState = false;
        digitalWrite(gpio1, LOW);
    } else {
        pyroState = true;
        digitalWrite(gpio1, HIGH);
    }

    // Reset the TVC mount
    servoX.write(xMid);
    servoY.write(yMid);
    zOut = 0;

    blueFlash();
    if (flightTime >= 30000) { // Stay in state 5 until 30 sec have passed from launch
        state = 6;
    }

    logging();
}

void landed() {
    pyroState = false;
    digitalWrite(gpio1, LOW);
    magentaFlash();
    myFile.flush();
    myFile.close();
}

void safed() {
    cyanGreenFlash();
}

void batLow() {
    redYellowFlash();
}

void sdFail() {
    redYellowFlash();
}


// Runs a method depending on the current state
void stateDetect() {
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

// Currently not functional due to barometer problems
void apogeeDetect() {
    if (trueAlt + 1 <= maxAlt) { state = 5; }
}

// Calculates weather we are ready to go from state 3 to 4
void apogeeTimer() {
    if (flightTime - burnoutTime >= burnoutDelay) { state = 5; }
}

// @TODO: Could be deprecated
void burnoutTimer() {
    if (flightTime >= 3000); // TODO: Remove this semi-colin
    {
        state = 5;

        servoX.write(xMid);
        servoY.write(yMid);

    }
}

void launchDetect() {
    accelTotal = accelTotal - accelReadings[accelReadIndex]; // subtract the last reading:
    accelReadings[accelReadIndex] = accelZ; // read from the sensor acceleration Z
    accelTotal = accelTotal + accelReadings[accelReadIndex]; // add the reading to the total

    // Possibly deprecated code
    // === BEGIN ===
    accelReadIndex = accelReadIndex + 1;

    // if we're at the end of the array...
    if (accelReadIndex >= accelNumReadings) {
        // ...wrap around to the beginning:
        accelReadIndex = 0;
    }
    // === END ===

    accelAverage = accelTotal / accelNumReadings; // calculate the average

    if (accelAverage >= launchAcceleration) { // Detect a launch
        state = 2;
        if (zeroed == false) {
            zeroed = true;
            zero();
            launchTime = dTime;
        }
    }
}

void abortDetect() {
    if ((yaw >= 30) or (yaw <= -30) or (pitch >= 30) or (pitch <= -30)) {
        state = 5;

        servoX.write(xMid);
        servoY.write(yMid);
    }
}
