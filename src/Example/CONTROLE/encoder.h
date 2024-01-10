

#ifndef ENCODER_H // include guard
#define ENCODER_H
  
  #include <Arduino.h>

  class Encoder
  {
    public:
      Encoder( int pin_A, int pin_B);
      void setup();
      double readPulses();
      double readAngle();
      double readRPM();
      void debugPrint();
      void reset();
      int DI_ENCODER_CH_A;
      int DI_ENCODER_CH_B;
        
    private:
      static void IRAM_ATTR interruptionChA();
      static void IRAM_ATTR interruptionChB();

      // objeto que representa a classe Encoder
      static Encoder* obj_Encoder; 
      // Encoder* obj_Encoder; 

  };

  
#endif