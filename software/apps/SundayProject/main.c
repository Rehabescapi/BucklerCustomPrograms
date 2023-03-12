#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "buckler.h"
#include "nrfx_gpiote.h"
#include "pwmManager.h"
#include "nrf_timer.h"

#include "gpio.h"
#include "pwmManager.h"
#include "timerManager.h"


static ret_code_t err_code;
#define LED 24
#define BTN 28


#define PIN_OUT 17
#define PIN_IN 14


// Event handler for the GPIOTE event for toggling LED
void button_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    if(action == NRF_GPIOTE_POLARITY_TOGGLE  && pin == BTN){
        nrf_gpio_pin_toggle(LED);
         NRF_TIMER3->TASKS_COUNT = 0x01;
        
    }
    if(action == NRF_GPIOTE_POLARITY_LOTOHI && pin == BTN){
        
        
    }
}


void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
   NRF_TIMER3->TASKS_COUNT = 0x01;
}

int main(void){


    // Configure LED GPIO
    nrf_gpio_cfg_output(LED);
    nrf_gpio_pin_set(LED);
    
    //Initialize GPIOTE driver
    nrf_drv_gpiote_init();


    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);
    
    err_code = nrf_drv_gpiote_out_init(PIN_OUT, &out_config);
    APP_ERROR_CHECK(err_code);



    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(PIN_IN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(PIN_IN, true);


    
    timer_init();
    pwm_init();

    



    printf("Got to button here");


    //Configure button with pullup and event on both high and low transition
    nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    config.pull = NRF_GPIO_PIN_PULLUP;
     nrf_drv_gpiote_in_init(BTN, &config, button_event_handler); //Assign button config to a GPIOTE channel
 
    //Assign button config to a GPIOTE channel
                                                               //and assigning the interrupt handler
    nrf_drv_gpiote_in_event_enable(BTN, true);                  //Enable event and interrupt
    

    uint32_t value = 0;

    while(true){


        for (value = 0; value <= 100; value++)
        {
           setTwoChannel( value);
            
            // for catching any errors returned
             nrf_delay_ms(100);
        }



    }
}