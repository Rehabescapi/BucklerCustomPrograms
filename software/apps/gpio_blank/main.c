// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "gpio.h"


static uint8_t* outset = (uint8_t *) 0x50000508;
static uint8_t* outclear = (uint8_t *) 0x5000050C;
static uint8_t* CNF0 = (uint8_t * ) 0x50000700;
static uint8_t*  PIN  = (uint8_t*) 0x50000754;
int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  printf("OutClear ");
  printf("%p\n",  outclear );
  printf("Out Set %p\n", outset);





  // loop forever
  while (1){

    nrf_gpio_pin_toggle(outclear);

    nrf_delay_ms(1000);
  }
}

