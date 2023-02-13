// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_gpio.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "software_interrupt.h"

#include "buckler.h"

void SWI1_EGU1_IRQHandler(void) {
    NRF_EGU1->EVENTS_TRIGGERED[0] = 0;
}

void GPIOTE_IRQHandler(void) {//Already written in part 
    NRF_GPIOTE->EVENTS_IN[0] = 0;

    gpio_set(25);
    nrf_delay_ms(300);
    gpio_clear(25);


    printf("Woo");
    
}

void GPIOTE_LABHandler(void){
  NRF_GPIOTE->CONFIG[0]=1;
  NRF_GPIOTE->CONFIG[0] = 3 << 16;
  //NRF_GPIOTE->CONFIG[0] = 1 << 17;//Sets Polarity to HiToLow
  // Sets to event mode
  NRF_GPIOTE->INTENSET = 1;
  NVIC_EnableIRQ(GPIOTE_IRQn);
  printf("Lab_Handled\n\n");
}

void setDevices(void){
    //Setting up Lights
  gpio_config(25, 1);
  gpio_config(22,0);
  gpio_config(28,0);
}

int main(void) {
  //NRF_GPIOTE_IRQHandler();
  ret_code_t error_code = NRF_SUCCESS;

  
  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

//Q5, Q6 configure a GPIOTE event to occur for a button press.
  GPIOTE_LABHandler();
  //setDevices();


  
  // loop forever
  while (1) {
    printf("Looping\n");
    nrf_delay_ms(1000);
    software_interrupt_generate();
  }
}

