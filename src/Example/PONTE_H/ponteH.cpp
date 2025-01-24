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
  digitalWrite(M1_PWM, 1); 
  digitalWrite(M2_PWM, 1);

  digitalWrite(M1_PWM, 1); 
  digitalWrite(M2_PWM, 1);
  //! Faz com que os motores andem pra frente
  Serial.println("Motor Esquerdo Acionado");

}

void loop(){
  digitalWrite(M1_IN1, 0); 
  digitalWrite(M2_IN1, 0); 

  digitalWrite(M1_IN2, 1); 
  digitalWrite(M2_IN2, 1);


  Serial.println("Motor Direito Acionado");
  digitalWrite(M1_IN1, 1); 
  digitalWrite(M2_IN1, 1); 

  digitalWrite(M1_IN2, 0); 
  digitalWrite(M2_IN2, 0);

  delay(1000); 

  digitalWrite(M1_IN1, 1); 
  digitalWrite(M2_IN1, 1); 

  digitalWrite(M1_IN2, 0); 
  digitalWrite(M2_IN2, 0);


  Serial.println("Motor Direito Acionado");
  digitalWrite(M1_IN1, 0); 
  digitalWrite(M2_IN1, 0); 

  digitalWrite(M1_IN2, 1); 
  digitalWrite(M2_IN2, 1);

  delay(1000); 

}