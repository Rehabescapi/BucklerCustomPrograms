
// Robot Utilities app
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

extern KobukiSensors_t sensors;

#define CONVERSION 0.000677 // 2*pi/65535 * Wheel_dia in cm
// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);


float distance = 0.0f;
float bias = 0.0f;


float distance_measure(uint16_t encoder)
{
  kobukiSensorPoll(&sensors);
  uint16_t cur_encoder = sensors.leftWheelEncoder ;

  

  float updated_dist = cur_encoder - encoder;


  return (float) (updated_dist * CONVERSION) -bias; 
}


int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
 


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

  // initialize all the sensors in LSM9DS1
  lsm9ds1_init(&twi_mngr_instance);
  printf("IMU initialized!\n");

  // initialize Kobuki
  
  kobukiInit();


   bias = distance_measure(0);
   float goal = .5;

  printf("Kobuki initialized! with initial bias of %f\n",bias);

  // configure initial state
  states state = OFF;
  KobukiSensors_t sensors = {0};

  // distance measurement, encoder
  uint16_t prev_encoder = sensors.leftWheelEncoder -bias;

  float angle = 0.0f;
  float goalAngle =  90.0f;
  int step = 1;
  int buf[16] = {0};
  
  stop_kobuki();

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    kobukiSensorPoll(&sensors);
   
    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(10);

    switch(step){
    /// Testing the Point Rotation 
    ///////////////////////////////////////
     case 0:
        start_gyro();

       if (abs(angle)<=goalAngle)
       {

        angle = read_gyro();
         printf("Angle: %f \n", angle);

          display_write("Spining Right", DISPLAY_LINE_0);
          snprintf(buf, 16, "g: %.2f c %.2f", goalAngle, angle);
          display_write(buf, DISPLAY_LINE_1);

         drive_kobuki(50, -50);

       }

       else{      
         stop_gyro();
         stop_kobuki();
         step = 1; 
         distance = distance_measure(prev_encoder);
         goal = distance + .5;
         //bias = distance_measure(0);     
        }
        break;

    case 1:
      if (distance<=goal){

           drive_kobuki(50,50);
           printf("Encoders: %d - %d\n bias\t %f \n", sensors.leftWheelEncoder, sensors.rightWheelEncoder, bias);
           distance = distance_measure(prev_encoder);
           snprintf(buf, 16, "Distance %f", distance);
          display_write(buf, DISPLAY_LINE_1);


           snprintf(buf, 16, "Goal %f", goal);
          display_write(buf, DISPLAY_LINE_0);
     }
     else
     {
       stop_kobuki();
       printf("Distance Reached! \n");
      
       nrf_delay_ms(500);
       goalAngle = (angle + 90.0 );
       if(goalAngle > 360){
        goalAngle = goalAngle - 360;
       }
       step = 0;
     }
    

     printf("Distance: %f \n", distance);

      break;

    }

    /////////////////////////////////////////////

    /// Testing the Straight Movement 
    /// for straight movement, distance measurement can be done using one wheel's
    /// encoder reading as well
    ///////////////////////////////////////

 

     
    

     

}
return 0;
}