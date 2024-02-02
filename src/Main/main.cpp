#include "micro_ros.h"
#include <Arduino.h>
#include <Main/config.h>
#include <Main/controler.h>
#include "rampa.h"
#include "cinematic.h"
#include "power.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"

TaskHandle_t taskSubHandle = NULL;
TaskHandle_t taskPubHandle = NULL;

void vTaskSub(void *pvParameters);
void vTaskPub(void *pvParameters);

#include "freertos/semphr.h"

SemaphoreHandle_t dataMutex;
//TODO 
//stil need to add some topics 
//divide task in two cores 
// write docs 


#include "encoder.h"
Encoder encoder(34, 35, 39,36);

#include "MedianFilter.h"
MedianFilter encoderRightFilter(33,0);
MedianFilter encoderLeftFilter(33,0);

#include "controler.h"
Controler  esquerda_controler(1, 0, 0);  //(p,i,d)
Controler  direita_controler(1, 0, 0);  //(p,i,d) ->0.4

const int ACC = 50 ;
// const int GAIN = 1 ;
const int GAIN = 10 ;
const int GAIN_ANGULAR = 7;

bool _connect = false ;

float rpm_right = 0 ;
float rpm_left = 0;

float rpm = 0;
float rpm_controled = 0;

float controled_RPM_right;
float controled_RPM_left ;

volatile double ticks_encoder_read_left;
volatile double ticks_encoder_read_right;
volatile double rpm_encoder_read_left;
volatile double rpm_encoder_read_right;
volatile double angle_encoder_read_left;
volatile double angle_encoder_read_right;
volatile float angular_speed_left;
volatile float angular_speed_right;





void setup() {
  //Serial.begin(115200);
  init_ros();
  encoder.setup();
  // Criação do semáforo
  dataMutex = xSemaphoreCreateMutex();
  // Criação das Tasks
  BaseType_t result = xTaskCreatePinnedToCore(vTaskSub, "vTaskSub", configMINIMAL_STACK_SIZE+8192, NULL, 2, &taskSubHandle, PRO_CPU_NUM);
  if (result != pdPASS) {
   //Serial.println("Erro ao criar vTaskSub!");
  }
  result = xTaskCreatePinnedToCore(vTaskPub, "vTaskPub", configMINIMAL_STACK_SIZE+8192, NULL, 2, &taskPubHandle, APP_CPU_NUM);
  if (result != pdPASS) {
   //Serial.println("Erro ao criar vTaskPub!");
  }
}

void loop() {
  // RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  vTaskDelay(10);
}

void vTaskSub(void *pvParameters)
{
  while(1)
  {
    
      float linear = getLinear();//robot
      float angular = getAngular();//robot
    // Adquira o semáforo antes de acessar as variáveis globais
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {

      // debug = debugControl();

      //---------------------LEFT-------------------------------------------
      
      // status -------encoder 
      angle_encoder_read_left  = encoder.readAngle(LEFT);

      rpm_encoder_read_left = encoder.readRPM(LEFT);
      encoderLeftFilter.in(rpm_encoder_read_left);

      rpm_encoder_read_left = encoderLeftFilter.out();

      ticks_encoder_read_left = encoder.readPulses(LEFT);

      // cmd------ 

      angular_speed_left = cinematic_left(linear,angular,GAIN); //wheel [rad/s]

      rpm_left = angular2rpm(angular_speed_left);// [RPM]
      float rpm_left_com_rampa = rampa(rpm_left, 10, LEFT);
      // rpm_left =  saturation(rpm_left,800);

      // float controled_RPM_left = rpm_left;
      float controled_RPM_left = esquerda_controler.output(rpm_left, rpm_encoder_read_left);
      // float controled_RPM_left = esquerda_controler.output(rpm_left,rpm_encoder_read_left);


      //------------------------------RIGHT-------------------------------------------

      //status -- encoder 

      angle_encoder_read_right = encoder.readAngle(RIGHT);

      rpm_encoder_read_right = encoder.readRPM(RIGHT);
      encoderRightFilter.in(rpm_encoder_read_right);
      rpm_encoder_read_right = encoderRightFilter.out();

      ticks_encoder_read_right = encoder.readPulses(RIGHT);

      //cmd -- 

      angular_speed_right = cinematic_right(linear,angular,GAIN); //wheel [RAD/S]

      rpm_right = angular2rpm(angular_speed_right);   // [RPM]
      float rpm_right_com_rampa = rampa(rpm_right, 10, RIGHT); //not used

      float controled_RPM_right = direita_controler.output(rpm_right, rpm_encoder_read_right); //not used

    //----------------debug------------------------------
      if(debug){
        rpm = getRPMsetpoint();
        rpm_controled = direita_controler.output(rpm,rpm_encoder_read_right);
        write2motor(rpm_controled,2);
      //write2motor(rpm,2);
      }
      //--------------------------execute-----------------

      if(!debug){
      write2motors(controled_RPM_left,controled_RPM_right);
      }
      xSemaphoreGive(dataMutex);
    }
    vTaskDelay(10/portTICK_RATE_MS);
    RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  }
}

void vTaskPub(void *pvParameters)
{
  while(1)
  {
    if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) 
    {
      ros_loop(    angular_speed_right,       angular_speed_left,
                  angle_encoder_read_left,   angle_encoder_read_right,
                  rpm_encoder_read_left ,    rpm_encoder_read_right,
                  ticks_encoder_read_left,   ticks_encoder_read_right, 
                  rpm_controled,              controled_RPM_left, 
                  controled_RPM_left);
      xSemaphoreGive(dataMutex);
      // Em qualquer ponto do código para verificar o uso de heap
    }
    RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
    vTaskDelay(10/portTICK_RATE_MS);
  }
}