
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

// Create the instance "PWM1" using TIMER1.
static int FourSecondCount = 125000;
static int OneSecondCount = 31250;
static int tY= 2;

uint32_t read_counter(){
    

  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;
  return NRF_TIMER3->CC[1];

  // fill in this function for reading the timer value on calling this function
}

void TIMER4_IRQHandler(void){
  NRF_TIMER4->EVENTS_COMPARE[tY]= 0;
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;
  
  NRF_TIMER3->TASKS_COUNT = 0x01;
  
  printf("3 Seconds and triggered an interupt!!! %lu\n", read_counter());  
}

static void in_pin_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    nrfx_gpiote_out_task_trigger(24);
}





void timer_init(){
  // fill in this function to initialize a timer of your choice
  
  NRF_TIMER4->TASKS_STOP = 1;
  NRF_TIMER4->TASKS_CLEAR =1;
  NRF_TIMER4->PRESCALER = 0x09;
  NRF_TIMER4->BITMODE = 0x02;
  NRF_TIMER4->MODE = 0x00;
  NRF_TIMER4->INTENSET =0x40000;//setup for 
  NRF_TIMER4->SHORTS =0x01;

  NRF_TIMER4->CC[tY] = 0x7A12;//Actual thing that is stopping x

  NVIC_EnableIRQ(TIMER4_IRQn);// when CC[ty] is reached flag interrupt
  NVIC_SetPriority(TIMER4_IRQn, 0);

  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;


  //Count every 1 seconds
  NRF_TIMER3 -> BITMODE = 0x02;
  NRF_TIMER3->MODE = 0x01;
  NRF_TIMER3->TASKS_START = 0x01;
  
}

uint32_t read_timer(){


  NRF_TIMER4->TASKS_CAPTURE[1] = 0x01;
  return NRF_TIMER4->CC[1];

    // fill in this function for reading the timer value on calling this function
}






int main(void)
{
    pwm_init();
    timer_init();
    LABHandler();

    
    uint32_t value = 0;
    printf("start of while loop");
    while (true)
    {
        /* Your code for modifying the duty cycle value */
        for (value = 0; value <= 100; value++)
        {
            setTwoChannel( value);
            
            // for catching any errors returned
             nrf_delay_ms(100);
        }

    }

}

