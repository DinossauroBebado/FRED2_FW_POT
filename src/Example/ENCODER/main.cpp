#include <Arduino.h>

#include "encoder.h"
EncoderR encoder ;

double curEncoderRPM ; 
double encoderRatio = 1;
double reducerRatio = 1;
double curAxisRPM; 
double curEncoderAngle; 
double curAxisAngle ;
double curMotorRPM ; 


void setup(){
    Serial.begin(115200);
    encoder.setup();

}

void loop(){

//Read and convert Current encoder Velocity
    curEncoderRPM = encoder.readRPM(); //Encoder Read
    curAxisRPM = curEncoderRPM / encoderRatio;
    curMotorRPM = curAxisRPM * reducerRatio;

    //Read and convert Current encoder Angle
    curEncoderAngle = encoder.readAngle(); //Encoder Read
    curAxisAngle = curEncoderAngle / encoderRatio;

    encoder.debugPrint();

}