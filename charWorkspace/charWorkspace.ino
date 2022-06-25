/*
 * I'v been working top to bottom starting in the setup function and renaming and reviewing as I go.
 * All variables that can't be in their functions need to be defined here
 * Variable sorting goes as they appear (top to bottom)
 * Need to find a style guide and figure out intellisense
 */

// ------------ //
/* Include libs */
// ------------ //

#include <Servo.h> // Servos
#include <Wire.h> // Communication over i2c (comms between processor and sensors)
#include <SPI.h> // Communication over spi (talk to SD card)
#include <SD.h> // Read & write to SD card
#include <Adafruit_LSM9DS1.h> // Communication with the IMU (Gryoscope, magnatometer, and Acceleration)
#include <Adafruit_BMP280.h> // Communication with the BARO (Temperature and barometric pressure)
#include <Adafruit_Sensor.h> // General talk to sensors
#include <Quaternion.h> // Orientation tracking (does the math for us)

// -------------- //
/* Init variables */
// -------------- //

// Basic pin definitions
unsigned const int rLED = 28; // Red control on the LED
unsigned const int gLED = 27; // Green control on the LED
unsigned const int bLED = 26; // Blue control on the LED
unsigned const int buzzer = 30;
unsigned const int GPIO1 = 37;
unsigned const int GPIO2 = 38;
unsigned const int servoXPin = 21; // TVC control servo
unsigned const int servoYPin = 20; // TVC control servo

// Micro SD card logging
// Docs: https://github.com/arduino-libraries/SD/blob/master/docs/api.md
unsigned const int sdCardPin = BUILTIN_SDCARD;
File logFile;
unsigned const int loggingInterval = 100; // ms between logging // 100>10 hz 25>40 hz
File myFile; // deprecated. @TODO: Remove completely

// IMU (Gryoscope, magnatometer, and Acceleration)
// Docs: https://github.com/arduino-libraries/Arduino_LSM9DS1/blob/master/docs/api.md
//       https://github.com/adafruit/Adafruit_LSM9DS1/blob/master/Adafruit_LSM9DS1.cpp
Adafruit_LSM9DS1 IMU; // Might need to set to "Adafruit_LSM9DS1();"

// BARO (Temperature and barometric pressure)
// Docs: https://github.com/adafruit/Adafruit_BMP280_Library
Adafruit_BMP280 BARO;
float baroAltiOffset; // Estimated alti found upon startup

// ---

// Pin definitions @TODO: Redo these comments to be consistent
const int vbat = 34; //analog pin to read battery voltage
const int pwm1 = 23; //pulse width modulation output
const int pwm2 = 22; //pulse width modulation output
const int tvc1 = 21; //controls x servo
const int tvc2 = 20; //controls y servo
const int pyro = 29; //controls pyro channel
const int cont = 33; //detects continuity of pyro channel

const int flashCS = 10; //controls chip select pin of the spi flash chip

// Acceleration stuff
unsigned const int numOfAccelReadings = 15; // The number of accel readings that will be used in finding the average accel
unsigned int accelReadings[numOfAccelReadings]; // All of the raw accel readings taken into account when calculating the average accel
int accelReadIndex = 0;              // the index of the current reading
int accelTotal = 0;                  // the running total
int accelAverage = 0;                // the average
// Raw acceleration data in m/s^2
float accelX;
float accelY;
float accelZ;
// Raw acceleration data in deg/s or rad/s
float gyroX;
float gyroY;
float gyroZ;

// Unknown
uint8_t logFileNumber;
float rollOut;
int runOnce = 0; // Ensures a method only runs once
int zeroed = false;

// Char information machine @TODO: Make docs for state machine ints
int state = 0; //defines the state machine
float batV; //voltage of the battery

Servo servoX;
Servo servoY;

// Flight stuffs
const int abortRange = 30; //defines how many degrees off target an abort is initiated
int pyroState = false; //whether the pyro channel is activated or not // @TODO: Make boolean
int launchAcceleration = 11; // Threshold for launch detection in m/s^2
// Given in deg
float yaw;
float pitch;
float roll;

// Stuff the BARO gives
float baroAlt; //estimated altitude from the barometer
float trueAlt; //estimated altitude from the barometer minus altitude found upon startup (baroalt - baroAltiOffset)
float maxAlt; //the highest estimated altitude acheived during flight

// Time stuff
unsigned long dTime; // Time since power on
unsigned long flightTime; //milliseconds since launch detect
unsigned long launchTime; //time at launch in milliseconds
unsigned long burnoutTime; //time at burnout in milliseconds
unsigned long burnoutDelay = (6500 - 3450); // Time between burnout and parachute deploy
unsigned long lastTime;
unsigned long pyroTime; // Time the parachute launches maybe...
unsigned long lastLog; // Time of the last log

// Orientation stuff
uint64_t lastGyroUpdate;
uint64_t newGyroUpdate;
uint64_t gyro_dt;
Quaternion orientation;
struct EulerAngles { // Unknown purpose
    float yaw;
    float pitch;
    float roll;
};
// Current rotation, stored in rad/s according to RHR
float bX = 0;
float bY = 0;
float bZ = 0;

// Gyro stuff
float gxcal;
float gycal;
float gzcal;
int calCount = 0;
float gxout;
float gyout;
float gzout;
float gxcalout;
float gycalout;
float gzcalout;

// Servos
float xMid = 86;
float yMid = 86;
// Ratio of servo movement to TVC mount (Hypothetically both to 1)
float gearingX = 3;
float gearingY = 3.18;
// Target sero positions
float xOut;
float yOut;
// Reaction wheel output
float zOut;
float zOutI;

// PID stuff for TVC mount
// x-axis
float kpx = 1.55;
float kix = 0.0; //NONFUNCTIONAL DO NOT USE "we're working on it"
float kdx = 0.35;
// y-axis
float kpy = kpx;
float kiy = kix;
float kdy = kdx;
// z-axis
float kpz = 3;
float kiz = 0;
float kdz = 20;
// PID helpers
float lastErrX;
float errSumX;
float lastErrY;
float errSumY;
float lastErrZ;
float errSumZ;
// Corrections for TVC mount control based on roll axis rotation occurring in flight
float xOutPR; //PR= post roll
float yOutPR;

// -------------- //
/* Core Functions */
// -------------- //

/** Runs once to setup the program then executes loop() forever */
void setup() {
    setupPinMappings();
    setLED("white");

    // Setup serial logging
    Serial.begin(9600);
    Serial.println("Serial log started");

    // Setup Micro SD card logging
    if (SD.begin(sdCardPin) == 0) {
        Serial.println("SD init failure");
        state = 100;
        return;
    } else {
        bool foundNextFlightNum = false;
        unsigned int nextFlightNum = 1;

        while(foundNextFlightNum == false) {
            if (SD.exists(("flight-" + String(nextFlightNum) + ".csv").c_str())) {
                nextFlightNum++;
            } else {
                foundNextFlightNum = true;
            }
        }

        logFile = SD.open(("flight-" + String(nextFlightNum) + ".csv").c_str(), FILE_WRITE);
        logFile.println("dTime,flightTime,trueAlt,accelX,accelY,accelZ,gyroX,gyroY,gyroZ,yaw,pitch,roll,xOut,yOut,state,pyroState");
        logFile.flush();
    }

    // Setup the IMU
    if (IMU.begin() == 0) {
        Serial.println("IMU init failure");
        state = 100;
        return;
    } else {
        IMU.setupAccel(IMU.LSM9DS1_ACCELRANGE_16G); // Can be: LSM9DS1_ACCELRANGE_2G, LSM9DS1_ACCELRANGE_4G, LSM9DS1_ACCELRANGE_8G, or LSM9DS1_ACCELRANGE_16G
        IMU.setupMag(IMU.LSM9DS1_MAGGAIN_4GAUSS); // Can be: LSM9DS1_MAGGAIN_4GAUSS, LSM9DS1_MAGGAIN_8GAUSS, LSM9DS1_MAGGAIN_12GAUSS, or LSM9DS1_MAGGAIN_16GAUSS
        IMU.setupGyro(IMU.LSM9DS1_GYROSCALE_245DPS); // Can be: LSM9DS1_GYROSCALE_245DPS, LSM9DS1_GYROSCALE_500DPS, or LSM9DS1_GYROSCALE_2000DPS
    }

    // Setup the BARO
    if (BARO.begin() == 0) {
        Serial.println("BARO init failure");
        state = 100;
        return;
    } else {
        BARO.setSampling(   Adafruit_BMP280::MODE_NORMAL, // Operating Mode
                            Adafruit_BMP280::SAMPLING_X2, // Tempeture oversampling
                            Adafruit_BMP280::SAMPLING_X16, // Pressure oversampling
                            Adafruit_BMP280::FILTER_X16, // Filtering
                            Adafruit_BMP280::STANDBY_MS_125); // Standby time

        baroAltiOffset = BARO.readAltitude(1013.25);
    }

    // ---

    digitalWrite(GPIO1, LOW); // What does this do?

    servoX.write(xMid);
    servoY.write(yMid);
}

void loop() {
    getTelemetry();
    dTime = millis();

    stateDetect();

    // batLowDetect(); //////////////////////disable for flight
}
