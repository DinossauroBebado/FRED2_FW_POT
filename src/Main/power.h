
#include "motor.h"
#include "tools.h"

// fred(linear(m/s),angular(rad/s)) -> |cinematic| -> wheel(angular(rad/s)) -> |angular2rpm| -> wheel(angular(rpm)) -> |rpm2pwm| -> wheel(pwm)

//@vel PWM signal between 0 and 1023 
// postive our negative representes de direction, positive been forward

motor motor1(M1_IN1, M1_IN2, M1_PWM, 0);
motor motor2(M2_IN1, M2_IN2, M2_PWM, 1);
motor motor3(M3_IN1, M3_IN2, M3_PWM, 3);
motor motor4(M4_IN1, M4_IN2, M4_PWM, 4);


//Global 

int pwm_right = 0 ;
int pwm_left = 0  ; 
int pwm_motor = 0;

int OFFSET = 130;  //Offset fix the diference of velocity between front and back wheels 

void stop(motor motor){
  digitalWrite(motor.In_A, HIGH);  
  digitalWrite(motor.In_B, HIGH);
  ledcWrite(motor.Canal ,0);
}


void write_PWM(motor motor, int vel){

  //satured output

  if(vel>=SATURATION){
    vel = SATURATION;
  }
  if(vel<= -SATURATION){
    vel = -SATURATION;
  }
  
  //send to H bridge 
 
    ledcWrite(motor.Canal ,abs(vel));
    digitalWrite(motor.In_B, vel < 0);
    digitalWrite(motor.In_A, vel > 0);  
}

 int rpm2pwm(float speed_rpm){
  //scale convertion 
  // rpm - 0        pwm - 0
  // ------   =   --------------
  // max_rpm        max_pwm 

  

  float desired_pwm = 0;

  desired_pwm = (speed_rpm*MAX_PWM)/MAX_RPM;

  

  return desired_pwm;
}

 float angular2rpm(float speed_angular){
  //convert from angular w to rpm 
  
  float desired_rpm = (speed_angular*60)/(2*PI)  ;

  return desired_rpm;
}



void write2motors(int rpm_left, int rpm_right) {

  pwm_left  = rpm2pwm(rpm_left);
  pwm_right = rpm2pwm(rpm_right);

  int front_gain;  // Declare front_gain outside the if-else blocks

  if (pwm_left >= OFFSET && pwm_right >= OFFSET) {
    front_gain = -130;
  } else {
    front_gain = 0;
  }

  write_PWM(motor1, pwm_right + front_gain); 
  write_PWM(motor2, pwm_right); 
  write_PWM(motor3, pwm_left); 
  write_PWM(motor4, pwm_left + front_gain); 
}


void write2motor(int rpm,int motor){
  pwm_motor = rpm2pwm(rpm);

  switch (motor)  {
    case 1:
       write_PWM(motor1,pwm_motor);
        break;

    case 2:
       write_PWM(motor2,pwm_motor);
        break;
    case 3:
       write_PWM(motor3,pwm_motor);
        break;
     case 4:
       write_PWM(motor4,pwm_motor);
        break;
    default:
          write_PWM(motor1,pwm_motor);
}

}




