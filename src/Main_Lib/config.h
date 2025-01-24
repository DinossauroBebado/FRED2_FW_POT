#include <Arduino.h>


// -------------------------------------------------------
// Subscribers
// -------------------------------------------------------

#define cmd_wheels_topic "cmd_wheels"
#define cmd_rpm_topic "cmd/rpm"
#define cmd_vel_topic "cmd_vel/safe"
#define debug_control_status_topic "power/debug/control/status"
// #define cmd_led_strip_topic "cmd/led_strip/color"




// -------------------------------------------------------
// Publishers
// -------------------------------------------------------

#define pwm_right_topic "power/status/pwm/right"
#define pwm_left_topic "power/status/pwm/left"

#define angular_speed_right_topic "power/status/speed/angular/right"
#define angular_speed_left_topic "power/status/speed/angular/left"

#define rpm_speed_right_topic "power/status/speed/rpm/right"
#define rpm_speed_left_topic "power/status/speed/rpm/left"

#define ticks_left_topic "power/status/distance/ticks/left"
#define ticks_right_topic "power/status/distance/ticks/right"

#define seted_rpm_left_topic "power/status/seted/speed/rpm/left"
#define seted_rpm_right_topic "power/status/seted/speed/rpm/right"

#define vel_control_left_topic "power/status/speed/control/left"
#define vel_control_right_topic "power/status/speed/control/right"

//debug
#define rpm_controled_topic "power/status/debug/controler/rpm"
#define pwm_debug_topic "power/status/debug/pwm"




// -------------------------------------------------------
// Loop config
// -------------------------------------------------------

#define LOOP_TIME 200000  
#define  SERIAL_RATE  115200




// -------------------------------------------------------
// PWM config
// -------------------------------------------------------

#define SATURATION 1020
#define FREQUENCIA 1000  
#define PWM_SATURATION_UNDER 0

#define CANAL_M1 0
#define CANAL_M2 1





// -------------------------------------------------------
// Robot kinematics
// -------------------------------------------------------

#define L 0.3

#define RADIUS 0.075

#define MAX_PWM 1020
#define MIN_PWM -1020

#define MAX_RPM 1000
#define MIN_RPM -1000

#define MAX_SPEED_LINEAR 10 //m/s 
#define MAX_SPEED_ANGULAR 3.141592653589*2 //rad/s 





// -------------------------------------------------------
// Driver motor pins -> //! FRONT PCB 
// -------------------------------------------------------

#define  FM1_IN1  19
#define  FM1_IN2  23
#define  FM1_PWM  25

#define  FM2_IN1  18
#define  FM2_IN2  4
#define  FM2_PWM  13




// -------------------------------------------------------
// Driver motor pins -> //! BACK PCB 
// -------------------------------------------------------

#define  BM1_IN1  18
#define  BM1_IN2  4
#define  BM1_PWM  13

#define  BM2_IN1  19
#define  BM2_IN2  23
#define  BM2_PWM  25



// -------------------------------------------------------
// Degub LED
// -------------------------------------------------------

//debug led
#define  LED_BUILD_IN  2




// -------------------------------------------------------
// ENCODER -> programming pins
// -------------------------------------------------------

// CHANGE THESE AS APPROPRIATE
#define CSpin   5
#define CLKpin  18
#define DOpin   4
#define PROGpin 19

// OUTPUT PINS -- VERIFFFFY!!!
#define enc_a  17
#define enc_b  16
#define enc_z  2
