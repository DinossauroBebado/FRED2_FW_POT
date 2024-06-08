#include <Arduino.h>
#include <AS5040.h>
#include "encoder.h"

// CHANGE THESE AS APPROPRIATE
#define CSpin   5
#define CLKpin  18
#define DOpin   4
#define PROGpin 19

// OUTPUT PINS
#define enc_a  17
#define enc_b  16
#define enc_z  2


AS5040 enc(CLKpin, CSpin, DOpin, PROGpin);
Encoder encoder(enc_a, enc_b);

double contVolta = 0;

void contadorVolta(){
  contVolta++;
}




// Set mode to quadrature (A + B + index), monitor via serial IF
void setup ()
{
  Serial.begin (115200) ;   // NOTE BAUD RATE
  /*
  if (!enc.begin (AS5040_QUADRATURE, false, 0))  // example setting reverse sense and an offset
    Serial.println ("Error setting up AS5040");
  */
  encoder.setup(); 

  attachInterrupt(enc_z, contadorVolta, CHANGE);
}

void loop (){
    int val; 
    /*
    Serial.print(val);
    Serial.print(" | ");
    Serial.print (enc.read (), HEX) ;
    Serial.print ("   ") ;
    Serial.print (enc.status (), BIN) ;
    Serial.print ("   ") ;
    Serial.print (enc.valid () ? "OK" : "Fault") ;
    Serial.print ("   ") ;
    Serial.println (enc.Zaxis ()) ;
    */
    double pulses = encoder.readPulses();
    double rpm = encoder.readRPM();

    Serial.print("P: ");
    Serial.print(pulses);
    Serial.print(" RPM: ");
    Serial.println(rpm);
    delay (100) ;
}