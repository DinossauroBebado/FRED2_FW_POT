#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>


#include "controler.h"
#include "config.h"

#include "rampa.h"

float linear, angular ;

const int ACC = 50 ;
// const int GAIN = 1 ;
const int GAIN = 10 ;
const int GAIN_ANGULAR = 7;


rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;


#define LED_PIN 13

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
// cmd------ 

float angular_speed_left = cinematic_left(linear,angular,GAIN); //wheel [rad/s]

rpm_left = angular2rpm(angular_speed_left);// [RPM]
float rpm_left_com_rampa = rampa(rpm_left, 200, LEFT);
// rpm_left =  saturation(rpm_left,800);

// float controled_RPM_left = rpm_left;
float controled_RPM_left = esquerda_controler.output(rpm_left, rpm_encoder_read_left);

write2motors(controled_RPM_left,controled_RPM_right);

}

void setup() {
  set_microros_transports();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  
  delay(2000);

  allocator = rcl_get_default_allocator();

   //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "micro_ros_arduino_twist_subscriber"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));

}

void loop() {
  delay(100);
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}