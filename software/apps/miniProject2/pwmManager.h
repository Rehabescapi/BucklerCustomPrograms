#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "app_error.h"
#include "bsp.h"


#include "nrf_delay.h"
#include "app_pwm.h"




 


void pwm_ready_callback(uint32_t pwm_id);

void pwm_init();


void setTwoChannel(int);


/*
void GPIOTE_IRQHandler(void) {//Already written in part 
    

    if(NRF_GPIOTE->EVENTS_IN[0] == 1){
      NRF_GPIOTE->EVENTS_IN[0] = 0;

       printf("Button interrupt received\n\n");
    //nrf_delay_ms(100);
    

    }else if (NRF_GPIOTE->EVENTS_IN[1] == 1){
      NRF_GPIOTE->EVENTS_IN[1] =0;
      printf("switch interrupt received\n\n");
      


    }else if(NRF_GPIOTE->EVENTS_IN[2]==1){
        NRF_GPIOTE->EVENTS_IN[1]=0;
        NRF_TIMER3->TASKS_COUNT = 0x01;
    }
   
}
*/