// printf app
//
// Use RTT to print messages via printf

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // loop forever
  uint32_t i = 0;
  printf(&i);

  float a = 0;
  int b = 0;
  char c = '0';

  printf("Float A : %p\n Int B = %p\n  Char C =%p\n", &a, &b, &c);
  while (1) {
    nrf_delay_ms(10000);
    printf("Iteration: %lu\n", i++);
  }
}

