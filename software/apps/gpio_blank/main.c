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



static uint32_t*  PIN23 = (uint32_t*) 0x5000075C;
static uint32_t*  PIN24 = (uint32_t*) 0x50000760;
static uint32_t*  PIN25 = (uint32_t*) 0x50000764;

//static uint32_t* Button0
//static unit32_t* SWITCH0


//0x50000000 + 0x700 + pin_num *4

//Recreating Blink P1


int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  

  //Blink 
 
  

  *PIN23 =1;// Blue
  //*PIN24 = 1; // Yellow
  *PIN25 =1; // Red
  *OUTCLEAR =  1 << 25; 
  printf("Turning on 25.");
  nrf_delay_ms(3000);
  *OUTSET = 1 << 25;
  printf("Turning off Blue\n");


  bool temp = true;

  // loop forever
  while (1){
    printf("Starting loop\n");
   
    if(temp)
    {
      printf("toggle back on\n");
      temp = !temp;
      *OUTCLEAR = 1 <<23;
      *OUTSET = 1 <<25;

      
      

    }
    else
    {
      temp = true;
      
      *OUTSET =  1 << 23; 
      *OUTCLEAR =1 <<25;
    }




    nrf_delay_ms(1000);
  }
}

