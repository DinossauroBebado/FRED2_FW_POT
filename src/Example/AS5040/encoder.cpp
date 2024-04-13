#include "Arduino.h"
#include "encoder.h" // header in local directory

//--------------------------------------------------
//Pinout - LEGADO
//--------------------------------------------------
// volatile int DI_ENCODER_CH_A = 36;
// volatile int DI_ENCODER_CH_B = 39;

//--------------------------------------------------
//Configuration
//--------------------------------------------------
int encoderPPR = 1024;

Encoder* Encoder::obj_Encoder = 0;

//--------------------------------------------------
//Variable Declaration/Initialization
//--------------------------------------------------
volatile bool firstRead = true;

//only to debug interruption time
volatile unsigned long interr_curTime_us = 0;
volatile unsigned long interr_prevTime1_us = 0;
volatile unsigned long interr_prevTime2_us = 0;
volatile unsigned long interr_deltaTime_us = 0;
volatile unsigned long interr_highestTime_us = 0;

//Pulse Counter
volatile bool curA, curB, prevA, prevB; 

volatile unsigned long pulseTime = 0;
volatile double encoderCount = 0;
volatile double encoderPulseCount=0;
volatile double encoderPulseError=0;
volatile bool encoderErro;

//Calculate Current Angle
double curAngle = 0;


//Calculate RPM
volatile unsigned long pulseTimeLatch = 0;
volatile unsigned long pulsePrevTime = 0;
volatile unsigned long pulsePrevPrevTime = 0;
volatile unsigned long pulseDeltaTime = 0;

volatile double encoderCountLatch = 0;
volatile double encoderPrevCount = 0;
volatile double encoderPrevPrevCount = 0;
volatile double encoderDeltaCount = 0;

volatile bool lastPulseForward = 0;
volatile bool lastPulseBackward = 0;

volatile double curRPM = 0;
volatile double prevRPM = 0;
volatile double highestRPM = 0;

volatile int filterGain = 20;
volatile double curRPM_Filtered = 0;



//--------------------------------------------------
// Functions
//--------------------------------------------------

void IRAM_ATTR interruptionChA();
void IRAM_ATTR interruptionChB();

Encoder::Encoder(int pin_A,int pin_B){
    this->DI_ENCODER_CH_A = pin_A;
    this->DI_ENCODER_CH_B = pin_B;

}


void Encoder::setup()
{
    //Serial.println("EncSetup");
    
    //IO
    pinMode(DI_ENCODER_CH_A, INPUT_PULLUP);
    pinMode(DI_ENCODER_CH_B, INPUT_PULLUP);

    //Read Channels AB Initial State
    curA = digitalRead(DI_ENCODER_CH_A);
    curB = digitalRead(DI_ENCODER_CH_B);
    prevA = curA;
    prevB = curB;

    obj_Encoder = this;

    //Configure Interrupt
    attachInterrupt(DI_ENCODER_CH_A, Encoder::interruptionChA, CHANGE);
    attachInterrupt(DI_ENCODER_CH_B, Encoder::interruptionChB, CHANGE);
    
}


void IRAM_ATTR Encoder::interruptionChA()
{

    //Serial.println("A");
    //  interr_prevTime1_us = micros();

    pulseTime = micros();

    curA = digitalRead(obj_Encoder-> DI_ENCODER_CH_A);

    if(curA == prevA)
    {
        //depois da interrupção, já leu e voltou ao valor anterior -> debounce = faz nada
    }
    else
    {
    curB = digitalRead(obj_Encoder->DI_ENCODER_CH_B);

    if (curB != prevB){
        encoderErro = true;
        encoderPulseError++;
        //Serial.println("E_A");
    }

    // Serial.print("A:");
    // Serial.print(prevA);
    // Serial.print(prevB);
    // Serial.print("-");
    // Serial.print(curA);
    // Serial.print(curB);
    // Serial.print("-");
    // Serial.println(encoderPulseError);

    prevA = curA;
    prevB = curB;

    if (curA == false)
    {
        if (curB == true)
        {
        encoderCount++;
        }
        else {
        encoderCount--;
        }
        
    }
    if (curA == true)
    {
        if (curB == false)
        {
        encoderCount++;
        }
        else {
        encoderCount--;
        }
    }

    encoderPulseCount++;


    //      interr_prevTime2_us = interr_prevTime1_us;
    //      interr_curTime_us = micros();
    //      interr_deltaTime_us = interr_curTime_us - interr_prevTime2_us;
    //      
    //      if (interr_deltaTime_us > interr_highestTime_us) interr_highestTime_us = interr_deltaTime_us;
    
    }
}

void IRAM_ATTR Encoder::interruptionChB()
{
    //Serial.println("B");

    pulseTime = micros();

    curB = digitalRead(obj_Encoder->DI_ENCODER_CH_B);

    if(curB == prevB)
    {
    //depois da interrupção, já leu e voltou ao valor anterior -> debounce = faz nada
    }
    else
    {

    curA = digitalRead(obj_Encoder->DI_ENCODER_CH_A);

    if (curA != prevA){
        encoderErro = true;
        encoderPulseError++;
        //Serial.println("E_B");
    }

    //    Serial.print("B - ");
    //    Serial.print(prevA);
    //    Serial.print(prevB);
    //    Serial.print("-");
    //    Serial.print(curA);
    //    Serial.print(curB);
    //    Serial.print("-");
    //    Serial.println(encoderPulseError);

    prevB = curB;
    prevA = curA;

    if (curB == false)
    {
        if (curA == false)
        {
        encoderCount++;
        }
        else {
        encoderCount--;
        }
        
    }
    if (curB == true)
    {
        if (curA == true)
        {
        encoderCount++;
        }
        else {
        encoderCount--;
        }
    }

    encoderPulseCount++;

    }
  
}


double Encoder::readAngle()
{
    curAngle = ((double)360.0*(double)encoderCount)  / ((double)(3.0*encoderPPR));
    return curAngle;

}

double Encoder::readPulses(){
    return encoderCount; 

}

double Encoder::readRPM()
{

    noInterrupts(); //Disable interruptions - Critical code
    pulseTimeLatch = pulseTime;
    encoderCountLatch = encoderCount;
    interrupts(); //Re-Enable interruptions

    encoderDeltaCount = encoderCountLatch - encoderPrevCount;

    
    if (firstRead==true)
    {
        curRPM = 0;
        firstRead = false;
    }
    else
    {
        if (encoderDeltaCount != 0) //New pulses
        {
            
            
            pulseDeltaTime = pulseTimeLatch - pulsePrevTime;
            
            if (pulseDeltaTime < 1) //Prevent division by 0
            {
                curRPM = 0;
            }
            else
            {
                curRPM = (double)encoderDeltaCount*(1000000.0/(double)pulseDeltaTime)*60.0/encoderPPR;
            }

            //Set flag to remember if going forward or backward
            if (encoderDeltaCount>0)
            {
                lastPulseForward = true;
                lastPulseBackward = false;
            }else
            {
                lastPulseForward = false;
                lastPulseBackward = true;
            }
            
        }
        else //No new pulses, use current time
        {
            pulseDeltaTime = micros() - pulsePrevTime;
         
            if (pulseDeltaTime < 1)
            {
                curRPM = 0;
            }
            else
            {
                curRPM = (double)1.0*(1000000.0/(double)pulseDeltaTime)*60.0/encoderPPR;
            }

            if (lastPulseBackward)
            {
                curRPM = -curRPM;
                
            }

        }
    }
    
    
    encoderPrevPrevCount = encoderPrevCount;
    encoderPrevCount = encoderCountLatch;


    pulsePrevPrevTime = pulsePrevTime;
    if (encoderDeltaCount != 0) //New pulses to update time
    {
        pulsePrevTime = pulseTimeLatch;
    }
    

    if (curRPM < 0.5 && curRPM > -0.5)
    {
        curRPM = 0;
    }

    //filter glitches (position overflow)
    if ((curRPM > 10000) | (curRPM < -10000)){
        curRPM = prevRPM;
    }  else {
        prevRPM = curRPM;
    }
    
    //---
    curRPM_Filtered = (curRPM_Filtered*filterGain+curRPM)/(filterGain+1);
    if (curRPM_Filtered < 0.1 && curRPM_Filtered > -0.1)
    {
        curRPM_Filtered = 0;
    }
    

    //Store highest RPM ever
    if (curRPM > highestRPM){
        highestRPM = curRPM;
    }

    return curRPM_Filtered;

}

void Encoder::reset()
{
  encoderCount = 0;
  encoderErro = false;
  highestRPM = 0;
  encoderPulseCount = 0;
  encoderPulseError = 0;
}


//--------------------------------------------------
//Debug Print
//--------------------------------------------------
void Encoder:: debugPrint() {

    Serial.print("|Encoder->");

    //Serial.print(digitalRead(DI_ENCODER_CH_A));
    //Serial.println(digitalRead(DI_ENCODER_CH_B));

    // Serial.print(", ErroCount");
    // Serial.print(encoderPulseError);

    //---

    Serial.print(", CurAngle:");
    Serial.print(curAngle);

    //---

    // Serial.print("P_T:");
    // Serial.print(pulseTimeLatch);

    // Serial.print(", P_PT:");
    // Serial.print(pulsePrevPrevTime);

    Serial.print(", P_DT:");
    Serial.print(pulseDeltaTime);

    // Serial.print(", P_C:");
    // Serial.print(encoderCountLatch);

    // Serial.print(", P_PC:");
    // Serial.print(encoderPrevPrevCount);
    
    Serial.print(", P_DC:");
    Serial.print(encoderDeltaCount);

    Serial.print(", RPM:");
    Serial.print(curRPM);

    // Serial.print(", RPM_F:");
    // Serial.print(curRPM_Filtered);
    
    // Serial.print(", RPM_H:");
    // Serial.print(highestRPM);
    

    Serial.println();

}