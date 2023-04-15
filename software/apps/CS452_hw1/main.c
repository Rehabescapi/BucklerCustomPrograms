
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"

#include "buckler.h"


#define LED_1 23
static uint8_t LEDS[3] = {BUCKLER_LED0, BUCKLER_LED1, BUCKLER_LED2};

int main(void) {
    //bsp_board_init(LEDS);
    ret_code_t error_code = NRF_SUCCESS;

      // initialize GPIO driver
      if (!nrfx_gpiote_is_init()) {
        error_code = nrfx_gpiote_init();
      }
      APP_ERROR_CHECK(error_code);

      // configure leds
      // manually-controlled (simple) output, initially set
      nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
      for (int i=0; i<3; i++) {
    error_code = nrfx_gpiote_out_init(LEDS[i], &out_config);
    APP_ERROR_CHECK(error_code);
  }
     

    while (true) {
        nrf_gpio_pin_toggle(LED_1);
        nrf_delay_ms(500);
    }
}
