
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

#include "nrf_drv_gpiote.h"


//bookKeeping files
#include "pwmManager.h"

// Create the instance "PWM1" using TIMER1.
static int FourSecondCount = 125000;
static int tX = 1, tY= 2;

uint32_t read_counter(){
    

  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;
  printf("%d count" , NRF_TIMER3->CC[1]);
  return NRF_TIMER3->CC[1];

  // fill in this function for reading the timer value on calling this function
}

static void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
  printf("%d and polarity %d", pin, action);
  printf("woots");
    //Callback function when the interrupt is triggered
}

static void gpiote_init()
{
    //VCE: This block is a one time configuration
    ret_code_t err_code;
    if(!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }
    
    //VCE: The below block needs to be called for each pin
    nrf_drv_gpiote_in_config_t in_config_1;
    in_config_1.pull = NRF_GPIO_PIN_NOPULL; //User defined
    in_config_1.sense = NRF_GPIOTE_POLARITY_LOTOHI; //User defined
    in_config_1.hi_accuracy = true; //User defined
    in_config_1.is_watcher = false; //Don't change this
    in_config_1.skip_gpio_setup = false; //Don't change this

    //VCE: Configuring 
    err_code = nrf_drv_gpiote_in_init(14, &in_config_1, in_pin_handler);
    APP_ERROR_CHECK(err_code);
    printf("got to here");

    nrf_drv_gpiote_in_event_enable(14, true);
    
}

void TIMER4_IRQHandler(void){
  NRF_TIMER4->EVENTS_COMPARE[tY]= 0;
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;
  
  NRF_TIMER3->TASKS_COUNT = 0x01;
  printf("3 Seconds and triggered an interupt!!! %d\n", read_counter());  
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






int main(void)
{

    gpiote_init();
    pwm_init();
    timer_init();
    


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

