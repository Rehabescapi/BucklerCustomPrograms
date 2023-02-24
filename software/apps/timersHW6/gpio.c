#include "gpio.h"


// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - INPUT =0, OUTPUT =1
void gpio_config(uint8_t pin_numb, bool mode){
    printf("Requested from Pin #: %d \n", pin_numb);
    uint32_t address = 0x50000000 + 0x700 + 4 * pin_numb;  // loading PIN_CNF[x] register for pin x
    uint32_t *ptr_cnf = (uint32_t *) address;//0x50000000 + 0x00000700;// + 4 * pin_numb;   
    // printf("Address being written at is: %x \n", ptr_cnf);
    *ptr_cnf = mode; // a single digit value automatically goes to DIR bit
}

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t pin_numb)
{
  // printf("Setting Pin #: %d \n", pin_numb);
  uint32_t address = 0x50000000 + 0x508; // loading OUTSET register first
  uint32_t *ptr_OUTSET = (uint32_t *) address;//0x50000000 + 0x00000700;// + 4 * pin_numb; 
  // printf("Address being written at is: %x \n", ptr_OUTSET);
  *ptr_OUTSET = 1<<pin_numb; // a single digit value automatically goes to DIR bit      
}

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t pin_numb){
  // printf("Clearing Pin #: %d \n", pin_numb);
  uint32_t address = 0x50000000 + 0x50C; // loading OUTCLR register first
  uint32_t *ptr_OUTCLR = (uint32_t *) address;//0x50000000 + 0x00000700;// + 4 * pin_numb; 
  // printf("Address being written at is: %x \n", ptr_OUTCLR);
  *ptr_OUTCLR = 1<<pin_numb; // a single digit value automatically goes to DIR bit      
}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    uint32_t address = 0x50000000 + 0x510; // loading IN register first
    uint32_t *ptr_IN = (uint32_t *) address;//0x50000000 + 0x00000700;// + 4 * pin_numb; 
    
    bool val = (*(ptr_IN)&(1<<gpio_num));
    
    // printf("%d is read from: %x \n", val, ptr_IN);
    // should return pin state
    return val;
}

void gpio_OUT(uint8_t pin_numb, int val){
    printf("Using OUT Register to write: %d on pin: %d \n", val, pin_numb); 
    uint32_t address = 0x50000000 + 0x700 + 0x504; // loading OUT register
    uint32_t *ptr = (uint32_t *) address; 
    printf("Address being written at is: %x \n", 0xFFFFFFFF||val<<pin_numb);
    *ptr &=val<<pin_numb;  // Read the contents of the register and clear a necessary bit
}


void setDevices(){
  /*  ret_code_t error_code = NRF_SUCCESS;

   initialize RTT library
  //error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  */
  printf("Log initialized!\n");
  

  //Adding Light struct
  
  volatile pins* const pinD = (pins*) 0x50000700;

  volatile gpio* const GPIO = (gpio*) 0x50000504;



  //Setting up Lights
  /*gpio_config(25, 1);
  gpio_config(24, 1);
  gpio_config(23, 1);

  //Setting up Buttons
  gpio_config(22, 0);*/
  gpio_config(28, 0);

 /*
  gpio_clear(25);
  gpio_clear(23);
  gpio_clear(24);
  */

}



bool getInput(int x){
    //todo Make this work for more things. 
    return gpio_read(x);
}






void LABHandler(void){
  
   

  // 0x00021C01 or 138241
  NRF_GPIOTE->CONFIG[0]=0x00021C01;// mode is set to event mode.
  NRF_GPIOTE->INTENSET = 1;
  NVIC_EnableIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn,0);

  //NRF_TIMER4->CONFIG[0] = 0x00021C01;
  NRF_TIMER4->INTENSET =0x30;
  NVIC_EnableIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 1);
}

