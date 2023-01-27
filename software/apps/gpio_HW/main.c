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





int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  

  //Adding Light struct
  
  volatile pins* const pinD = (pins*) 0x50000700;

  volatile gpio* const GPIO = (gpio*) 0x50000504;
 
  gpio_config(25, 1);
  gpio_config(24, 1);
  gpio_config(23, 1);
  

  gpio_config(22, 0);
  gpio_config(28, 0);
  //pinD->SWITCH0 = 0;
  pinD->SWITCH0 = 0 <<1;
  //pinD->BUTTON0 =0;
  pinD->BUTTON0 =0 <<1;

  //GPIO->OUTCLEAR = 1 << 28;//clear
  //GPIO->OUTCLEAR = 1 << 22;//clear


  //Setting Pins as inputs
  //GPIO->IN = 1 <<28;
  //GPIO->IN = 1 <<22;
  
  //printf("BUTTON0 %p\n\n", pinD->BUTTON0);
  //printf("SWITCH0 %p\n\n",pinD->SWITCH0);
  gpio_clear(25);
  gpio_clear(23);


  //not sure what affect this should do.
  pinD->BUTTON0 = 3 << 3;
  

  // loop forever
  while (1){
    
    bool x = gpio_read(22);
    bool y = gpio_read(28);

    

    if(!y){
       gpio_clear(24);
       gpio_clear(23);
       gpio_clear(25);
    }
    else
    {
      gpio_set( 24);
      gpio_set(23);
      gpio_set(25);
    }
    




    nrf_delay_ms(500);
     
  }
}
