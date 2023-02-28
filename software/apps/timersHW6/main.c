

//** Code: Hard Timers (Lab 6)
//** Author: Umer Huzaifa
//** Date: 12/30/2021
//** Comments: Keeping a separate app for the hardware timers and counters


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
#include "gpio.h"

static int threeSecondCount = 93750;
static int tX = 1, tY = 2;
uint32_t read_counter(){

  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
  return NRF_TIMER3->CC[1];

  // fill in this function for reading the timer value on calling this function
}

void TIMER4_IRQHandler(void){
  NRF_TIMER4->EVENTS_COMPARE[tY]= 0;


  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;

  NRF_TIMER3->TASKS_COUNT = 0x01;
  printf("3 Seconds and triggered an interupt!!! %d\n", read_counter());  

  /*if(NRF_TIMER4->EVENTS_COMPARE[tY]){
      NRF_TIMER4->EVENTS_COMPARE[tY]=0;
      NRF_TIMER4->TASKS_CLEAR = 0x01;
      NRF_TIMER4->TASKS_START = 0x01;


      

    }*/
}

void GPIOTE_IRQHandler(void) {//Already written in part 
    NRF_GPIOTE->EVENTS_IN[0] = 0;
   
    
    printf("Button Pressed + Current Count %d\n", read_counter());
    nrf_delay_ms(500);
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

uint32_t read_timer(){


  NRF_TIMER4->TASKS_CAPTURE[tX] = 0x01;
  return NRF_TIMER4->CC[tX];

  // fill in this function for reading the timer value on calling this function
}



int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Board initialized!\n");

  // You can use the NRF GPIO library to test your timers
  nrf_gpio_pin_dir_set(BUCKLER_LED0, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED1, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(BUCKLER_LED2, NRF_GPIO_PIN_DIR_OUTPUT);

  timer_init(); 
  setDevices(); 
  LABHandler();


  nrf_delay_ms(4000);

  printf("%d value of Compare", NRF_TIMER4->EVENTS_COMPARE[tY]);

  // loop forever
  while (1) {
    
    if(!getInput(28))
    {
      NRF_TIMER3->TASKS_COUNT = 0x01;
    }
    //printf("Timer 4 readings are: %d\t  Counter 3 readings are: %d\n", read_timer(), read_counter());
    /*if(NRF_TIMER4->EVENTS_COMPARE[tY]){
      NRF_TIMER4->EVENTS_COMPARE[tY]=0;
      NRF_TIMER4->TASKS_CLEAR = 0x01;
      NRF_TIMER4->TASKS_START = 0x01;

      printf("3 Seconds happenned");

*/

      

    }
    // for better readability on the terminal window, adding some delay is useful
    nrf_delay_ms(1000);
  }
  

