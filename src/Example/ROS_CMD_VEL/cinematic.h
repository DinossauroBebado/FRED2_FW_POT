
#include <Main/config.h>
#include <geometry_msgs/Twist.h>

float speed_linear ;
float speed_angular ; 

float cmd_rpm; 

bool debug = false; 

//receive the twsit msg and figure out the speed of each wheel 
void cmdVelCB( const geometry_msgs::Twist& twist)
{

  speed_angular = twist.angular.z;
  speed_linear  = twist.linear.x;

  // if (speed_angular < 0.1)
  //   speed_angular = 0; 
  // if (speed_linear < 0.1)
  //   speed_linear = 0; 
}

void cmdRPMCB(const std_msgs::Float32& msgs){
    cmd_rpm = msgs.data;

}

void debugControlCB(const std_msgs::Bool& msg){

  debug = msg.data; 

}

bool debugControl(){
  return debug;
}


float getRPMsetpoint(){
    return cmd_rpm; 
}

float getLinear(){
    return speed_linear;
}

float getAngular(){
    return speed_angular;
}

// //return left wheel speed in radians/sec 
// float cinematic_left(float linear, float angular,float gain){

//   return gain*(linear - angular*(L/2))/RADIUS;
// }

// //return right wheel speed in radians/sec 
// float cinematic_right(float linear, float angular, float gain){

//   return gain*(linear + angular*(L/2))/RADIUS;
// }

//return left wheel speed in radians/sec 
float cinematic_left(float linear, float angular, float gain){

  return gain*(linear - angular*L);
}

//return right wheel speed in radians/sec 
float cinematic_right(float linear, float angular, float gain){

  return gain*(linear + angular*L);
}




