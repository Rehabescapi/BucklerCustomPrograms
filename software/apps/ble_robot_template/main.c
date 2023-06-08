// Robot Template app
//
// Framework for creating applications that control the Kobuki robot using BLE 

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_spi.h"

#include "buckler.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "mpu9250.h"


#include "simple_ble.h"
#include "states.h"

#include "helper_funcs.h"
//extern KobukiSensors_t sensors;


// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

// KobukiSensors_t sensors = {0};


// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:aa:bb  -- Make sure to match this with YOUR_ADDRESS in robot_control.py
        .platform_id       = 0x49,    	// used as 4th octect in device BLE address
        .device_id         = 0xaabb, 	// 
        .adv_name          = "TESTNAME", 	// used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(100, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t drive_service = {{
  .uuid128 = { 0x73, 0x30, 0xbb, 0x18,0x50,0xe5,0xab, 0x93, 0x75,0x4c,
0x7c, 0x45, 0xc4,0x99,0x58,0xc0}
}};


// c05899c4-457c-4c75-93ab-e55018bb3073 -- Make sure to match with SERVICE_UUID

static simple_ble_service_t led_service = {{
  .uuid128 = {0x70, 0x6c, 0x86, 0x19, 0xe4, 0x43,0x14, 0xa9, 0xb5, 0x4d, 0x22, 0x2b,0x89,0x10,0xe6,0x32}
}};
// TODO: define a service for driving with UUID c05899c4-457c-4c75-93ab-e55018bb3073


// TODO: defining characteristics related to driving 
  
	// characteristic for forward move: forward_char  -- {.uuid16 = 0x99c5}
	// connected variable		  : drive_forward
  static simple_ble_char_t forward_char = {.uuid16 = 0x99c5};
  static bool drive_forward;

	// characteristic for backward move: backward_char -- {.uuid16 = 0x99c6}
	// connected variable		   : drive_backward
  static simple_ble_char_t backward_char = {.uuid16 = 0x99c6};
  static bool drive_backward;

static simple_ble_service_t turn_service = {{
  .uuid128 = { 0x73, 0x30, 0xbb, 0x18,0x50,0xe5,0xab, 0x93, 0x75,0x4c,
0x7c, 0x45, 0xc7,0x99,0x58,0xc0}
}};

  static simple_ble_char_t right_char = {.uuid16 = 0x99c8};
  static bool drive_right;


  static simple_ble_char_t left_char = {.uuid16 = 0x99c9};
  static bool drive_left;

  static simple_ble_char_t led_char = {.uuid16 = 0x8911};
  static bool LED;

static bool prev_df, prev_db, prev_dr, prev_dl = 0;

simple_ble_app_t* simple_ble_app;


// robot drive variables

// left and right wheel speeds -- 0 - 100
static int16_t leftdrive, rightdrive = 0;

// speed increment at each button press from BLE controller 
static int16_t speed = 512;

// turn speed increment at each button press from BLE controller
static int16_t turning_speed = 300;

static int boardArray[2][16] = {' '};
static int x = 0;
static int y = 0;


static char display_data[33] = {0};
void displayGame(int dir){
  char lineOne[16] = {0};
  char lineTwo[16]= {0};
  switch (dir)
  {
  case 1:
  case 2:
    x = x +16;
   
    
    break;


  case 3:
    x = x-1;
    if(x<0){
      x = 31;
    }
    break;

  case 4:
     x = x+1;
   

    break;

  default:
    printf("I dun goofed");

  }
   x = x%32;
  printf("%d",x );
  display_data[x] = 'X';

  nrf_delay_ms(10);

  printf("value of data [%s]\n", display_data);
  memcpy(lineOne, &display_data, 16*sizeof(char));

  display_write(lineOne, DISPLAY_LINE_0);
  nrf_delay_ms(50);

  memcpy(lineTwo, &display_data[16], 16*sizeof(char));

  display_write(lineTwo, DISPLAY_LINE_1);

  memset(display_data, ' ',33);
  display_data[32] = '\0';


}

void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &forward_char)) {
      printf("Got write to forward characteristic!\n");
      if (drive_forward != prev_df) {
          if(drive_forward) {
              leftdrive += speed;
              rightdrive += speed;
          } 
	else {
              leftdrive -= speed;
              rightdrive -= speed;
          }
      }

	// a flag to remember if forward was pressed
      prev_df = drive_forward;
      simple_ble_notify_char(&forward_char);
      displayGame(1);
    }
    if (simple_ble_is_char_event(p_ble_evt, &backward_char)) {
      printf("Got write to backward characteristic!\n");
      if (drive_backward != prev_db) {
          if(drive_backward) {
              leftdrive -= speed;
              rightdrive -= speed;
          } 
	
          else {
              leftdrive += speed;
              rightdrive += speed;
          }
      }

	// a flag to remember if backward was pressed
      prev_db = drive_backward;
      simple_ble_notify_char(&backward_char);
      displayGame(2);
    }
    if (simple_ble_is_char_event(p_ble_evt, &right_char)) {
      displayGame(4);
      printf("Got write to right characteristic!\n");
      if (drive_right != prev_dr) {
          if(drive_right) {
              leftdrive += turning_speed;
              rightdrive -= turning_speed;
          } 
	  else {
              leftdrive -= turning_speed;
              rightdrive += turning_speed;
          }
      }
	// a flag to remember if right turn was pressed
      prev_dr = drive_right;
      simple_ble_notify_char(&right_char);
    }
    if (simple_ble_is_char_event(p_ble_evt, &left_char)) {
      printf("Got write to left characteristic!\n");
      displayGame(3);
      if (drive_left != prev_dl) {
          if(drive_left) {
              leftdrive -= turning_speed;
              rightdrive += turning_speed;
          } 
	 else {
              leftdrive += turning_speed;
             rightdrive -= turning_speed;
          }
      }
	// a flag to remember if left turn was pressed
      prev_dl = drive_left;
      simple_ble_notify_char(&left_char);
    }
    if(simple_ble_is_char_event(p_ble_evt, &led_char)){
  }
	// a small delay to allow the action for some time
     nrf_delay_ms(50); 
}

int main(void) {

  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  printf("%s", ble_config.adv_name);
  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&drive_service);


  
  // TODO: adding the characteristics for each directional movement

  // characteristic for backward move: backward_char
  // connected variable      : drive_backward
  simple_ble_add_characteristic(1, 1,0,0,
    sizeof(drive_backward), (uint8_t*)&drive_backward,
    &drive_service, &backward_char);



	// characteristic for forward move: forward_char
	// connected variable		  : drive_forward
  simple_ble_add_characteristic(1, 1,0,0,
    sizeof(drive_forward), (uint8_t*)&drive_forward,
    &drive_service, &forward_char);

  simple_ble_add_service(&turn_service);

      // uint8_t robot_service_char;
  // characteristic for right move  : right_char
  // connected variable     : drive_right 
  simple_ble_add_characteristic(1, 1,0,0,
    sizeof(drive_right), (uint8_t*)&drive_right,
    &turn_service, &right_char);

  

  // characteristic for left move   : left_char
  // connected variable     : drive_left
  simple_ble_add_characteristic(1, 1,0,0,
    sizeof(drive_left), (uint8_t*)&drive_left,
    &turn_service, &left_char);

  








  // Start Advertising
  simple_ble_adv_only_name();



  // You can choose to keep the LCD working
  
  // // initialize display
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
   display_init(&spi_instance);
    printf("Display initialized!\n");
    display_write("Hi Human", DISPLAY_LINE_0);
  
   /* 
   // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize all the sensors in LSM9DS1
  mpu9250_init(&twi_mngr_instance);
  printf("IMU initialized!\n");
/*
  // initialize Kobuki
  kobukiInit();
  printf("Kobuki initialized!\n");
  */
  
   displayGame(5);
  // loop forever, running state machine
  while (1) {
  
   // even if we are not using the sensors, this command is needed to update the robot status
   //kobukiSensorPoll(&sensors);
   nrf_delay_ms(5); 
    
    // an ever present function to operate the robot. As a result of drive actions in BLE application, the drive speeds for left and right wheels are modified   
    //printf("Driving with Left: %d \t Right: %d \n", leftdrive, rightdrive);
    //kobukiDriveDirect(leftdrive,rightdrive);
    power_manage();

  }
}

