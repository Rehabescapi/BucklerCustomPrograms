
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_error.h"
#include "bsp.h"
#include "nrf_delay.h"
#include "app_pwm.h"

// Create the instance "PWM1" using TIMER1.
APP_PWM_INSTANCE(PWM1,1); 
static volatile bool ready_flag; 

 void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    
    
    ready_flag = true;


}


int main(void)
{
    ret_code_t err_code;
    

    /* 2-channel PWM, 200Hz, output on Buckler LED pins. */

    app_pwm_config_t pwm_config =  APP_PWM_DEFAULT_CONFIG_2CH(5000, 23,25);
    //app_pwm_config_t pwm_config =  APP_PWM_DEFAULT_CONFIG_1CH(5000, 22);

    pwm_config.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
    pwm_config.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_LOW;

    printf("got to line 31\n");

    /* Switch the polarity of the second channel. */
    //app_pwm_channel_duty_ticks_set(&PWM1,1 ,10);


    err_code = app_pwm_init(&PWM1,&pwm_config , pwm_ready_callback);


    APP_ERROR_CHECK(err_code);
    printf("got to line 40\n");
    app_pwm_enable(&PWM1);

    while(app_pwm_channel_duty_set(&PWM1,1,50)== NRF_ERROR_BUSY);

    
    
    uint32_t value = 0;
    printf("start of while loop");
    while (true)
    {
        
        printf("%d\t", app_pwm_busy_check(&PWM1));
        

        /* Your code for modifying the duty cycle value */
        for (value = 0; value <= 100; value++)
        {
             err_code = app_pwm_channel_duty_set(&PWM1, 0, value);
            APP_ERROR_CHECK(err_code);
            err_code = app_pwm_channel_duty_set(&PWM1, 1, value);
            APP_ERROR_CHECK(err_code);
            
            // for catching any errors returned
             nrf_delay_ms(50);
        }

    }

}
