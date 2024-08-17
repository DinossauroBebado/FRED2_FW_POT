#include "micro_ros.h"
#include <Arduino.h>
#include <Main_Lib/config.h>
#include <Main_Lib/controler.h>
#include <Main_Lib/rampa.h>
#include <Main_Lib/cinematic.h>
#include <Main_Lib/power.h>

// TODO
// stil need to add some topics
// divide task in two cores
//  write docs

#include <Main_Lib/encoder.h>
Encoder encoder(34, 35, 36,39);

#include <Main_Lib/MedianFilter.h>
MedianFilter encoderRightFilter(33, 0);
MedianFilter encoderLeftFilter(33, 0);

#include <Main_Lib/controler.h>
Controler esquerda_controler(1, 0, 0); //(p,i,d)
Controler direita_controler(1, 0, 0);  //(p,i,d) ->0.4

const int ACC = 50;
// const int GAIN = 1 ;
const int GAIN = 10;
const int GAIN_ANGULAR = 7;

bool _connect = false;

float rpm_right = 0;
float rpm_left = 0;

float rpm = 0;
float rpm_controled = 0;

float controled_RPM_right;
float controled_RPM_left;

void setup()
{
  init_ros();
  encoder.setup();
}

void loop()
{

  float linear = getLinear();   // robot
  float angular = getAngular(); // robot

  // debug = debugControl();

  //---------------------LEFT-------------------------------------------

  // status -------encoder
  double angle_encoder_read_left = encoder.readAngle(LEFT);

  double rpm_encoder_read_left = encoder.readRPM(LEFT);
  encoderLeftFilter.in(rpm_encoder_read_left);

  rpm_encoder_read_left = encoderLeftFilter.out();

  double ticks_encoder_read_left = encoder.readPulses(LEFT);

  // cmd------

  float angular_speed_left = cinematic_left(linear, angular, GAIN); // wheel [rad/s]

  rpm_left = angular2rpm(angular_speed_left); // [RPM]
  float rpm_left_com_rampa = rampa(rpm_left, 10, LEFT);
  // rpm_left =  saturation(rpm_left,800);

  // float controled_RPM_left = rpm_left;
  float controled_RPM_left = esquerda_controler.output(rpm_left, rpm_encoder_read_left);
  // float controled_RPM_left = esquerda_controler.output(rpm_left,rpm_encoder_read_left);

  //------------------------------RIGHT-------------------------------------------

  // status -- encoder

  double angle_encoder_read_right = encoder.readAngle(RIGHT);

  double rpm_encoder_read_right = encoder.readRPM(RIGHT);
  encoderRightFilter.in(rpm_encoder_read_right);
  rpm_encoder_read_right = encoderRightFilter.out();

  double ticks_encoder_read_right = encoder.readPulses(RIGHT);

  // cmd --

  float angular_speed_right = cinematic_right(linear, angular, GAIN); // wheel [RAD/S]

  rpm_right = angular2rpm(angular_speed_right);            // [RPM]
  float rpm_right_com_rampa = rampa(rpm_right, 10, RIGHT); // not used

  float controled_RPM_right = direita_controler.output(rpm_right, rpm_encoder_read_right); // not used

  //----------------debug------------------------------
  if (debug)
  {
    rpm = getRPMsetpoint();
    rpm_controled = direita_controler.output(rpm, rpm_encoder_read_right);
    write2motor(rpm_controled, 2);
    // write2motor(rpm,2);
  }
  //--------------------------execute-----------------

  if (!debug)
  {
    write2motors(controled_RPM_left, controled_RPM_right);
  }

  ros_loop(angular_speed_right, angular_speed_left,
           angle_encoder_read_left, angle_encoder_read_right,
           rpm_encoder_read_left, rpm_encoder_read_right,
           ticks_encoder_read_left, ticks_encoder_read_right,
           rpm_controled, controled_RPM_left,
           controled_RPM_left);

  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}