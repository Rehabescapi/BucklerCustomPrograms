// Robot Template app
//
// Framework for creating applications that control the Kobuki robot

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
#include "lsm9ds1.h"
#include "helper_funcs.h"

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);


uint16_t previous_encoder = 0;
float distance = 0;




int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // initialize LEDs
  nrf_gpio_pin_dir_set(23, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(24, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_pin_dir_set(25, NRF_GPIO_PIN_DIR_OUTPUT);

  // initialize display
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
  display_write("Hello, Human!", DISPLAY_LINE_0);
  printf("Display initialized!\n");

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);
  lsm9ds1_init(&twi_mngr_instance);
  printf("IMU initialized!\n");

  // initialize Kobuki
  kobukiInit();
  printf("Kobuki initialized!\n");

  // configure initial state
  states state = OFF;
  KobukiSensors_t sensors = {0};

  char buf[16] = {0};

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    kobukiSensorPoll(&sensors);

    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(1);

    // handle states
    switch(state) {
      case OFF: {
        // transition logic
        if (is_button_pressed(&sensors)) {
          state = DRIVING;
          // saving the wheel encoder position the moment we start moving
          previous_encoder = sensors.leftWheelEncoder;
        } else {
          // perform state-specific actions here
          display_write("OFF", DISPLAY_LINE_0);
          snprintf(buf, 16, "%d", previous_encoder);
          display_write(buf, DISPLAY_LINE_1);

          //printf("OFF : %f \n", distance);
          kobukiDriveDirect(0,0);

          state = OFF;
        }
        break; // each case needs to end with break!
      }

    case STARTING:{


      display_write("STARTING", DISPLAY_LINE_0);

      nrf_delay_ms(50);
      state = DRIVING;

    }

      case DRIVING: {
        // transition logic
        if (is_button_pressed(&sensors)) {
          state = OFF;
        } else {
          // perform state-specific actions here
          
          kobukiDriveDirect(50,50); 
           
          distance = update_dist(distance, previous_encoder, true);
          display_write("DRIVING", DISPLAY_LINE_0);
          snprintf(buf,16, "distance %f", distance);
          display_write(buf,  DISPLAY_LINE_1);
          printf("DRIVING : %f \n", distance);
          state = DRIVING;



        }
        break; // each case needs to end with break!
      }


    case RIGHT:{
      if(is_button_pressed(&sensors)){
        state = OFF;

      }

      kobukiDriveDirect(50, -50);
      display_write("TURNING RIGHT", DISPLAY_LINE_0);
      printf("TURNING : %f \n", distance);
    }

      // add other cases here

    }
  }
}
