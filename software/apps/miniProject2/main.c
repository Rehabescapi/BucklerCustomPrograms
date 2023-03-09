
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "app_error.h"
#include "app_timer.h"
#include "bsp.h"
#include "nrf_delay.h"
#include "app_pwm.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"

//bookKeeping files
#include "pwmManager.h"

#include "nrf_drv_gpiote.h"


// Create the instance "PWM1" using TIMER1.
static int FourSecondCount = 125000;
#define PIN_IN  28
#define COUNT_READ_INTERVAL 10
static int tY= 2;

uint32_t read_counter(){
    

  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
  printf("%d count" , NRF_TIMER3->CC[1]);
  return NRF_TIMER3->CC[1];

  // fill in this function for reading the timer value on calling this function
}

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)

{
    printf("\n In Pin Triggered \n\n");
    //NRF_LOG_INFO("H");

}


void TIMER4_IRQHandler(void){
  NRF_TIMER4->EVENTS_COMPARE[tY]= 0;
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;
  
  
  printf("3 Seconds and triggered an interupt!!! %lu\n", read_counter());  
}





static void gpiote_init(void)

{

    ret_code_t err_code;



    err_code = nrf_drv_gpiote_init();

    APP_ERROR_CHECK(err_code);



    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(false);

    in_config.pull = NRF_GPIO_PIN_PULLDOWN;



    err_code = nrf_drv_gpiote_in_init(PIN_IN, &in_config, in_pin_handler);

    APP_ERROR_CHECK(err_code);

    

    nrf_drv_gpiote_in_event_enable(PIN_IN, false);

}
    
    



void timer_init(){
  // fill in this function to initialize a timer of your choice
  
  NRF_TIMER4->TASKS_STOP = 1;
  NRF_TIMER4->TASKS_CLEAR =1;

  NRF_TIMER4->PRESCALER = 0x09;
  NRF_TIMER4->BITMODE = 0x02;
  NRF_TIMER4->MODE = 0x00;
  NRF_TIMER4->INTENSET =0x40000;//setup for 
   printf("%lu INTENSET should be %lu  \t\n", NRF_TIMER4->INTENSET, 0x40);
   NRF_TIMER4->SHORTS =0x01;

  NRF_TIMER4->CC[tY] = 0x1E848;//Actual thing that is stopping x
  //printf("%d Setup  CC[2]\t %d\n", NRF_TIMER4->CC[tY], NRF_TIMER4->EVENTS_COMPARE[tY]);
  
 
 
  NVIC_EnableIRQ(TIMER4_IRQn);// when CC[ty] is reached flag interrupt
  NVIC_SetPriority(TIMER4_IRQn, 0);



  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;


  //Count every 4 seconds
  NRF_TIMER3 -> BITMODE = 0x02;
  NRF_TIMER3->MODE = 0x01;
  NRF_TIMER3->TASKS_START = 0x01;
  
  // 93750
  
  

}

uint32_t read_timer(){


  NRF_TIMER4->TASKS_CAPTURE[1] = 0x01;
  return NRF_TIMER4->CC[1];

    // fill in this function for reading the timer value on calling this function
}

//Functions intention is to trip when Pin# reaches a high level.
void LABHandler(void){
  
  printf("Starting conditions \nConfig[0] is %lu\n And INTENSET = %lu",NRF_GPIOTE->CONFIG[0], NRF_GPIOTE->INTENSET);
  

  // 0x00021C01 or 138241
  NRF_GPIOTE->CONFIG[0]=0x00021C01;//Event, Button0, onHiToLow

  //change event from button to checking GPIO at PIN18.
  NRF_GPIOTE->INTENSET = 1;
  NRF_GPIOTE->INTENSET = 1 <<1;

  NRF_GPIOTE->CONFIG[1] = 0x00031601;// Event Mode, Switch, onToggle
  

  NRF_GPIOTE->CONFIG[2] = 0x00011201;

  NVIC_EnableIRQ(GPIOTE_IRQn);//unsure how to track will return tomorrow. 
  //Second count prim in vs vode show some promise.
  NVIC_SetPriority(GPIOTE_IRQn,0);
  printf("Lab_Handled\n\n");
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

