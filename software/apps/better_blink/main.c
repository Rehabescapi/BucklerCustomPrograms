// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

// LED array
static uint8_t LEDS[3] = {BUCKLER_LED0, BUCKLER_LED1, BUCKLER_LED2};

 static int Addresses [3] = {BUCKLER_LED0, BUCKLER_LED2, BUCKLER_LED2};
//printf("%p\n \n", (void*)&Addresses[0]);

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

   int nums [3] = {1, 2 ,3};
   printf("Memory Addresses of 1 : %p\n 2: %p\n 3: %p\n", &nums[0], &nums[1], &nums[2]);

   int nums2 [3] = {1, 2 ,3};
   printf("Memory Addresses P2 of 1 : %p\n 2: %p\n 3: %p\n", &nums2[0], &nums2[1], &nums2[2]);


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


  int j = 0;
  nrf_gpio_pin_toggle(LEDS[0]);
  // loop forever
  while (1) {

    j++; 

    if( j% 4 ==0 )
    {
      nrf_gpio_pin_toggle(LEDS[0]);
      nrf_gpio_pin_set(LEDS[1]);
      nrf_gpio_pin_set(LEDS[2]);



    }



    if( j% 16 ==0 )
    {
      nrf_gpio_pin_toggle(LEDS[1]);
    }


    if( j% 32 == 0)
    {
      nrf_gpio_pin_toggle(LEDS[2]);
      j=0;
    }

    nrf_delay_ms(100);
    
  }
}

