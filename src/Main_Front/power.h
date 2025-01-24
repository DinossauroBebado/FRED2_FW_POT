
#include <Main_Lib/motor.h>
#include <Main_Lib/tools.h>
#include <Main_Lib/config.h>

// fred(linear(m/s),angular(rad/s)) -> |cinematic| -> wheel(angular(rad/s)) -> |angular2rpm| -> wheel(angular(rpm)) -> |rpm2pwm| -> wheel(pwm)

//@vel PWM signal between 0 and 1023 
// postive our negative representes de direction, positive been forward

motor motor1(FM1_IN1, FM1_IN2, FM1_PWM, 0);
motor motor2(FM2_IN1, FM2_IN2, FM2_PWM, 1);


//Global 

int pwm_right = 0 ;
int pwm_left = 0  ; 
int pwm_motor = 0;


// -------------------------------------------------------
// Stop motors function
// -------------------------------------------------------

void stop(motor motor){
  digitalWrite(motor.In_A, HIGH);  
  digitalWrite(motor.In_B, HIGH);
  ledcWrite(motor.Canal ,0);
}



// -------------------------------------------------------
// Write the PWM command at the motor driver
// -------------------------------------------------------

void write_PWM(motor motor, int vel){

  //satured output

  if(vel>=SATURATION){
    vel = SATURATION;
  }
  if(vel<= -SATURATION){
    vel = -SATURATION;
  }

  ledcWrite(motor.Canal ,abs(vel));
  digitalWrite(motor.In_B, vel < 0);
  digitalWrite(motor.In_A, vel > 0);  
}


// -------------------------------------------------------
// Converts RPM to PWM
// -------------------------------------------------------

int rpm2pwm(float speed_rpm){

//scale convertion 
// rpm - 0        pwm - 0
// ------   =   --------------
// max_rpm        max_pwm 

  float desired_pwm = 0;

  desired_pwm = (speed_rpm*MAX_PWM)/MAX_RPM;

  return desired_pwm; 
}




// -------------------------------------------------------
// Converts RAD/S to RPM
// -------------------------------------------------------

float angular2rpm(float speed_angular){
  //convert from angular w to rpm 
  
  float desired_rpm = (speed_angular*60)/(2*PI)  ;

  return desired_rpm;
}





// -------------------------------------------------------
// Receives the RPM command to sent PWM for both motors 
// -------------------------------------------------------

void write2motors(int rpm_left, int rpm_right) {

  pwm_left  = rpm2pwm(rpm_left);
  pwm_right = rpm2pwm(rpm_right);

  write_PWM(motor1, pwm_right); 
  write_PWM(motor2, pwm_left); 
}




// -------------------------------------------------------
// Receives the RPM command to sent PWM for a single motor 
// -------------------------------------------------------

void write2motor(int rpm,int motor){
  pwm_motor = rpm2pwm(rpm);

  switch (motor)  {
    case 1:
       write_PWM(motor1,pwm_motor);
        break;

    case 2:
       write_PWM(motor2,pwm_motor);
        break;
    default:
          write_PWM(motor1,pwm_motor);
  }

}




