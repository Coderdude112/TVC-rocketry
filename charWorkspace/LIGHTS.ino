void setLED(String color) {
    // Red LED
    if ((color == "white") || (color == "red") || (color == "yellow") || (color == "magenta")) {
        digitalWrite(rLED, LOW);
    } else {
        digitalWrite(rLED, HIGH);
    }

    // Green LED
    if ((color == "white") || (color == "yellow") || (color == "green") || (color == "cyan")) {
        digitalWrite(gLED, LOW);
    } else {
        digitalWrite(gLED, HIGH);
    }

    // Blue LED
    if ((color == "white") || (color == "cyan") || (color == "blue") || (color == "magenta")) {
        digitalWrite(bLED, LOW);
    } else {
        digitalWrite(bLED, HIGH);
    }
}

// ---

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
        digitalWrite(rLED, HIGH);
        digitalWrite(gLED, HIGH);
        digitalWrite(bLED, HIGH);
    } else if ((ledState == LOW) && (currentMills - previousMills >= offTime)) {
        ledState = HIGH;
        previousMills = currentMills;
        analogWrite(buzzer, buzzerVolume); // Comment out to disable beeping

        // Set the LEDs
        if ((color == "white") || (color == "red") || (color == "yellow") || (color == "magenta")) {
            digitalWrite(rLED, LOW);
        } else {
            digitalWrite(rLED, HIGH);
        }

        if ((color == "white") || (color == "yellow") || (color == "green") || (color == "cyan")) {
            digitalWrite(gLED, LOW);
        } else {
            digitalWrite(gLED, HIGH);
        }

        if ((color == "white") || (color == "cyan") || (color == "blue") || (color == "magenta")) {
            digitalWrite(bLED, LOW);
        } else {
            digitalWrite(bLED, HIGH);
        }
    }
}
