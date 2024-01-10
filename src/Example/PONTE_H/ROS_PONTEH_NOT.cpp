
// #include <Arduino.h>
// #include <ros.h>
// #include <std_msgs/Float32.h>

// #include "config.h"

// const int freq = 5000; 

// const int m1_channel = 0; 
// const int m2_channel = 1; 
// const int m3_channel = 2; 
// const int m4_channel = 3; 

// const int resolution = 8;  


// ros::NodeHandle nh; 

// int speed = 0; 

// void velocity_control(float speed, int motor, int s1, int s2) 
// {
//   if (speed > 100)
//   speed = 100; 

//   if (speed < -100)
//   speed = -100;
  
//   speed = map(speed, -100, 100, -255, 255); 

//   if (speed >= 0)
//   {
//     digitalWrite(s1, 0); 
//     digitalWrite(s2, 1); 
//   }

//   else
//   {
//     digitalWrite(s1, 1); 
//     digitalWrite(s2, 0); 
//   }

//   ledcWrite(motor, abs(speed)); 

// }

// void motorLF_cb (const std_msgs::Float32 &m1_speed)        //motor esquerdo dianteiro
// {
//   velocity_control(m1_speed.data, pwm_m1, s1_m1, s2_m1);
//   digitalWrite(2, HIGH-digitalRead(2));
// }

// void motorRF_cb (const std_msgs::Float32 &m2_speed)        //motor direito dianteiro
// {
//   velocity_control(m2_speed.data, pwm_m2, s1_m2, s2_m2); 
//   digitalWrite(2, HIGH-digitalRead(2));
// }

// void motorLB_cb (const std_msgs::Float32 &m3_speed)        //motor esquerdo traseiro
// {
//   velocity_control(m3_speed.data, pwm_m3, s1_m3, s2_m3); 
//   digitalWrite(2, HIGH-digitalRead(2));
// }

// void motorRB_cb (const std_msgs::Float32 &m4_speed)       //motor direito traseiro 
// {
//   velocity_control(m4_speed.data, pwm_m4, s1_m4, s2_m4); 
//   digitalWrite(2, HIGH-digitalRead(2));
// }

// ros::Subscriber<std_msgs::Float32> sub_motor_leftFront("cmd/pwm/motorLF",  &motorLF_cb);
// ros::Subscriber<std_msgs::Float32> sub_motor_rightFront("cmd/pwm/motorRF",  &motorRF_cb);
// ros::Subscriber<std_msgs::Float32> sub_motor_leftBack("cmd/pwm/motorLB", &motorLB_cb);
// ros::Subscriber<std_msgs::Float32> sub_motor_rightBack("cmd/pwm/motorRB", &motorRB_cb);
// void setup()
// { 
//   pinMode(pwm_m1, OUTPUT);
//   pinMode(pwm_m2, OUTPUT); 
//   pinMode(pwm_m3, OUTPUT); 
//   pinMode(pwm_m4, OUTPUT);

//   pinMode(s1_m1, OUTPUT); 
//   pinMode(s2_m1, OUTPUT); 

//   pinMode(s1_m2, OUTPUT); 
//   pinMode(s2_m2, OUTPUT); 

//   pinMode(s1_m3, OUTPUT); 
//   pinMode(s2_m3, OUTPUT); 

//   pinMode(s1_m4, OUTPUT); 
//   pinMode(s2_m4, OUTPUT); 

//   ledcSetup(m1_channel, freq, resolution);
//   ledcSetup(m2_channel, freq, resolution);
//   ledcSetup(m3_channel, freq, resolution);
//   ledcSetup(m4_channel, freq, resolution);

//   ledcAttachPin(pwm_m1, m1_channel);
//   ledcAttachPin(pwm_m2, m2_channel);
//   ledcAttachPin(pwm_m3, m3_channel);
//   ledcAttachPin(pwm_m4, m4_channel);

//   nh.initNode(); 
//   nh.subscribe(sub_motor_leftFront); 
//   nh.subscribe(sub_motor_rightFront); 
//   nh.subscribe(sub_motor_leftBack); 
//   nh.subscribe(sub_motor_rightBack);

//   ledcWrite(pwm_m1, speed); 
//   ledcWrite(pwm_m2, speed); 
//   ledcWrite(pwm_m3, speed); 
//   ledcWrite(pwm_m4, speed); 
// }

// void loop()
// {
//  nh.spinOnce(); 
//   delay(1); 
// }