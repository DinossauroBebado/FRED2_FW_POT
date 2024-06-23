#include <Arduino.h>
#include <Main_Lib/config.h>

void setup()
{
  Serial.begin(115200);
  pinMode(M1_IN1, OUTPUT); 
  pinMode(M1_IN2, OUTPUT); 
  
  pinMode(M2_IN1, OUTPUT); 
  pinMode(M2_IN2, OUTPUT);
  pinMode(M1_PWM, OUTPUT); 
  pinMode(M2_PWM, OUTPUT); 

// /

  //! Faz com que os motores andem pra frente
  Serial.println("Motor Esquerdo Acionado");
  digitalWrite(M1_IN1, 1); 
  digitalWrite(M2_IN1, 1); 

  digitalWrite(M1_IN2, 0); 
  digitalWrite(M2_IN2, 0);

  digitalWrite(M1_PWM, 1); 
  digitalWrite(M2_PWM, 1);

  Serial.println("Motor Direito Acionado");
  digitalWrite(M1_IN1, 0); 
  digitalWrite(M2_IN1, 0); 

  digitalWrite(M1_IN2, 1); 
  digitalWrite(M2_IN2, 1);

  digitalWrite(M1_PWM, 1); 
  digitalWrite(M2_PWM, 1);
}

void loop(){

}