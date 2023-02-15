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
    printf("software_interrupt duu duu\n");
    gpio_clear(23);
    nrf_delay_ms(2000);
    gpio_set(23);
}

void GPIOTE_IRQHandler(void) {//Already written in part 
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    printf("Butttton interrupt received\n\n");
    //nrf_delay_ms(100);
    gpio_clear(25);
    nrf_delay_ms(2000);
    gpio_set(25);


    
    
}

void LABHandler(void){
  
  printf("Starting conditions \nConfig[0] is %p\n And INTENSET = %p",NRF_GPIOTE->CONFIG[0], NRF_GPIOTE->INTENSET);
  

  // 0x00021C01 or 138241
  NRF_GPIOTE->CONFIG[0]=0x00021C01;// mode is set to event mode.
  NRF_GPIOTE->INTENSET = 1;
  printf("Config[0] is %p\n And INTENSET = %p",NRF_GPIOTE->CONFIG[0], NRF_GPIOTE->INTENSET);
  NVIC_EnableIRQ(GPIOTE_IRQn);

  printf("Lab_Handled\n\n");
}

void setDevices(void){
    //Setting up Lights
  gpio_config(25, 1);
  gpio_config(23,1);
  gpio_set(25);
  gpio_set(23);
}

int main(void) {
  //NRF_GPIOTE_IRQHandler();
  ret_code_t error_code = NRF_SUCCESS;

  
  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  software_interrupt_init();

//Q5, Q6 configure a GPIOTE event to occur for a button press.
  LABHandler();
  setDevices();


  //Q7, Q8

    NVIC_SetPriority(SWI1_EGU1_IRQn, 4);
    NVIC_SetPriority(GPIOTE_IRQn,0);

    

  // loop forever
  while (1) {
    printf("Looping\n");
    //software_interrupt_generate();
    
    __WFI();
    nrf_delay_ms(1000);

  }
}

