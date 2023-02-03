#include "gpio.h"

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



static uint32_t* OUTSET = (uint32_t*) 0x50000508;
static uint32_t* OUTCLEAR = (uint32_t*) 0x5000050C;
static uint32_t* CNF0 = (uint32_t*) 0x5000070C;

static uint32_t* IN = (uint32_t*) 0x50000510;
static uint32_t* DIR = (uint32_t*) 0x5000514;






void setDevices(){
	 ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  

  //Adding Light struct
  
  volatile pins* const pinD = (pins*) 0x50000700;

  volatile gpio* const GPIO = (gpio*) 0x50000504;



  //Setting up Lights
  gpio_config(25, 1);
  gpio_config(24, 1);
  gpio_config(23, 1);

  //Setting up Buttons
  gpio_config(22, 0);
  gpio_config(28, 0);

 
  gpio_clear(25);
  gpio_clear(23);


}

void deviceLoop(){ //handles color and r
	bool x = gpio_read(22);
    bool y = gpio_read(28);

    
    if(x) // if Switch is Low
    {
       gpio_clear( 23);
       gpio_set( 25);

       if(y){

       }



    }
    else //Switch is low
    {
     gpio_set (23); 
     gpio_clear(25);
     
    

    }

   
}


bool getInput(int x){
	//todo Make this work for more things. 
	return gpio_read(x);
}