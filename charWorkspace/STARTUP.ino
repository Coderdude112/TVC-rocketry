/** Sets up all pin related mappings */
void setupPinMappings () {
    // Define pins as input or output
    pinMode(rLED, OUTPUT);
    pinMode(gLED, OUTPUT);
    pinMode(bLED, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(GPIO1, OUTPUT);
    pinMode(GPIO2, OUTPUT);

    // Attach the servos to pins
    servoX.attach(servoXPin);
    servoY.attach(servoYPin);
}

// ---

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