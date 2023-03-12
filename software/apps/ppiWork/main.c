/** @file

 * @defgroup pin_change_int_example_main main.c

 * @{

 * @ingroup pin_change_int_example

 * @brief Pin Change Interrupt Example Application main file.

 *

 * This file contains the source code for a sample application using interrupts triggered by GPIO pins.

 *

 */



#include <stdbool.h>

#include "nrf.h"

#include "nrf_drv_gpiote.h"

#include "nrf_drv_ppi.h"

#include "nrf_drv_timer.h"

#include "nrf_delay.h"

#include "app_error.h"

#include "boards.h"



#include "nrf_log.h"

#include "nrf_log_ctrl.h"

#include "nrf_log_default_backends.h"



#define PIN_IN  28

#define COUNT_READ_INTERVAL 10 // Interval in ms to read the counting timer



static const nrf_drv_timer_t m_timer_count = NRF_DRV_TIMER_INSTANCE(1);



static const nrf_drv_timer_t m_timer_read = NRF_DRV_TIMER_INSTANCE(2);



nrf_ppi_channel_t ppi_channel_1, ppi_channel_2;



void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)

{

    //NRF_LOG_INFO("H");

}



void timer_handler_count(nrf_timer_event_t event_type, void * p_context)

{



}



void timer_handler_read(nrf_timer_event_t event_type, void * p_context)

{

    uint32_t count = nrf_drv_timer_capture_get(&m_timer_count, NRF_TIMER_CC_CHANNEL0);

//    if(count > 0)

//    {

        NRF_LOG_INFO("C: %d",count);

//    }

}



/**

 * @brief Function for configuring: PIN_IN pin for input sensing

 */

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



void timer_init()

{

    ret_code_t err_code;



    // Configure TIMER1 for counting of low to high events on GPIO

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;

    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;

    timer_cfg.mode = NRF_TIMER_MODE_LOW_POWER_COUNTER;

    err_code = nrf_drv_timer_init(&m_timer_count, &timer_cfg, timer_handler_count);

    APP_ERROR_CHECK(err_code);



    // Configure TIMER2 for reading the counter timer at a given interval COUNT_READ_INTERVAL

    timer_cfg.mode = NRF_TIMER_MODE_TIMER;

    err_code = nrf_drv_timer_init(&m_timer_read, &timer_cfg, timer_handler_read);

    APP_ERROR_CHECK(err_code);

    

    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer_read, COUNT_READ_INTERVAL);

    nrf_drv_timer_extended_compare(&m_timer_read,

                                   NRF_TIMER_CC_CHANNEL0,

                                   ticks,

                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,

                                   true);

    nrf_drv_timer_enable(&m_timer_read);

}



void ppi_init()

{

    ret_code_t err_code;



    err_code = nrf_drv_ppi_init();

    APP_ERROR_CHECK(err_code);

    

    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel_1);

    APP_ERROR_CHECK(err_code);

    

    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel_2);

    APP_ERROR_CHECK(err_code);

    

    uint32_t gpiote_evt_addr_1              = nrf_drv_gpiote_in_event_addr_get(PIN_IN);

    

    uint32_t timer_count_count_task_addr    = nrf_drv_timer_task_address_get(&m_timer_count, NRF_TIMER_TASK_COUNT);

    uint32_t timer_count_capture_task_addr  = nrf_drv_timer_task_address_get(&m_timer_count, NRF_TIMER_TASK_CAPTURE0);

    uint32_t timer_count_clear_task_addr    = nrf_drv_timer_task_address_get(&m_timer_count, NRF_TIMER_TASK_CLEAR);

    

    uint32_t timer_read_compare_event_addr  = nrf_drv_timer_event_address_get(&m_timer_read, NRF_TIMER_EVENT_COMPARE0);

    

    

    err_code = nrf_drv_ppi_channel_assign(ppi_channel_1, gpiote_evt_addr_1, timer_count_count_task_addr); // Trigger timer count task when GPIOTE pin go from low to high.

    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(ppi_channel_2, timer_read_compare_event_addr, timer_count_capture_task_addr); // Capture counter timer using PPI to make sure this is not delayed by BLE interrupt

    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_fork_assign(ppi_channel_2, timer_count_clear_task_addr); // Clear counter timer using PPI to make sure this is not delayed by BLE interrupt

    APP_ERROR_CHECK(err_code);

    

    err_code = nrf_drv_ppi_channel_enable(ppi_channel_1);

    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_enable(ppi_channel_2);

    APP_ERROR_CHECK(err_code);

}



/**

 * @brief Function for application main entry.

 */

int main(void)

{

    uint32_t err_code = NRF_LOG_INIT(NULL);

    APP_ERROR_CHECK(err_code);



    NRF_LOG_DEFAULT_BACKENDS_INIT();

    

    gpiote_init();

    timer_init();

    ppi_init();

    

    NRF_LOG_INFO("Example start\r\n");

    

    nrf_gpio_cfg_output(30);

    nrf_gpio_pin_clear(30);

    

    while (true)

    {

        while(NRF_LOG_PROCESS() == true);

        // Testing pin change (connect GPIO P0.30 to P0.28). This is not accurate, as log processing can take some time, but will show that example works.

        nrf_delay_us(100);

        nrf_gpio_pin_toggle(30);



    }

}

