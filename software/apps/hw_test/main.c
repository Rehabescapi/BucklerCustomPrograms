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

typedef struct {
  uint32_t OUT;
  uint32_t OUTSET;
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t DIRSET;
  uint32_t DIRCLR;
  uint32_t LATCH;
  uint32_t DETECTMODE;
  uint32_t PADDING[118]; // This is nothing, just to move PIN_CNF to the right address
  uint32_t PIN_CNF[32];
} gpioPins;

static gpioPins* pins = (gpioPins*)0x50000504;

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  gpio_direction_t dir = INPUT;
  gpio_config(28, dir);
  gpio_config(22, dir);

  //*dirSet |= mask;
  uint32_t* buttonAddr = (uint32_t*)&pins->PIN_CNF[28];
  //*buttonAddr = (uint32_t)8;
  printf("%p\n", buttonAddr);

  // loop forever
  while (1) {
    // Link button to led 0
    if (gpio_read(28)) {
      gpio_clear(23);
      gpio_clear(24);
      gpio_clear(25);
    } else {
      gpio_set(23);
      gpio_set(24);
      gpio_set(25);
    }

    // Link switch to led 1
    if (gpio_read(22) == true) {
      // Make button pull up
      *buttonAddr = (uint32_t)12;
    } else {
      // Make button pull down
      *buttonAddr = (uint32_t)8;
    }
    printf("%x\n", *buttonAddr);

    nrf_delay_ms(100);
  }
}
