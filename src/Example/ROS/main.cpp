#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;
rcl_subscription_t subscriber;
std_msgs__msg__Int32 received_msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define RCCHECK(fn)                  \
  {                                  \
    rcl_ret_t temp_rc = fn;          \
    if ((temp_rc != RCL_RET_OK))     \
    {                                \
      error_loop();                  \
    }                                \
  }
#define RCSOFTCHECK(fn)              \
  {                                  \
    rcl_ret_t temp_rc = fn;          \
    if ((temp_rc != RCL_RET_OK))     \
    {                                \
      error_loop();                  \
    }                                \
  }

void error_loop()
{
  while (1)
  {
    Serial.printf("error_loop\n");
    delay(1);
  }
}

void subscription_callback(const void *msgin)
{
  const std_msgs__msg__Int32 *received_msg = (const std_msgs__msg__Int32 *)msgin;
  Serial.printf("Received: %d\n", received_msg->data);
}

void setup()
{
  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();

  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // Publisher Initialization
  RCCHECK(rclc_publisher_init_default(
      &publisher,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "micro_ros_platformio_node_publisher"));

  // Subscriber Initialization
  RCCHECK(rclc_subscription_init_default(
      &subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, received_msg, Int32),
      "micro_ros_platformio_node_subscriber"));

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));

  // Add subscriber to executor
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &received_msg, &subscription_callback, ON_NEW_DATA));
}

void loop()
{
  // Publish the message
  RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));

  // Increment the message data
  msg.data++;

  delay(100); // Add a delay to control the loop rate
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}
