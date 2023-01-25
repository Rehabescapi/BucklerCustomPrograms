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

//added to make blink work again
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"

#include "buckler.h"
#include "gpio.h"


//Lab3 Q1 Changed to uint32 from uint8 after rewatching lecture
//

static int x = 3;
static int* x_ptr = (int *) &x;
static uint32_t* outset = (uint32_t *) 0x50000508;
static uint32_t* outclear = (uint32_t *) 0x5000050C;
static uint32_t* CNF0 = (uint32_t * ) 0x50000700;
static uint32_t*  PIN  = (uint32_t*) 0x50000754;

static uint32_t *store = (uint32_t *) 0x4000600F;

static uint32_t  store2 = (uint32_t) 0x4000500C;

//0x50000000 + 0x700 + pin_num *4

//Recreating Blink P1
static uint8_t LEDS[3] = {BUCKLER_LED0, BUCKLER_LED1, BUCKLER_LED2};

/*
int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  

  //Blink setup
  if(!nrfx_gpiote_is_init())
  {
    error_code = nrfx_gpiote_init();
  }
  printf("Initial address PIN : %p \n Initial Address Store %p\n, Initial Address Store2 %p\n" ,PIN, store, store2);

  PIN = 5;
  *store = 5;
  store2 = 5;


  printf(" PIN is now equal %li at &PIN address %p \n *PIN is %p\n" ,&PIN, PIN, *PIN);
  printf("Store value %li or written also as %p\n *Store is %p\n", store, store, *store);

  uint32_t holdingVariable = 2;
  printf("Holding initially %li \n", holdingVariable);
   printf("Store2 value %li at &Store2 %p\n *Store2 is %p\n", store2, store2, (uint32_t*)store2);

  holdingVariable = *x_ptr;

  printf("%li \n", holdingVariable);
  //Q6 output
  printf("OutClear ");
  printf("%p\n",  outclear );
  printf("Out Set %p\n", outset);



  //Blinker Setup
  printf("LED0 :%p \n LED1 : %p \n LED2 : %p \n", LEDS[0], LEDS[1], LEDS[2]);
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
  error_code = nrfx_gpiote_out_init(LEDS[0], &out_config);
  APP_ERROR_CHECK(error_code);



  // loop forever
  while (1){

    nrf_gpio_pin_toggle(LEDS[0]);




    nrf_delay_ms(1000);
  }
}

*/