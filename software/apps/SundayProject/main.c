#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#define LED 25
#define BTN 28
// Event handler for the GPIOTE event for toggling LED
void button_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
    if(action == NRF_GPIOTE_POLARITY_TOGGLE  && pin == BTN){
        nrf_gpio_pin_toggle(LED);
    }
}

int main(void){
    // Configure LED GPIO
    nrf_gpio_cfg_output(LED);
    nrf_gpio_pin_set(LED);
    
    //Initialize GPIOTE driver
    nrf_drv_gpiote_init();
    
    //Configure button with pullup and event on both high and low transition
    nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    config.pull = NRF_GPIO_PIN_PULLUP;
    nrf_drv_gpiote_in_init(BTN, &config, button_event_handler); //Assign button config to a GPIOTE channel
                                                                //and assigning the interrupt handler
    nrf_drv_gpiote_in_event_enable(BTN, true);                  //Enable event and interrupt
    
    for(;;)__WFE(); //CPU sleep w
}