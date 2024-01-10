#include "Arduino.h"

#define S1_M1 15
#define S2_M1 18
#define PWM_M1 12

#define S1_M2 23
#define S2_M2 19
#define PWM_M2 25 

#define S1_M3 21
#define S2_M3 22
#define PWM_M3 32

#define S1_M4 4
#define S2_M4 5
#define PWM_M4 27


const int freq = 1000;

int vel = 100;

const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int ledChannel4 = 3;

const int resolution = 8;

void setup()
{
  pinMode(S1_M1, OUTPUT); 
  pinMode(S2_M1, OUTPUT); 
  
  pinMode(S1_M2, OUTPUT); 
  pinMode(S2_M2, OUTPUT); 

  pinMode(S1_M3, OUTPUT); 
  pinMode(S2_M3, OUTPUT); 

  pinMode(S1_M4, OUTPUT); 
  pinMode(S2_M4, OUTPUT); 

  pinMode(PWM_M1, OUTPUT); 
  pinMode(PWM_M2, OUTPUT); 
  pinMode(PWM_M3, OUTPUT); 
  pinMode(PWM_M4, OUTPUT); 

  ledcSetup(ledChannel1, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcSetup(ledChannel3, freq, resolution);
  ledcSetup(ledChannel4, freq, resolution);

  ledcAttachPin(PWM_M1, ledChannel1);
  ledcAttachPin(PWM_M2, ledChannel2);
  ledcAttachPin(PWM_M3, ledChannel3);
  ledcAttachPin(PWM_M4, ledChannel4);

  ledcWrite(ledChannel1, vel);
  ledcWrite(ledChannel2, vel);
  ledcWrite(ledChannel3, vel);
  ledcWrite(ledChannel4, vel);

  //! Faz com que os motores andem pra frente

  digitalWrite(S1_M1, 1); 
  digitalWrite(S1_M2, 1); 
  digitalWrite(S1_M3, 1); 
  digitalWrite(S1_M4, 1); 

  digitalWrite(S2_M1, 0); 
  digitalWrite(S2_M2, 0);
  digitalWrite(S2_M3, 0);
  digitalWrite(S2_M4, 0);

  
}

void loop()
{

}