
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"

static int threeSecondCount = 93750;
static int tX = 1, tY = 2;

uint32_t read_counter(){
    NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
    return NRF_TIMER3->CC[1];
}



void timer_init(){
  // fill in this function to initialize a timer of your choice
  
  NRF_TIMER4->TASKS_STOP = 1;
  NRF_TIMER4->TASKS_CLEAR =1;

  NRF_TIMER4->PRESCALER = 0x09;
  NRF_TIMER4->BITMODE = 0x02;
  NRF_TIMER4->MODE = 0x00;
  NRF_TIMER4->INTENSET =0x40000;//setup for 
   printf("%d INTENSET should be %d  \t\n", NRF_TIMER4->INTENSET, 0x40);
   NRF_TIMER4->SHORTS =0x01;

  NRF_TIMER4->CC[tY] = 0x16E36;
  //printf("%d Setup  CC[2]\t %d\n", NRF_TIMER4->CC[tY], NRF_TIMER4->EVENTS_COMPARE[tY]);
  
 
 
  NVIC_EnableIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 3);



  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;


  
  NRF_TIMER3 -> BITMODE = 0x02;
  NRF_TIMER3->MODE = 0x01;
  NRF_TIMER3->TASKS_START = 0x01;
  
  // 93750
  
  

}
