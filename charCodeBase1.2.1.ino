#include <Servo.h> //supports controlling servos
#include <Wire.h> //supports communication over i2c
#include <SPI.h> //supports communication over spi
#include <SD.h> //supports writing and reading to the micro sd card
#include <Adafruit_LSM9DS1.h> //supports communication with the IMU
#include <Adafruit_BMP280.h> //supports communication with the barometer
#include <Adafruit_Sensor.h> //supports other libraries
#include <Quaternion.h> //supports orientation tracking

Adafruit_BMP280 bmp;
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

Servo servoX;
Servo servoY;

File myFile;





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
const int abortRange = 30; //defines how many degrees off target an abort is initiated
const int chipSelect = BUILTIN_SDCARD; //controls chip select pin of the micro sd card

int state = 0; //defines the state machine

float batV; //voltage of the battery

float baroAlt; //estimated altitude from the barometer
float trueAlt; //estimated altitude from the barometer minus altitude found upon startup
float baroOffset; //estimated altitude found upon startup
float maxAlt; //the highest estimated altitude acheived during flight

const int buzzVol = 10; //controls the volume of the buzzer

int ledState = 0;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
long OffTime = 900;          // milliseconds of LED off-time
long OnTime = 100;           // milliseconds of LED on-time

unsigned long dTime;
unsigned long flightTime; //milliseconds since launch detect
unsigned long launchTime; //time at launch in milliseconds
unsigned long burnoutTime; //time at burnout in milliseconds

unsigned long burnoutDelay = (6500 - 3450); //backup timer to apogee detection, measured starting at burnout
// F15 burn time: 3450 millis






const int accelNumReadings = 15;
int accelReadings[accelNumReadings];      // the readings from the analog input
int accelReadIndex = 0;              // the index of the current reading
int accelTotal = 0;                  // the running total
int accelAverage = 0;                // the average

uint8_t logFileNumber;



int pyroState = false; //whether the pyro channel is activated or not


float accelX;
float accelY;
float accelZ;

float gyroX;
float gyroY;
float gyroZ;

uint64_t lastGyroUpdate;
uint64_t newGyroUpdate;
uint64_t gyro_dt;

Quaternion orientation;

// Current rotation, stored in radians per second according to RHR
float bX = 0;
float bY = 0;
float bZ = 0;

float yaw;
float pitch;
float roll;

struct EulerAngles {
  float yaw;
  float pitch;
  float roll;
};

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



int launchAcceleration = 11;


float xMid = 86;
float yMid = 86;

float gearingX = 3;
float gearingY = 3.18;
//x axis 3, y axis -3.16 3.2

float rollOut;



unsigned long lastTime;


float xOut;
float yOut;
float zOut;

float zOutI;



float kpx = 1.55;
float kix = 0.0; //NONFUNCTIONAL DO NOT USE "we're working on it"
float kdx = 0.35;


float kpy = kpx;
float kiy = kix;
float kdy = kdx;


float kpz = 3;
float kiz = 0;
float kdz = 20;



float lastErrX;
float errSumX;

float lastErrY;
float errSumY;

float lastErrZ;
float errSumZ;




float xOutPR;//PR= post roll
float yOutPR;

int runOnce = 0;
unsigned long pyroTime;

int zeroed = false;



unsigned long lastLog;

int loggingInterval = 100;
// 100>10 hz 25>40 hz




void setup() {
  pinModes();
  digitalWrite(gpio1, LOW);
  red();
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








void loop()
{

  realTime();


  //  Serial.print(pyrostate:   );
 // Serial.print(kpz);
 // Serial.print(roll);
 // Serial.print(zOut);
  //Serial.println();

  Serial.println();
}







void realTime()
{

  getTelemetry();
  dTime = millis();




  stateDetect();

  /*
    Serial.print("   pitch:   ");
    Serial.print(pitch);

    Serial.print("   yaw:   ");
    Serial.print(yaw);

    Serial.print("   roll:   ");
    Serial.print(roll);
  */

  // batLowDetect(); //////////////////////disable for flight

}
