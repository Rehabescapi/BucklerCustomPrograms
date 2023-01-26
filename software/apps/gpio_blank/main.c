// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"


#include "buckler.h"
#include "gpio.h"


//Lab3 Q1 Changed to uint32 from uint8 after rewatching lecture
//

static uint32_t* OUTSET = (uint32_t*) 0x50000508;
static uint32_t* OUTCLEAR = (uint32_t*) 0x5000050C;
static uint32_t* CNF0 = (uint32_t*) 0x5000070C;

static uint32_t* IN = (uint32_t*) 0x50000510;
static uint32_t* DIR = (uint32_t*) 0x5000514;



//static uint32_t* Button0
//static unit32_t* SWITCH0


//0x50000000 + 0x700 + pin_num *4

//Recreating Blink P1
/*
typedef struct 
{
   uint32_t  var_OUT;
   uint32_t var_IN;
   uint32_t var_DIR; 
   uint32_t arr[32];
    
}gpio_t;
*/

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  

  //Adding Light struct
  
  volatile pins* const pinD = (pins*) 0x50000700;
  volatile gpio* const GPIO = (gpio*) 0x50000508;
 
  
  //hardwareptr->arr= &PIN0;//0x50000700
  
 pinD->BLUE = 1;
  
  pinD->YLLO = 1; // Yellow
  pinD->RED =1; // Red

  pinD->SWITCH0 = 0;
  pinD->SWITCH0 = 1 <<1;
  pinD->BUTTON0 =0;
  pinD->BUTTON0 =1 <<1;

  GPIO->OUTCLEAR = 1 <<28;//clear
  GPIO->OUTCLEAR = 1 <<22;//clear


  //Setting Pins as inputs
  *IN = 1 <<28;
  *IN = 1 <<22;

  GPIO->OUTCLEAR =  1 << 25; //clear
  printf("Turning on 25.");
  nrf_delay_ms(3000);
  GPIO->OUTSET = 1 << 25;
  printf("Turning off Blue\n");


  bool temp = true;

  // loop forever
  while (1){
    //printf("Starting loop\n");

    
   
    if(temp)
    {
      //printf("toggle back on\n");
      temp = !temp;
      GPIO->OUTCLEAR = 1 <<23;
      GPIO->OUTSET = 1 <<25;

      
      

    }
    else
    {
      temp = true;
      
      GPIO->OUTSET =  1 << 23; 
      GPIO->OUTCLEAR =1 <<25;
    }




    nrf_delay_ms(1000);
  }
}

