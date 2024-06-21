#include <Arduino.h>

#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <Main_Lib/config.h>
#include <std_msgs/msg/int32.h>
#include <geometry_msgs/msg/twist.h>

// Defina o intervalo de timeout em milissegundos
#define TIMEOUT_INTERVAL_MS 1000

// Variável para armazenar o último tempo em que cmd_vel foi recebido
unsigned long last_cmd_vel_time = 0;

const char * namespace_ = "esp_front";

rcl_subscription_t cmd_vel_subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;



rcl_publisher_t ticks_right_t;
std_msgs__msg__Int32 ticks_right_m;

rcl_publisher_t ticks_left_t;
std_msgs__msg__Int32 ticks_left_m;


rcl_publisher_t angular_speed_right_t;
std_msgs__msg__Int32 angular_speed_right_m;

rcl_publisher_t angular_speed_left_t;
std_msgs__msg__Int32 angular_speed_left_m;


rcl_publisher_t rpm_speed_right_t;
std_msgs__msg__Int32 rpm_speed_right_m;

rcl_publisher_t rpm_speed_left_t;
std_msgs__msg__Int32 rpm_speed_left_m;


rcl_publisher_t pwm_right_t;
std_msgs__msg__Int32 pwm_right_m;

rcl_publisher_t pwm_left_t;
std_msgs__msg__Int32 pwm_left_m;


rcl_publisher_t vel_control_left_t;
std_msgs__msg__Int32 vel_control_left_m;

rcl_publisher_t vel_control_right_t;
std_msgs__msg__Int32 vel_control_right_m;




float speed_linear ;
float speed_angular ; 

#define LED_PIN 2

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}


void error_loop(){
  for (int i = 0; i<10; i++)
  {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
  ESP.restart();
}

//twist message cb
void subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;
  // if velocity in x direction is 0 turn off LED, if 1 turn on LED
  speed_linear = msg->linear.x ; 
  speed_angular = msg->angular.z;

  // Atualiza o tempo do último comando recebido
  last_cmd_vel_time = millis();
//   digitalWrite(LED_PIN, (msg->linear.x == 0) ? LOW : HIGH);
}

float getLinear(){
    return speed_linear;
}

float getAngular(){
    return speed_angular;
}

// Função para verificar e zerar as velocidades se o timeout ocorrer
void check_cmd_vel_timeout() {
  unsigned long current_time = millis();
  // Verifica se o tempo decorrido desde o último comando recebido excede o intervalo de timeout
  if (current_time - last_cmd_vel_time > TIMEOUT_INTERVAL_MS) {
    // Zera as velocidades linear e angular
    speed_linear = 0;
    speed_angular = 0;
  }
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
  RCCHECK(rclc_node_init_default(&node, "micro_ros_power_node", namespace_, &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &cmd_vel_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    cmd_vel_topic));

  //create publisher for encoder ticks right 
  RCCHECK(rclc_publisher_init_default(
    &ticks_right_t,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    ticks_right_topic));

  //create publisher for encoder ticks left
  RCCHECK(rclc_publisher_init_default(
  &ticks_left_t,
  &node,
  ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
  ticks_left_topic));

  
  //create publisher for angular_speed_right 
  RCCHECK(rclc_publisher_init_default(
  &angular_speed_right_t,
  &node,
  ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
  angular_speed_right_topic));

  //create publisher for angular_speed_left
  RCCHECK(rclc_publisher_init_default(
  &angular_speed_left_t,
  &node,
  ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
  angular_speed_left_topic));

  //create publisher for rpm_speed_left
  RCCHECK(rclc_publisher_init_default(
  &rpm_speed_left_t,
  &node,
  ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
  rpm_speed_left_topic));

  //create publisher for rpm_speed_right
  RCCHECK(rclc_publisher_init_default(
  &rpm_speed_right_t,
  &node,
  ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
  rpm_speed_right_topic));




  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &cmd_vel_subscriber, &msg, &subscription_callback, ON_NEW_DATA));
}


void ros_loop(float speed_right,               float speed_left,
              double angle_encoder_read_left,  double angle_encoder_read_right,
              double rpm_encoder_read_left ,   double rpm_encoder_read_right,
              double ticks_encoder_read_left,  double ticks_encoder_read_right, 
              float rpm_controled,             float control_output_left, 
              float control_output_right){

              ticks_right_m.data =  ticks_encoder_read_right;
              RCSOFTCHECK(rcl_publish(&ticks_right_t, &ticks_right_m, NULL));

              ticks_left_m.data =  ticks_encoder_read_left;
              RCSOFTCHECK(rcl_publish(&ticks_left_t, &ticks_left_m, NULL));


              angular_speed_left_m.data = angle_encoder_read_left;
              RCSOFTCHECK(rcl_publish(&angular_speed_left_t, &angular_speed_left_m, NULL));

              angular_speed_right_m.data = angle_encoder_read_right;
              RCSOFTCHECK(rcl_publish(&angular_speed_right_t, &angular_speed_right_m, NULL));


              rpm_speed_left_m.data = rpm_encoder_read_left;
              RCSOFTCHECK(rcl_publish(&rpm_speed_left_t, &rpm_speed_left_m, NULL));

              rpm_speed_right_m.data = rpm_encoder_read_right;
              RCSOFTCHECK(rcl_publish(&rpm_speed_right_t, &rpm_speed_right_m, NULL));

              // Verifica o timeout do comando cmd_vel
              check_cmd_vel_timeout();

              }