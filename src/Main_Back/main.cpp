#include "micro_ros.h"
#include "power.h"
#include <Arduino.h>
#include <Main_Lib/config.h>
#include <Main_Lib/controler.h>
#include <Main_Lib/rampa.h>
#include <Main_Lib/kinematics.h>

// TODO
// stil need to add some topics
// divide task in two cores
//  write docs

// -------------------------------------------------------
// Encoder config
// -------------------------------------------------------

#include <Main_Lib/encoder.h>
Encoder encoder(34, 35, 36,39);



// -------------------------------------------------------
// Median filter config
// -------------------------------------------------------

#include <Main_Lib/MedianFilter.h>
MedianFilter encoderRightFilter(33, 0);
MedianFilter encoderLeftFilter(33, 0);



// -------------------------------------------------------
// PID controller config
// -------------------------------------------------------

#include <Main_Lib/controler.h>
Controler esquerda_controler(1, 0, 0); //(p,i,d)
Controler direita_controler(1, 0, 0);  //(p,i,d) ->0.4




// -------------------------------------------------------
// RAMP
// -------------------------------------------------------

const int ACC = 50;



// -------------------------------------------------------
// Kinematics velocity gain
// -------------------------------------------------------

const int GAIN = 10;
const int GAIN_ANGULAR = 7;


// -------------------------------------------------------
// Micro-ros connection status 
// -------------------------------------------------------

bool _connect = false;



// -------------------------------------------------------
// Velocity variables
// -------------------------------------------------------

float rpm_right = 0;
float rpm_left = 0;

float rpm = 0;
float rpm_controled = 0;

float controled_RPM_right;
float controled_RPM_left;




// -------------------------------------------------------
// Inicialization
// -------------------------------------------------------

void setup()
{
  init_ros();
  encoder.setup();
}




// -------------------------------------------------------
// Loop
// -------------------------------------------------------

void loop()
{

  // -------------------------------------------------------
  // Get cmd_vel from ROS
  // -------------------------------------------------------

  float linear = getLinear();   // robot
  float angular = getAngular(); // robot

  // debug = debugControl();



  // -------------------------------------------------------
  // Left wheel controller 
  // -------------------------------------------------------

  // Get wheel current angle and RPM and ticks
  double angle_encoder_read_left = encoder.readAngle(LEFT);
  double rpm_encoder_read_left = encoder.readRPM(LEFT);
  double ticks_encoder_read_left = encoder.readPulses(LEFT);


  // Use the median filter to trow outliers
  encoderLeftFilter.in(rpm_encoder_read_left);
  rpm_encoder_read_left = encoderLeftFilter.out();


  // Use kinematics equation to convert robot vel for wheel speed 
  float angular_speed_left = kinematics_left(linear, angular, GAIN); // wheel [rad/s]


  // Converts rad/s to rpm
  rpm_left = angular2rpm(angular_speed_left); // [RPM]


  // Pass the command through a ramp -> not used 
  // float rpm_left_com_rampa = rampa(rpm_left, 10, LEFT);
  // rpm_left =  saturation(rpm_left,800);


  // Pass the setpoint for the PID controller
  // float controled_RPM_left = rpm_left;
  float controled_RPM_left = esquerda_controler.output(rpm_left, rpm_encoder_read_left);





  // -------------------------------------------------------
  // Right wheel controller 
  // -------------------------------------------------------

  // Get wheel current angle and RPM and ticks
  double angle_encoder_read_right = encoder.readAngle(RIGHT);
  double rpm_encoder_read_right = encoder.readRPM(RIGHT);
  double ticks_encoder_read_right = encoder.readPulses(RIGHT);


  // Use the median filter to trow outliers
  encoderRightFilter.in(rpm_encoder_read_right);
  rpm_encoder_read_right = encoderRightFilter.out();


  // Use kinematics equation to convert robot vel for wheel speed 
  float angular_speed_right = kinematics_right(linear, angular, GAIN); // wheel [RAD/S]


  // Converts rad/s to rpm
  rpm_right = angular2rpm(angular_speed_right);            // [RPM]


  // Pass the command through a ramp
  // float rpm_right_com_rampa = rampa(rpm_right, 10, RIGHT); // not used


  // Pass the setpoint for the PID controller
  float controled_RPM_right = direita_controler.output(rpm_right, rpm_encoder_read_right); 

  

  // -------------------------------------------------------
  // Debug mode -> teste a single motor 
  // -------------------------------------------------------

  if (debug)
  {
    rpm = getRPMsetpoint();
    rpm_controled = direita_controler.output(rpm, rpm_encoder_read_right);
    write2motor(rpm_controled, 2);
    // write2motor(rpm,2);
  }
  


  // -------------------------------------------------------
  // Send the velocity command to the motors
  // -------------------------------------------------------

  if (!debug)
  {
    write2motors(controled_RPM_left, controled_RPM_right);
  }



  // -------------------------------------------------------
  // Send the data for ROS
  // -------------------------------------------------------

  ros_loop(angular_speed_right, angular_speed_left,
           angle_encoder_read_left, angle_encoder_read_right,
           rpm_encoder_read_left, rpm_encoder_read_right,
           ticks_encoder_read_left, ticks_encoder_read_right,
           rpm_controled, controled_RPM_left,
           controled_RPM_left);

  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}