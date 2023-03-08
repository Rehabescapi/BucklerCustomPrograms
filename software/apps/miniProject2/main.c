
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


//bookKeeping files
#include "pwmManager.h"

// Create the instance "PWM1" using TIMER1.


int main(void)
{
    pwm_init();

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
             nrf_delay_ms(50);
        }

    }

}

