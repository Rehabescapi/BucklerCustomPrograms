#include<stdio.h>

#include "app_error.h"
#include "app_timer.h"/**/
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_drv_spi.h"
// #include "nordic_common.h"
#include "buckler.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "states.h"
#include "lsm9ds1.h"
#include "nrf_twi_mngr.h"
//#include "supporting_functions.h"
#include "gpio.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xffffff )
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5,0);
state current_state;

void print_state(){
	printf("CURRENT STATE: %d \n", current_state);
}


typedef struct ThirdAngle{
   float phi, psi , degree;
   bool type;
}ThirdAngle;

/* Define the linked list structure.  This is used to link free blocks in order
 * of their size. */
// typedef struct A_BLOCK_LINK
// {
//     struct A_BLOCK_LINK * pxNextFreeBlock; << The next free block in the list. 
//     size_t xBlockSize;                     /*<< The size of the free block. */
// } BlockLink_t;
// static const uint16_t heapSTRUCT_SIZE = ( ( sizeof( BlockLink_t ) + ( portBYTE_ALIGNMENT - 1 ) ) & ~( ( size_t ) portBYTE_ALIGNMENT_MASK ) );

// void vPortEnterCritical(){
// 	printf("Entered vPortEnterCritical here. \n");

// }
TaskHandle_t  vtask1_handle;  
TaskHandle_t  vtask2_handle;  

void prvSetupHardware(){

	printf("Entered prvSetupHardware. \n");
}

// void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize ){
// 	printf("Entered vApplicationGetIdleTaskMemory \n");

// }

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );



void initialize_hardware(){
  ret_code_t error_code = NRF_SUCCESS;

  gpio_config(BUCKLER_LED0, true);
  gpio_config(BUCKLER_LED1, true);
  gpio_config(BUCKLER_LED2, true);


  gpio_set(BUCKLER_LED0);
  gpio_set(BUCKLER_LED1);
  gpio_set(BUCKLER_LED2);

  gpio_config(BUCKLER_BUTTON0, false);

  // any additional robot related initialization can be placed here as well

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");


    //gyroscope
  
  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize LSM9DS1 driver
  lsm9ds1_init(&twi_mngr_instance);
  printf("lsm9ds1 initialized\n");
  

}

int main(void) {
	ret_code_t error_code;

    /* Initialize clock driver for better time accuracy in FREERTOS */
    error_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(error_code);
  	initialize_hardware();
  	ThirdAngle sharedVar =  {0.0,0.0,0.0, true};


  

  
  // initialize the state variable

  current_state = INIT; 


 xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE + 200, &sharedVar, 2, &vtask1_handle);
 xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE + 200, NULL, 2, &vtask2_handle);
 /* The task handle is the last parameter _____^^^^^^^^^^^^^ */


 printf("The two tasks are created now \n");
 /* Start the scheduler so the tasks start executing. */
 vTaskStartScheduler(); 
 
 for(;;){

 }




 /* If all is well then main() will never reach here as the scheduler will 
 now be running the tasks. If main() does reach here then it is likely there
 was insufficient heap memory available for the idle task to be created. 
 Chapter 2 provides more information on heap memory management. */

 	return 0;
}


/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
	ret_code_t error_code = NRF_SUCCESS;
	const char *pcTaskName = "Task 1 is running\r\n";
	volatile uint32_t ul;
	lsm9ds1_measurement_t acc_measurement;
	

	ThirdAngle  *local = (ThirdAngle*) pvParameters;
	
  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  

  // initialize LEDs
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  
    nrf_drv_spi_config_t spi_config = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };
  error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code);

  // initialize display driver

  
  printf("Display initialized\n");
  

  display_init(&spi_instance);


	char buf[16]= {0};
	char buf2[16] = {0};

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		nrf_delay_ms(250);
		print_state();
		/* Print out the name of this task. */
		printf( "%s %d\n",pcTaskName ,current_state);

		



		switch(current_state){
		case ON:
			acc_measurement = lsm9ds1_read_accelerometer();
			snprintf(buf, 16, "Hi Mom");
			printf("case %d running on task %s", current_state, pcTaskName);
			local->psi = acc_measurement.x_axis;
			local->phi = acc_measurement.y_axis;
			local->degree = acc_measurement.z_axis;


		
				snprintf(buf, 16, "aX%.3f Y%.3f",local->psi, local->phi);
				display_write(buf,0);

				snprintf(buf2, 16, "aZ: %.3f ",local->degree);
				display_write(buf2,1);


			break;

		case OFF:
			if(true)
			{
				snprintf(buf, 16, "");
				display_write(buf,0);
				display_write("",1);
			}
			
			break;



		default:
			snprintf(buf, 16, "Lab5 Q2");
			break;

		}
		
		display_write(buf,0);
		vTaskDelay(300);

	}
}
// /*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
const char *pcTaskName = "Task 2 is running\r\n";
volatile uint32_t ul;


	nrf_gpio_cfg_output(BUCKLER_LED1);
	
  // loop forever, running state machine
  while (1) {
    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(250);
   
   
    
    // iterate statechart
    switch(current_state){    
      case INIT:
        // move on to testing the button status
        current_state = OFF;
        break;
      case ON:
        
        gpio_clear(BUCKLER_LED0);    
        if (gpio_read(BUCKLER_BUTTON0) )
          {
            
            current_state = OFF;
            
            
          }
        break;
      case OFF:
        gpio_set(BUCKLER_LED0);
        
        if (!gpio_read(BUCKLER_BUTTON0) )
          {current_state = ON;   
          }
        break;
      
      default:
        current_state = OFF;

    }
    vTaskDelay(300);
    
  }

  
 
}

