#include <Arduino.h>




#define LOOP_TIME 200000  
#define  SERIAL_RATE  115200


#define SATURATION 1020

// #define SATURATION 4095
#define  FREQUENCIA 1000  


// #define L  0.300
#define L 0.3

#define RADIUS 0.075

#define MAX_PWM 1020
#define MIN_PWM -1020

#define MAX_RPM 1000
#define MIN_RPM -1000

#define MAX_SPEED_LINEAR 10 //m/s 
#define MAX_SPEED_ANGULAR 3.141592653589*2 //rad/s 


//define the pins from the HARDWARE project 

//H Bridge 
#define CANAL_M1 0
#define CANAL_M2 1
#define CANAL_M3 2
#define CANAL_M4 3

#define  M1_IN1  18
#define  M1_IN2  15
#define  M1_PWM  12

#define  M2_IN1  19
#define  M2_IN2  23
#define  M2_PWM  25

#define  M3_IN1  22
#define  M3_IN2  21
#define  M3_PWM  32

#define  M4_IN1  5
#define  M4_IN2  4
#define  M4_PWM  27

#define ENCODER_RIGHT 36
#define ENCODER_LEFT 39


//debug led
#define  LED_BUILD_IN  2

#define  LED_STRIP  14
#define NUMPIXELS   17

#define  PWM_SATURATION_UNDER 0