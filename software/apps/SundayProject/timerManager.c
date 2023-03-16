#include <stdint.h>
static int FourSecondCount = 125000;
static int tY= 2;

#include "timerManager.h"


//Interrupts flow every second.
void TIMER4_IRQHandler(void){
  NRF_TIMER4->EVENTS_COMPARE[tY]= 0;
  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;
  


  
  reportCount();
  //printf("\n Counter = %lu\n\n", read_counter());  
}



static float average = 0;
static int count = 0;



void reportCount(){
  NRF_TIMER3->TASKS_CAPTURE[1] = 0x01;

    count++;
    uint32_t time = NRF_TIMER3->CC[1];
    NRF_TIMER3->TASKS_CLEAR =0x01;
    NRF_TIMER3->TASKS_START =0x01;
    average = ((average * (count -1)) + time )/ (count );
    
    
    printf("\n Count %d, Time %d,  average%f,\n", count, time, average);
    float xS = 1.0/ time;
    float xMs = xS* 1000000;
    printf("1 / average is around %.4f seconds or %.2e microseconds", xS, xMs );



}



void timer_init(){
  // fill in this function to initialize a timer of your choice
  
  NRF_TIMER4->TASKS_STOP = 1;
  NRF_TIMER4->TASKS_CLEAR =1;

  NRF_TIMER4->PRESCALER = 0x09;
  NRF_TIMER4->BITMODE = 0x02;
  NRF_TIMER4->MODE = 0x00;
  NRF_TIMER4->INTENSET =0x40000;//setup for 
   
   NRF_TIMER4->SHORTS =0x01;

  NRF_TIMER4->CC[tY] = 0x7A12;//Actual thing that is stopping x

  NVIC_EnableIRQ(TIMER4_IRQn);// when CC[ty] is reached flag interrupt
  NVIC_SetPriority(TIMER4_IRQn, 0);

  NRF_TIMER4->TASKS_CLEAR = 0x01;
  NRF_TIMER4->TASKS_START = 0x01;


  //Count every 4 seconds
  NRF_TIMER3 -> BITMODE = 0x02;
  NRF_TIMER3->MODE = 0x01;
  NRF_TIMER3->TASKS_START = 0x01;


  NRF_TIMER2 -> BITMODE = 0x02;
  NRF_TIMER2->MODE = 0x01;
  NRF_TIMER2->TASKS_START = 0x01;
  
}

uint32_t read_timer(){


  NRF_TIMER4->TASKS_CAPTURE[1] = 0x01;
  return NRF_TIMER4->CC[1];

    // fill in this function for reading the timer value on calling this function
}

