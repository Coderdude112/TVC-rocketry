// Constants
unsigned const int onTime = 100; // milliseconds of LED on-time
unsigned const int offTime = 700;
unsigned const int buzzerVolume = 10; // For best results keep this between 1 and 15

// Other variables
unsigned long previousMills = 0; // Last time the LED was updated
int ledState = LOW; // @TODO: Figure out what this is

void flashLED(String color) {
    unsigned long currentMills = millis();

    if ((ledState == HIGH) && (currentMills - previousMills >= onTime)) {
        ledState = LOW;
        previousMills = currentMills;
        analogWrite(buzzer, 0);

        // Turn off the LEDs
        digitalWrite(Rled, HIGH);
        digitalWrite(Gled, HIGH);
        digitalWrite(Bled, HIGH);
    } else if ((ledState == LOW) && (currentMills - previousMills >= offTime)) {
        ledState = HIGH;
        previousMills = currentMills;
        analogWrite(buzzer, buzzerVolume); // Comment out to disable beeping

        // Set the LEDs
        if ((color == "white") || (color == "red") || (color == "yellow") || (color == "magenta")) {
            digitalWrite(Rled, LOW);
        } else {
            digitalWrite(Rled, HIGH);
        }

        if ((color == "white") || (color == "yellow") || (color == "green") || (color == "cyan")) {
            digitalWrite(Gled, LOW);
        } else {
            digitalWrite(Gled, HIGH);
        }

        if ((color == "white") || (color == "cyan") || (color == "blue") || (color == "magenta")) {
            digitalWrite(Bled, LOW);
        } else {
            digitalWrite(Bled, HIGH);
        }
    }
}

void setLED(String color) {
    // Red LED
    if ((color == "white") || (color == "red") || (color == "yellow") || (color == "magenta")) {
        digitalWrite(Rled, LOW);
    } else {
        digitalWrite(Rled, HIGH);
    }

    // Green LED
    if ((color == "white") || (color == "yellow") || (color == "green") || (color == "cyan")) {
        digitalWrite(Gled, LOW);
    } else {
        digitalWrite(Gled, HIGH);
    }

    // Blue LED
    if ((color == "white") || (color == "cyan") || (color == "blue") || (color == "magenta")) {
        digitalWrite(Bled, LOW);
    } else {
        digitalWrite(Bled, HIGH);
    }
}