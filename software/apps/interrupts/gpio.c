#include "gpio.h"

static uint32_t* OUTSET = (uint32_t*) 0x50000508;
static uint32_t* OUTCLEAR = (uint32_t*) 0x5000050C;
static uint32_t* IN = (uint32_t*) 0x50000510;
 
// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
    uint32_t* pinStart = (uint32_t*) 0x50000700;
    uint32_t *tempPTR = pinStart + (uint8_t)1 *gpio_num;//Could be updated but it works.
    *tempPTR = dir;
}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
    *OUTSET =  1 << gpio_num;

}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
    *OUTCLEAR = 1 << gpio_num;

}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    
    // should return pin state
    return (*IN >> gpio_num) &1;
}

void GPIOTE_IRQHandler(void) {//Already written in part 
    NRF_GPIOTE->EVENTS_IN[0] = 0;
    printf("Butttton interrupt received\n\n");
    //nrf_delay_ms(100);
    gpio_clear(25);
    nrf_delay_ms(2000);
    gpio_set(25);


    
    
}