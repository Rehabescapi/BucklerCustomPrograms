// Virtual timer implementation

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "nrf.h"

#include "virtual_timer.h"
#include "virtual_timer_linked_list.h"


static int id = 0;

// This is the interrupt handler that fires on a compare event
void TIMER4_IRQHandler(void) {
  // This should always be the first line of the interrupt handler!
  // It clears the event so that it doesn't happen again
  NRF_TIMER4->EVENTS_COMPARE[0] = 0;

  // Place your interrupt handler code here
  printf("interuption occured\n");
   
  // Update CC[0] register from the remaining timer values
  checkTimers();
}

void checkTimers(){

  node_t * base = list_remove_first();
  printf("base id : %lu \t" ,base->ID);
  node_t *temp;

  if(base != NULL){

    node_t * current = list_get_first();
    (* (base->cbFunc)) ();
    if(current!= NULL)
    {
      while(base->timer_value == current->timer_value)
      {
        printf("Matched times %lu and %lu \t ", base->ID , current->ID);
        (* (current->cbFunc))();

        temp = current;
        if(current->next != NULL){
          current = current->next;



        }
        
        temp->next = NULL;

        if(temp->repeat)
        {
          printf("updating Temp %lu \n", temp->ID);
          update(temp);

        }else
        {
          free(temp);
        }
      }
    }

    NRF_TIMER4->CC[0] = base->timer_value;

    if(base->repeat){
      printf("\n Updating value %lu \n", base->ID);
      update(base);

    }else{
      printf("\tFreeing %lu \n", base->ID);
      free(base);
    }


      list_print();
  }else{
    printf("empty lot\n");
  }

	// update CC[0] value by looking at the linked list
	// node after the current one, and update CC[0] using
	// the timer_value from this node

  /*
  1. Remove the first node and place at the end of the linked list after adding the current timer value. 
  Use read_timer() to get the current timer value. Use the linked list function that both inserts the 
  node and also sorts the list based on the timer value.
  2. Now get the first node of the updated list. This will be the next timer value to trigger the new COMPARE EVENT at.
  3. Check if the timer vallue in this node is already reached. 
  In that case, call the callback function. The callback function can be called using the following command: timer_node->cbFunc();
  */
}

void update(node_t* x ){
  uint32_t newCount = x->period + x->timer_value;
  x->timer_value=  newCount;

  x->ID = id++;
  list_insert_sorted(x);




}


// Read the current value of the timer counter
uint32_t read_timer(void) {
  NRF_TIMER4->TASKS_CAPTURE[1] = 0x01;
  return NRF_TIMER4->CC[1];
}

// Initialize TIMER4 as a free running timer
// 1) Set to be a 32 bit timer
// 2) Set to count at 1MHz
// 3) Enable the timer peripheral interrupt (look carefully at the INTENSET register!)
// 4) Clear the timer
// 5) Start the timer
void virtual_timer_init(void) {
  NRF_TIMER4->TASKS_STOP = 1;
  

  NRF_TIMER4->PRESCALER = 0x04;
  NRF_TIMER4->INTENSET = 0x10000;//Setup to prescaler for Compare 0
  NRF_TIMER4->BITMODE = 0x03;
  NRF_TIMER4->MODE = 0x0;

  NVIC_EnableIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 0);

  NRF_TIMER4->TASKS_CLEAR = 1;
  NRF_TIMER4->TASKS_START =0x01;



  // Place your timer initialization code here
 
}

// Start a timer. This function is called for both one-shot and repeated timers
// To start a timer:
// 1) Create a linked list node (This requires `malloc()`. Don't forget to free later)
// 2) Setup the linked list node with the correct information
//      - You will need to modify the `node_t` struct in "virtual_timer_linked_list.h"!
// 3) Place the node in the linked list
// 4) Setup the compare register so that the timer fires at the right time
// 5) Return a timer ID
//
// Your implementation will also have to take special precautions to make sure that
//  - You do not miss any timers
//  - You do not cause consistency issues in the linked list (hint: you may need the `__disable_irq()` and `__enable_irq()` functions).
//
// Follow the lab manual and start with simple cases first, building complexity and
// testing it over time.

static uint32_t timer_start(uint32_t microseconds, virtual_timer_callback_t cb, bool repeated) {

  //malloc
  struct node_t * link = (struct node_t*) malloc(sizeof(struct node_t));
  link->timer_value = microseconds;
  link->period = microseconds;
  link->cbFunc = cb;
  link->repeat = repeated;
  link->ID = id++;
  list_insert_sorted(link);


  //list_print();
  
  return 0;
}

// You do not need to modify this function
// Instead, implement timer_start
uint32_t virtual_timer_start(uint32_t microseconds, virtual_timer_callback_t cb) {
  
  return timer_start(microseconds, cb, false);
}

// You do not need to modify this function
// Instead, implement timer_start
uint32_t virtual_timer_start_repeated(uint32_t microseconds, virtual_timer_callback_t cb) {
  
  return timer_start(microseconds, cb, true);
}

// Remove a timer by ID.
// Make sure you don't cause linked list consistency issues!
// Do not forget to free removed timers.
void virtual_timer_cancel(uint32_t timer_id) {
}

