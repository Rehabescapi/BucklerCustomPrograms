
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
static int tY= 2;

uint32_t read_counter(){
    

  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
  printf("%d count" , NRF_TIMER3->CC[1]);
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
/*
void GPIOTE_IRQHandler(void) {//Already written in part 
    
  
    if(NRF_GPIOTE->EVENTS_IN[0] == 1){
      NRF_GPIOTE->EVENTS_IN[0] = 0;

       printf("Button interrupt received\n\n");
    //nrf_delay_ms(100);
    gpio_clear(25);
    nrf_delay_ms(2000);
    gpio_set(25);

    }else{
      NRF_GPIOTE->EVENTS_IN[1] =0;
      printf("switch interrupt received\n\n");
      gpio_clear(24);
      nrf_delay_ms(2000);
      gpio_set(24);}
}*/
//Functions intention is to trip when Pin# reaches a high level.
void LABHandler(void){
  
  printf("Starting conditions \nConfig[0] is %p\n And INTENSET = %p",NRF_GPIOTE->CONFIG[0], NRF_GPIOTE->INTENSET);
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);
  APP_ERROR_CHECK(nrfx_gpiote_out_init(24, &out_config));
  nrfx_gpiote_out_task_enable(24);
    // Configure input pin
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
  in_config.pull = NRF_GPIO_PIN_PULLUP;
  APP_ERROR_CHECK(nrfx_gpiote_in_init(28, &in_config, in_pin_handler));
    nrfx_gpiote_in_event_enable(28, true);

  printf("Lab_Handled\n\n");
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

  NRF_TIMER4->CC[tY] = 0x1E848;//Actual thing that is stopping x

  NVIC_EnableIRQ(TIMER4_IRQn);// when CC[ty] is reached flag interrupt
  NVIC_SetPriority(TIMER4_IRQn, 0);

  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;


  //Count every 4 seconds
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

    //while(app_pwm_channel_duty_set(&PWM1, 1, 20) == NRF_ERROR_BUSY);
    
    
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

