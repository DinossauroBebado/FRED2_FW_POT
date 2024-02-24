#include "Arduino.h"
#include <Main/config.h>

void setup()
{
  pinMode(M1_IN1, OUTPUT); 
  pinMode(M1_IN2, OUTPUT); 
  
  pinMode(M2_IN1, OUTPUT); 
  pinMode(M2_IN2, OUTPUT); 

  pinMode(M3_IN1, OUTPUT); 
  pinMode(M3_IN2, OUTPUT); 

  pinMode(M4_IN1, OUTPUT); 
  pinMode(M4_IN2, OUTPUT); 

  pinMode(M1_PWM, OUTPUT); 
  pinMode(M2_PWM, OUTPUT); 
  pinMode(M3_PWM, OUTPUT); 
  pinMode(M4_PWM, OUTPUT); 

  ledcSetup(CANAL_M1, FREQUENCIA, 10);
  ledcSetup(CANAL_M2, FREQUENCIA, 10);
  ledcSetup(CANAL_M3, FREQUENCIA, 10);
  ledcSetup(CANAL_M4, FREQUENCIA, 10);

  ledcAttachPin(M1_PWM, CANAL_M1);
  ledcAttachPin(M2_PWM, CANAL_M2);
  ledcAttachPin(M3_PWM, CANAL_M3);
  ledcAttachPin(M4_PWM, CANAL_M4);

  ledcWrite(CANAL_M1, 170);
  ledcWrite(CANAL_M2, 300);
  ledcWrite(CANAL_M3, 300);
  ledcWrite(CANAL_M4, 170);

  //! Faz com que os motores andem pra frente

  digitalWrite(M1_IN1, 1); 
  digitalWrite(M2_IN1, 1); 
  digitalWrite(M3_IN1, 1); 
  digitalWrite(M4_IN1, 1); 

  digitalWrite(M1_IN2, 0); 
  digitalWrite(M2_IN2, 0);
  digitalWrite(M3_IN2, 0);
  digitalWrite(M4_IN2, 0);

  
}

void loop()
{

}