/**
 * Copyright (c) 2014 - 2017, Nordic Semiconductor ASA/home/student/buckler3/software/apps/miniProject2/buckler.h
*
* @defgroup gpiote_example_main main.c
* @{
* @ingroup nrf_gpiote_example
* @brief GPIOTE Example Application main file.
*
* This file contains the source code for a sample application using GPIOTE.
*/

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "buckler.h"
//#define MY_BUTTON BUTTON_1
#define BUTTON 28
#define SECONDPIN 30
#define OUTPUT 25

static nrf_drv_timer_t timer = NRF_DRV_TIMER_INSTANCE(1);
static nrf_drv_timer_t timer2 = NRF_DRV_TIMER_INSTANCE(0);
//static uint32_t cap;
void timer_dummy_handler(nrf_timer_event_t event_type, void * p_context){}

void GPIOTE_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{

			nrf_gpio_pin_toggle(OUTPUT);
			uint32_t cap =	NRF_TIMER1->CC[0];
			NRF_LOG_INFO("Capture value: %d",cap);
	
	
}

static void timer_setup(void)
{
	ret_code_t err_code;
	nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
	err_code = nrf_drv_timer_init(&timer, &timer_cfg, timer_dummy_handler);
	APP_ERROR_CHECK(err_code);

	
	err_code = nrf_drv_timer_init(&timer2, &timer_cfg, timer_dummy_handler);
	APP_ERROR_CHECK(err_code);
	
}
	
static void gpiote_setup()
{
	ret_code_t err_code;
	nrf_gpio_cfg_output(OUTPUT);
	
	nrf_drv_gpiote_in_config_t i_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
	i_config.pull = NRF_GPIO_PIN_PULLUP;
	
	err_code = nrf_drv_gpiote_in_init(28 , &i_config, NULL); 						//configure BUTTON_1, event handler callback is set to NULL to keep it from triggering unecessary CB calls.
	APP_ERROR_CHECK(err_code);
	nrf_drv_gpiote_in_event_enable(28 , true);
	
	
	i_config.is_watcher = false;
	i_config.sense = NRF_GPIOTE_POLARITY_LOTOHI;
	i_config.pull = NRF_GPIO_PIN_NOPULL; 																			//Pullup is not really necessary as pullup is enabled on BUTTON_1.
	
	err_code = nrf_drv_gpiote_in_init(SECONDPIN, &i_config, GPIOTE_handler); 	//Configure the second pin. Event handler assigned to store value.
	APP_ERROR_CHECK(err_code);
	nrf_drv_gpiote_in_event_enable(SECONDPIN, true);
	
	
}

static void ppi_setup(void)
{
	ret_code_t err_code;
	nrf_ppi_channel_t ppi1, ppi2;
	err_code = nrf_drv_ppi_channel_alloc(&ppi1);
	APP_ERROR_CHECK(err_code);
	err_code = nrf_drv_ppi_channel_alloc(&ppi2);
	APP_ERROR_CHECK(err_code);


	
	
	err_code = nrf_drv_ppi_channel_assign(ppi1, nrf_drv_gpiote_in_event_addr_get(28), nrf_drv_timer_task_address_get(&timer, NRF_TIMER_TASK_CLEAR));
	APP_ERROR_CHECK(err_code);
	printf("err_code ppi1");
	

	//breaks at this point.
	err_code = nrf_drv_ppi_channel_assign(ppi2, nrf_drv_gpiote_in_event_addr_get(SECONDPIN), nrf_drv_timer_capture_task_address_get(&timer2, NRF_TIMER_TASK_CAPTURE0));
	APP_ERROR_CHECK(err_code);
	printf('ppi2');
	

	nrf_drv_ppi_channel_enable(ppi1);
	nrf_drv_ppi_channel_enable(ppi2);
}

static void log_init(void)
{
	ret_code_t err_code;
	err_code = NRF_LOG_INIT(NULL);
	APP_ERROR_CHECK(err_code);
	
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
	log_init();
	
	ret_code_t err_code;
	err_code = nrf_drv_gpiote_init();
	APP_ERROR_CHECK(err_code);
	
	
	timer_setup();
	printf("timer setup\n");
	gpiote_setup();
	printf("gpiote setup \n");
	ppi_setup();
	printf("Ppi complete");
	
	nrf_drv_timer_enable(&timer);

    while (true)
    {

			// Do Nothing - GPIO can be toggled without software intervention.
    }
}


/** @} */
