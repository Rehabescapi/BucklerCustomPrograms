#include <stdbool.h>
#include "nrf.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"
#include "boards.h"
#include "nrf_drv_timer.h"

#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_util_platform.h"


#define PIN_IN 28 //BUTTON_1 

#define PIN_OUT 25

const nrf_drv_timer_t TIMER = NRF_DRV_TIMER_INSTANCE(1);
uint8_t state = 0, state_prev=0;
uint8_t ready = 0;
uint16_t captured_value,captured_prev = 0;
float frequency = 0.0;

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    nrf_drv_gpiote_out_toggle (25);
}

void timer_event_handler(nrf_timer_event_t event_type, void* p_context)
{
 //Never launch, do nothing
}

/**
 * @brief Function for configuring: input PIN, output PIN,
 * and configures GPIOTE to give an interrupt on pin change.
 */
static void gpio_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    
    /* Configure output pin */
    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_SIMPLE(false);
    
    err_code = nrf_drv_gpiote_out_init(PIN_OUT, &out_config);
    APP_ERROR_CHECK(err_code);

    
    /**************************************/

    /* Configure input for capture */
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(PIN_IN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(PIN_IN, true);
    /**************************************/
}

static void timer_init (void){
    
    ret_code_t err_code;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init (&TIMER, &timer_cfg, timer_event_handler);
    APP_ERROR_CHECK(err_code);
}

int main(void)
{
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("HelloWorld");

    /*Macro for processing all log entries from the buffer.
    It blocks until all buffered entries are processed by the backend.*/
    NRF_LOG_FLUSH();
        
    timer_init();
    gpio_init();
    
    nrf_drv_timer_enable(&TIMER);
    
    while (true)
    {
        //Nothing to do as it's the first test
    }
}