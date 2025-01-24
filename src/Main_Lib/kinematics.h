
#include <Main_Lib/config.h>



float cmd_rpm; 
bool debug = false; 




// -------------------------------------------------------
// Kinematics debug function
// -------------------------------------------------------

bool debugControl(){
  return debug;
}




// -------------------------------------------------------
// Kinematics debug function
// -------------------------------------------------------

float getRPMsetpoint(){
    return cmd_rpm; 
}




// -------------------------------------------------------
// Return left wheel speed in rad/s
// -------------------------------------------------------

float kinematics_left(float linear, float angular, float gain){

  return gain*(linear - angular*L);
}




// -------------------------------------------------------
// Return right wheel speed in rad/s
// -------------------------------------------------------
 
float kinematics_right(float linear, float angular, float gain){

  return gain*(linear + angular*L);
}




