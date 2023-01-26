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


#include "buckler.h"
#include "gpio.h"


//Lab3 Q1 Changed to uint32 from uint8 after rewatching lecture
//

static uint32_t* OUTSET = (uint32_t*) 0x50000508;
static uint32_t* OUTCLEAR = (uint32_t*) 0x5000050C;
static uint32_t* CNF0 = (uint32_t*) 0x5000070C;

static uint32_t* IN = (uint32_t*) 0x50000510;
static uint32_t* DIR = (uint32_t*) 0x5000514;

static uint32_t* PIN0 = (uint32_t* ) 0x50000700;

static uint32_t*  PIN23 = (uint32_t*) 0x5000075C;
static uint32_t*  PIN24 = (uint32_t*) 0x50000760;
static uint32_t*  PIN25 = (uint32_t*) 0x50000764;


static uint32_t* PIN22 = (uint32_t*) 0x50000758; // Switch0 
static uint32_t* PIN28 = (uint32_t*) 0x50000770;



//static uint32_t* Button0
//static unit32_t* SWITCH0


//0x50000000 + 0x700 + pin_num *4

//Recreating Blink P1
/*
typedef struct 
{
   uint32_t  var_OUT;
   uint32_t var_IN;
   uint32_t var_DIR; 
   uint32_t arr[32];
    
}gpio_t;
*/

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  

  //Adding Light struct
  struct gpio *hardware;

  //hardware = (struct gpio*)malloc(sizeof(struct gpio));
 
  hardware->var_OUTSET = OUTSET;
  hardware->var_OUTCLEAR = OUTCLEAR;
  hardware->var_IN = IN;
  hardware->arr[0]= PIN0;//0x50000700
  for(int i =1;  i< 32; i++)
  {
    uint32_t *x = PIN0 + sizeof(uint8_t)*i;

    //printf("%p \n\n", x);
    hardware->arr[i] = (uint32_t*) x;
    printf("%p \n\n",hardware->arr[i]);
  }
  
  



  printf("%p for initial point of arr at first element\n\n", hardware->arr[0]);
  //printf("%p  should end in 758 if you went to arr 22.\n\n" ,hardware->arr[22]);

  hardware->arr[23] =1;// Blue *PIN23
  //*PIN24 = 1; // Yellow
  *PIN25 =1; // Red

  *PIN22 = 0;
  *PIN22 = 1 <<1;
  *PIN28 =0;
  *PIN28 =1 <<1;

  *OUTCLEAR = 1 <<28;
  *OUTCLEAR = 1 <<22;


  //Setting Pins as inputs
  *IN = 1 <<28;
  *IN = 1 <<22;

  *OUTCLEAR =  1 << 25; 
  printf("Turning on 25.");
  nrf_delay_ms(3000);
  *OUTSET = 1 << 25;
  printf("Turning off Blue\n");


  bool temp = true;

  // loop forever
  while (1){
    //printf("Starting loop\n");

    
   
    if(temp)
    {
      //printf("toggle back on\n");
      temp = !temp;
      *OUTCLEAR = 1 <<23;
      *OUTSET = 1 <<25;

      
      

    }
    else
    {
      temp = true;
      
      *OUTSET =  1 << 23; 
      *OUTCLEAR =1 <<25;
    }




    nrf_delay_ms(1000);
  }
}

