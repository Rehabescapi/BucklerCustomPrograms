
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
    //app_pwm_config_t pwm_config =  APP_PWM_DEFAULT_CONFIG_2CH(5000, 22,23);
    app_pwm_config_t pwm_config =  APP_PWM_DEFAULT_CONFIG_1CH(5000, 22);

    ///pwm_config.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    

    /* Switch the polarity of the second channel. */
    //app_pwm_channel_duty_ticks_set(&PWM1,1 ,10);

    /* Initialize PWM. */


    err_code = app_pwm_init(&PWM1,&pwm_config , pwm_ready_callback);

    APP_ERROR_CHECK(err_code);
    /* Enable PWM. */
    app_pwm_enable(&PWM1);
    uint32_t value = 0;

    while (true)
    {
        if(value >= 10000)
        {
            value = 0;
        }

        /* Your code for modifying the duty cycle value */
        if(app_pwm_busy_check(&PWM1)){
            printf("chaaannnnnnnging value\n");
            app_pwm_channel_duty_set(&PWM1,1,(value++/100));



        }
            
            // for catching any errors returned
            APP_ERROR_CHECK(err_code);
            nrf_delay_ms(25);
        
    }

}
