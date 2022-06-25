// ------------- //
/* Bring in libs */
// ------------- //
#include <Servo.h> // Servos
#include <Wire.h> // Communication over i2c (comms between processor and sensors)
#include <SPI.h> // Communication over spi (talk to SD card)
#include <SD.h> // Read / write to SD card
#include <Adafruit_LSM9DS1.h> // Communication with the IMU
#include <Adafruit_BMP280.h> // Communication with the BARO (barometer)
#include <Adafruit_Sensor.h> // General talk to sensors
#include <Quaternion.h> // Orientation tracking (does the math for us)

// ----------------- //
/* Initing variables */
// ----------------- //

// Required for use with the IMU and BARO
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
Adafruit_BMP280 bmp;

Servo servoX;
Servo servoY;

// Logging stuff
File myFile; // Unknown use
int loggingInterval = 100; // ms between logging // 100>10 hz 25>40 hz

// Pin definitions @TODO: Redo these comments to be consistent
const int vbat = 34; //analog pin to read battery voltage
const int gpio1 = 37; //general purpose input output pin
const int gpio2 = 38; //general purpose input output pin
const int pwm1 = 23; //pulse width modulation output
const int pwm2 = 22; //pulse width modulation output
const int tvc1 = 21; //controls x servo
const int tvc2 = 20; //controls y servo
const int pyro = 29; //controls pyro channel
const int cont = 33; //detects continuity of pyro channel
const int Rled = 28; //controls red LED
const int Gled = 27; //controls green LED
const int Bled = 26; //controls blue LED
const int buzz = 30; //controls buzzer
const int flashCS = 10; //controls chip select pin of the spi flash chip
const int chipSelect = BUILTIN_SDCARD; //controls chip select pin of the micro sd card

// Unknown
uint8_t logFileNumber;
float rollOut;
int runOnce = 0; // Ensures a method only runs once
int zeroed = false;

// Char information machine @TODO: Make docs for state machine ints
int state = 0; //defines the state machine
const int buzzVol = 10; //controls the **volume** of the buzzer
float batV; //voltage of the battery

// LEDs
int ledState = 0; // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
long OffTime = 900;          // milliseconds of LED off-time
long OnTime = 100;           // milliseconds of LED on-time

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
float trueAlt; //estimated altitude from the barometer minus altitude found upon startup (baroalt - baroOffset)
float baroOffset; //estimated altitude found upon startup
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

// Acceleration stuff
const int accelNumReadings = 15;
int accelReadings[accelNumReadings];      // the readings from the analog input
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

// --------- //
/* Functions */
// --------- //
void setup() { // Runs once then goes to loop()
    pinModes();
    digitalWrite(gpio1, LOW);
    setLED("red");
    Serial.begin(9600);
    setupSensor();
    startBaro();

    servoX.write(xMid);
    servoY.write(yMid);

    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("sd initialization failed!");
        return;
        state = 9;
    }
    myFile = SD.open("test.csv", FILE_WRITE);
    loggingLabel();
    myFile.flush();
    myFile.close();
    myFile = SD.open("test.csv", FILE_WRITE);
}

void loop() {
    getTelemetry();
    dTime = millis();

    stateDetect();

    // batLowDetect(); //////////////////////disable for flight
}
