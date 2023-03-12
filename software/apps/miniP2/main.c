/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Sample app to demonstrate PWM.
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/gpio.h>

#define TST_CLK_PIN 17 //this is led 0 pin on nrf52-dk
#define FREQ_MEASURE_PIN 11

void gpio_clock_8m(uint32_t pin_number)
{
    
    NRF_TIMER0->PRESCALER = 0; // 16MHz
    NRF_TIMER0->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk;
    NRF_TIMER0->CC[0] = 3; //1 for 16/2=8MHz for, 2 for 8/2=4MHz, 4 for 4/2=2MHz etc..
    NRF_TIMER0->BITMODE = (TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos); 
  //using config [1] since [0]
    NRF_GPIOTE->CONFIG[1] = GPIOTE_CONFIG_MODE_Task | (pin_number << GPIOTE_CONFIG_PSEL_Pos) |
                            (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos);

    /*Connect TIMER event to GPIOTE out task*/
    NRF_PPI->CHEN |= 1 << 3;
    *(&(NRF_PPI->CH3_EEP)) = (uint32_t) &NRF_TIMER0->EVENTS_COMPARE[0];
    *(&(NRF_PPI->CH3_TEP)) = (uint32_t) &NRF_GPIOTE->TASKS_OUT[1];
    NRF_PPI->CHENSET |= 1 << 3;

    /*Starts clock signal*/
    NRF_TIMER0->TASKS_START = 1;
}

static void timer_init() //gate timer
{
  NRF_TIMER1->TASKS_STOP = 1; //trigger stop
  NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer; //mode timer
  NRF_TIMER1->PRESCALER = 8;  // Fhck / 2^8 
  //total gate time of timer 62500 - 1s
  NRF_TIMER1->CC[0] = 62501;  //end gate count
  NRF_TIMER1->CC[1] = 1;  //start gate count. don't start at 0 to no start on clear.
  
  NRF_TIMER1->BITMODE = (TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos); 
  
  NRF_TIMER1->TASKS_CLEAR = 1; //trigger zero timer
  NRF_TIMER1->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos); //interrupt on end gate count
  
  NRF_TIMER1->EVENTS_COMPARE[0] = 0; //rest event flag for gate end
  NRF_TIMER1->EVENTS_COMPARE[1] = 0; //rest event flag for gate start
}

static void counter_init() //actual counter
{
  NRF_TIMER2->TASKS_STOP = 1; //trigger stop
  NRF_TIMER2->MODE = TIMER_MODE_MODE_Counter; //counter
  NRF_TIMER2->BITMODE = (TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos);
  NRF_TIMER2->TASKS_CLEAR = 1; //trigger zero counter
  NRF_TIMER2->EVENTS_COMPARE[0] = 0; //reset event for compare
}

//counter input pin setting
static void gpiote_init(uint32_t pin)
{
  NRF_GPIOTE->CONFIG[0]   =   0x01 << 0;                // MODE: Event
  NRF_GPIOTE->CONFIG[0]   |=  pin << 8;               // Pin number
  NRF_GPIOTE->CONFIG[0]   |=  GPIOTE_CONFIG_POLARITY_LoToHi << 16;    // Event rising edge  
}

//hardware trigger: stop of counter on event of end timer1 gate
static void ppi_timer_stop_counter_init()
{
  NRF_PPI->CHEN |= 1 << 2; //channel
  *(&(NRF_PPI->CH2_EEP)) = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[0]; //attach end gate event
  *(&(NRF_PPI->CH2_TEP)) = (uint32_t)&NRF_TIMER2->TASKS_STOP; //to counter stop trigger
  NRF_PPI->CHENSET |= 1 << 2; //set channel
}

//hardware trigger: start of counter on event of begin timer1 (gate) event.gate
static void ppi_timer_start_counter_init()
{
  NRF_PPI->CHEN |= 1 << 4;// channel
  *(&(NRF_PPI->CH4_EEP)) = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[1]; //attach gate start event
  *(&(NRF_PPI->CH4_TEP)) = (uint32_t)&NRF_TIMER2->TASKS_START; //to counter start trigger
  NRF_PPI->CHENSET |= 1 << 4;
}


static void ppi_gpiote_counter_init()
//hardware attachment for the gpiote that was selected above
{
  NRF_PPI->CHEN |= 1 << 1;
  *(&(NRF_PPI->CH1_EEP)) = (uint32_t)&NRF_GPIOTE->EVENTS_IN[0]; //attach pin change
  *(&(NRF_PPI->CH1_TEP)) = (uint32_t)&NRF_TIMER2->TASKS_COUNT;//to one count
  NRF_PPI->CHENSET |= 1 << 1;
}

//gate end interrupt handler. Count is done at hardware and percice interrupt execution isnt critical
void TIMER1_IRQHandler(void) 
{
  if (NRF_TIMER1->EVENTS_COMPARE[0] != 0) //end gate event
  {
    NRF_TIMER1->EVENTS_COMPARE[0] = 0; //reset end gate flag
    NRF_TIMER1->EVENTS_COMPARE[1] = 0; //reset start gate flag
    NRF_TIMER2->TASKS_CAPTURE[0] = 1; //trigger get counter value
        
    printk("cc: %dHz\n", NRF_TIMER2->CC[0]); //report to serial console
        
    NRF_TIMER1->TASKS_CLEAR = 1; //reset timer
    NRF_TIMER2->TASKS_CLEAR = 1; //reset counter
            
    NRF_TIMER1->TASKS_START = 1; //start next count gate    
    }
}

void main(void)
{
  NVIC_EnableIRQ(TIMER1_IRQn); //enable timer1 interrupt
  IRQ_DIRECT_CONNECT(TIMER1_IRQn, 3, TIMER1_IRQHandler, 0); //link interrupt flag to handler
    NVIC_SetPriority(TIMER1_IRQn, 3); //set interrupt execution priority

  struct device *dev;//port container for gpio management
  dev = device_get_binding("GPIO_0"); //on nrf52805 all GPIOs are on port0
  /**/
  gpio_pin_configure(dev, FREQ_MEASURE_PIN, GPIO_INPUT|GPIO_ACTIVE_HIGH|GPIO_PULL_DOWN); //p0.11 freq in

  //init freq count hardware
  counter_init();
  timer_init();
  gpiote_init(FREQ_MEASURE_PIN);
  ppi_gpiote_counter_init();
  ppi_timer_stop_counter_init();
  ppi_timer_start_counter_init();

  //test clock hardware
  gpio_clock_8m(TST_CLK_PIN);

  //start freq count task
  NRF_TIMER1->TASKS_START = 1;
  while (1) {
        //manage loop
  }
}