#include <stdint.h>
#include <stdbool.h>

#include "app_error.h"

#include "nrf_delay.h"
#include "app_pwm.h"


static volatile bool ready_flag; 
static ret_code_t err_code;
APP_PWM_INSTANCE(PWM1,1);



APP_PWM_INSTANCE(PWM2, 0);





 void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    
    
    ready_flag = true;



}



void pwm_init()
{
    /* 2-channel PWM, 200Hz, output on Buckler LED pins. */

    app_pwm_config_t pwm_config =  APP_PWM_DEFAULT_CONFIG_2CH(5000, 25,23);
    



       pwm_config.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
    pwm_config.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_LOW;
    err_code = app_pwm_init(&PWM1,&pwm_config , pwm_ready_callback);
    APP_ERROR_CHECK(err_code);

    app_pwm_enable(&PWM1);       

    /* Switch the polarity of the second channel. */
    //app_pwm_channel_duty_ticks_set(&PWM1,1 ,10);

     app_pwm_config_t pwm_config2 =  APP_PWM_DEFAULT_CONFIG_1CH(5000, 17);
     err_code = app_pwm_init(&PWM2, &pwm_config2, pwm_ready_callback);
     APP_ERROR_CHECK(err_code);
    
    app_pwm_enable(&PWM2);
    while(app_pwm_channel_duty_set(&PWM2,1,50)== NRF_ERROR_BUSY);

}


void setTwoChannel( int value)
{
    while(app_pwm_channel_duty_set(&PWM1, 1, value) == NRF_ERROR_BUSY);
            
    while(app_pwm_channel_duty_set(&PWM1, 0, value) == NRF_ERROR_BUSY);
    //printf("%d\t", value);
         
}