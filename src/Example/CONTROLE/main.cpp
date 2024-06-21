
#include <Main_Lib/config.h>

#include <Main_Lib/power.h>
#include <Main_Lib/MedianFilter.h>

#include <Main_Lib/encoder.h>

Encoder encoder(34, 35, 39,36);

MedianFilter encoderRightFilter(33,0);
MedianFilter encoderLeftFilter(33,0);


#include "controler.h"
Controler  esquerda_controler(0.4,0.0001,0.008);  //(p,i,d)
// Controler  direita_controler(0.4,0.01,0.008);
Controler  direita_controler(0.4, 0.0001 , 0.008 );  //(p,i,d)


const int ACC = 50 ;
const int GAIN = 1 ;
const int GAIN_ANGULAR = 7;

bool _connect = false ;

float rpm_right = 0 ;
float rpm_left = 0;

int rpm = 0;
int rpm_controled = 0;

bool debug = true;




void setup() { 
  
  encoder.setup();
  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,HIGH);
  Serial.begin(115200);
//   delay(10000);
}

void loop() 
{   
    //add conection protection 
    


   

    //------------------------------RIGHT-------------------------------------------

    //status -- encoder 

    double angle_encoder_read_right = encoder.readAngle(RIGHT);

    double rpm_encoder_read_right = encoder.readRPM(RIGHT);
    encoderRightFilter.in(rpm_encoder_read_right);
    rpm_encoder_read_right = encoderRightFilter.out();

    double ticks_encoder_read_right = encoder.readPulses(RIGHT);

    //cmd -- 

   
    float controled_RPM_right = rpm_right;
    // float controled_RPM_right = direita_controler.output(rpm_right,rpm_encoder_read_right);

  //----------------debug------------------------------
    if(debug){
    rpm = -200;
    rpm_controled = direita_controler.output(rpm,rpm_encoder_read_right);
    direita_controler.debug();
    write2motor(rpm_controled,2);
    // write2motor(rpm,2);
    }
  //--------------------------execute-----------------

   

    
}