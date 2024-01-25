#include <Arduino.h>

#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include "config.h"

#include <std_msgs/msg/int32.h>

rcl_publisher_t ticks_right_t;
std_msgs__msg__Int32 ticks_right_m;


#include <geometry_msgs/msg/twist.h>

rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;


float speed_linear ;
float speed_angular ; 

#define LED_PIN 2

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}


void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

//twist message cb
void subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;
  // if velocity in x direction is 0 turn off LED, if 1 turn on LED
  speed_linear = msg->linear.x ; 
  speed_angular = msg->angular.z;
//   digitalWrite(LED_PIN, (msg->linear.x == 0) ? LOW : HIGH);
}

float getLinear(){
    return speed_linear;
}

float getAngular(){
    return speed_angular;
}

void init_ros(){

  set_microros_transports();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  
  delay(2000);

  allocator = rcl_get_default_allocator();

   //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_power_node", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    cmd_vel_topic));
  
  RCCHECK(rclc_publisher_init_default(
    &ticks_right_t,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    ticks_right_topic));


  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));
}


void ros_loop(float speed_right,               float speed_left,
              double angle_encoder_read_left,  double angle_encoder_read_right,
              double rpm_encoder_read_left ,   double rpm_encoder_read_right,
              double ticks_encoder_read_left,  double ticks_encoder_read_right, 
              float rpm_controled,             float control_output_left, 
              float control_output_right){

              ticks_right_m.data =  ticks_encoder_read_right;

              RCSOFTCHECK(rcl_publish(&ticks_right_t, &ticks_right_m, NULL));


              }