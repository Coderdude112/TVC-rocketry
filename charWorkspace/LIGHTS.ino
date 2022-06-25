void whiteFlash() {
    OffTime = 900;
    OnTime = 100;

    unsigned long currentMillis = millis();  // check to see if it's time to change the state of the LED

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        off();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        white();
        analogWrite(buzz, buzzVol);
    }


}

void redFlash()
{

    OffTime = 1900;
    OnTime = 100;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        red();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        white();
        analogWrite(buzz, buzzVol);
    }


}

void yellowFlash()
{

    OffTime = 200;
    OnTime = 200;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        yellow();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        white();
        analogWrite(buzz, buzzVol);
    }


}

void greenFlash()
{

    OffTime = 900;
    OnTime = 100;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        green();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        white();
        analogWrite(buzz, buzzVol);
    }


}

void cyanFlash()
{

    OffTime = 900;
    OnTime = 100;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        cyan();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        white();
        analogWrite(buzz, buzzVol);
    }


}

void blueFlash()
{

    OffTime = 900;
    OnTime = 100;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        blue();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        white();
        analogWrite(buzz, buzzVol);
    }


}

void magentaFlash()
{

    OffTime = 900;
    OnTime = 100;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        magenta();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        white();
        analogWrite(buzz, buzzVol);
    }


}

void cyanGreenFlash()
{

    OffTime = 900;
    OnTime = 100;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        cyan();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        green();
        analogWrite(buzz, buzzVol);
    }


}

void redYellowFlash()
{

    OffTime = 1000;
    OnTime = 1000;


    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();

    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        red();
        analogWrite(buzz, LOW);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        yellow();
        analogWrite(buzz, buzzVol);
    }


}

void off()
{
    digitalWrite(Rled, HIGH);
    digitalWrite(Gled, HIGH);
    digitalWrite(Bled, HIGH);
}

void white()
{
    digitalWrite(Rled, LOW);
    digitalWrite(Gled, LOW);
    digitalWrite(Bled, LOW);
}

void red()
{
    digitalWrite(Rled, LOW);
    digitalWrite(Gled, HIGH);
    digitalWrite(Bled, HIGH);
}

void yellow()
{
    digitalWrite(Rled, LOW);
    digitalWrite(Gled, LOW);
    digitalWrite(Bled, HIGH);
}

void green()
{
    digitalWrite(Rled, HIGH);
    digitalWrite(Gled, LOW);
    digitalWrite(Bled, HIGH);
}

void cyan()
{
    digitalWrite(Rled, HIGH);
    digitalWrite(Gled, LOW);
    digitalWrite(Bled, LOW);
}

void blue()
{
    digitalWrite(Rled, HIGH);
    digitalWrite(Gled, HIGH);
    digitalWrite(Bled, LOW);
}

void magenta()
{
    digitalWrite(Rled, LOW);
    digitalWrite(Gled, HIGH);
    digitalWrite(Bled, LOW);
}
