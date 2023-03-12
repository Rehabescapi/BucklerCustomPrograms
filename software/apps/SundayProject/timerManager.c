#include <stdint.h>
static int FourSecondCount = 125000;
static int tY= 2;

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "app_error.h"



#include "app_timer.h"



#include "bsp.h"
#include "nrf_delay.h"
#include "app_pwm.h"

//catch all 
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "nrfx_gpiote.h"
//bookKeeping files
#include "pwmManager.h"
#include "gpio.h"



uint32_t read_counter(){
    

  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
  printf("%d count" , NRF_TIMER3->CC[1]);
  return NRF_TIMER3->CC[1];

  // fill in this function for reading the timer value on calling this function
}