/*
--------------------------------- ATENÇÃO!!!!!! -----------------------------------
ANTES DE FAZER A PROGRAMAÇÃO DO ENCODER LEIA ATENTAMENTE AS INSTRUÇÕES!! QUALQUER ERRO PODE OCASIONAR DIRETAMENTE
A DANIFICAÇÃO DO COMPONENTE E COMPREMETER PERMANENTEMENTE O SEU USO.

Para reprogramação do AS5040 leia atentamente a página do Notion à seguir:
https://www.notion.so/oprojeto/AS5040-bf0dc3810a9845b3a698e62f5cb4c80a

Leve em conta todas as observações, principalmente a de modificação da biblioteca e nas modificações de níveis de tensão. POR FAVOR SE ATENTE A PINAGEM!
*/

#include <Arduino.h>
#include "AS5040.h"

// CHANGE THESE AS APPROPRIATE
#define CSpin   5
#define CLKpin  18
#define DOpin   4
#define PROGpin 19

// OUTPUT PINS
#define enc_a  35
#define enc_b  14
#define enc_z  8

// BUTTON PIN
#define pinBUTTON 26


AS5040 enc (CLKpin, CSpin, DOpin, PROGpin) ;

// Set mode to quadrature (A + B + index), monitor via serial IF
void setup (){    
    Serial.begin (115200) ;   // NOTE BAUD RATE

    pinMode(pinBUTTON, INPUT_PULLUP);

    // if (!enc.begin (AS5040_QUADRATURE, false, 0))  // example setting reverse sense and an offset
    //     Serial.println ("Error setting up AS5040") ;

    
    // // // Desconectar o pinPROG e conectar o Vprog
    Serial.println("Aguardando troca de tensão: pinPROG --> Vprg");
    while(true){
        if(digitalRead(pinBUTTON) == 0){
            break;
        }
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
    Serial.println("Vprog conectado, iniciando ciclo CLK");


    // Inicio da programação do sensor com o Vprog ()
    for (byte i = 0 ; i < 16 ; i++){
        digitalWrite (CLKpin, HIGH) ;
        delayMicroseconds (2) ;
        digitalWrite (CLKpin, LOW) ;
        delayMicroseconds (2) ;
    }

    // Desconectar o Vprog e conectar o 0V
    delay(2000);
    Serial.println("Ciclo CLK concluido, alterar Vprog para 0V");
    while(true){
        if(digitalRead(pinBUTTON) == 0){
            break;
        }
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
    Serial.println("Termino do procedimento");

    digitalWrite(CSpin, LOW);

}

void loop ()
{
//   Serial.print (enc.read (), HEX) ;
//   Serial.print ("   ") ;
//   Serial.print (enc.status (), BIN) ;
//   Serial.print ("   ") ;
//   Serial.print (enc.valid () ? "OK" : "Fault") ;
//   Serial.print ("   ") ;
//   Serial.println (enc.Zaxis ()) ;
//   delay (400) ;
}