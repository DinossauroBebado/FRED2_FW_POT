/*........................
  DC Motor Controller
  Author: Andre Henning
  Company: Control Robotics
 ...........................
 Change CommMode: 
  in Main.cpp

    commMode = COMM_SERIAL or COMM_ROS;
 
Change Left/Right: 
 
   in PlatformIO
    
    Select environment!

*/


//--------------------------------------------------
//Pinout
//--------------------------------------------------

//Drive
int DO_PWM_CLOCKWISE = 33;
int DO_PWM_COUNTER_CLOCKWISE = 32;


int PIN_ENABLE_A = 18;
int PIN_ENABLE_B = 5;

int PIN_OVERVOLTAGE = 27;

//Current sensor
int PIN_CURRENT_SENSOR = 35;

//Current sensor H bridge 
int PIN_CURRENT_SENSOR_H_A = 14;
int PIN_CURRENT_SENSOR_H_B = 27;

// // Encoder
// int DI_ENCODER_CH_A = 25;
// int DI_ENCODER_CH_B = 26;

//Batery 
int PIN_BATTERY = 13;



//--------------------------------------------------
//Comm Mode
//--------------------------------------------------


#define COMM_SERIAL 0
#define COMM_ROS 1
// static int commMode = COMM_SERIAL;
static int commMode = COMM_ROS;


//--------------------------------------------------
//Import Modules
//--------------------------------------------------


#include <Arduino.h>

#include "encoder.h"

Encoder encoder(39,36);










//--------------------------------------------------
//Variable Declaration/Initialization
//--------------------------------------------------

//General
long startupDelay = 1000;

static int driveReset = false;

static int disableCount = 0;
static int disableCountLimit = 500; //*2ms = 1s

static double reducerRatio = 50.0;
static double encoderRatio = 3.0;

static int ACC = 250;


static double setAxisRPM = 0;
static double setMotorRPM = 0;
static double targetRPM = 0;

static double curEncoderRPM = 0;
static double curAxisRPM = 0;
static double curMotorRPM = 0;

static double curEncoderAngle = 0;
static double curAxisAngle = 0;
static double pulses = 0 ;

//--------------------------------------------------
//Setup = Initialization
//--------------------------------------------------
void setup()
{

  Serial.begin(115200);
  encoder.setup(); //Setup Encoder Reading

}

void loop(){

    pulses = encoder.readPulses();

    //Read and convert Current encoder Velocity
    curEncoderRPM = encoder.readRPM(); //Encoder Read
    curAxisRPM = curEncoderRPM / encoderRatio;
    curMotorRPM = curAxisRPM * reducerRatio;

    //Read and convert Current encoder Angle
    curEncoderAngle = encoder.readAngle(); //Encoder Read
    curAxisAngle = curEncoderAngle / encoderRatio;

    //-----------------------
    //Set Drive Control
    //-----------------------
    //Convert and Set Drive Velocity
    setMotorRPM = setAxisRPM * reducerRatio;

    Serial.print("| pulses: ");
    Serial.print(pulses);

    Serial.print("| curEncoderRPM: ");
    Serial.print(curEncoderRPM);

    Serial.print("| curAxisRPM: ");
    Serial.print(curAxisRPM);

    Serial.print("| curMotorRPM: ");
    Serial.print(curMotorRPM);

    Serial.print("| curEncoderAngle: ");
    Serial.print(curEncoderAngle);

    Serial.print("| curAxisAngle: ");
    Serial.print(curAxisAngle);

    Serial.print("| setMotorRPM: ");
    Serial.print(setMotorRPM);

    Serial.println("");

  }


