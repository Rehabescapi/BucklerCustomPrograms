// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>  // needed for tilt angle calculation


#include "timestamp.h"
#include "support.h"

#define RAD2DEG 57.13 

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2


float timestamp=0.0;
float baseTimeStamp;



// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
 nrf_saadc_value_t sample_value (uint8_t channel) {
   nrf_saadc_value_t val;
   ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
   APP_ERROR_CHECK(error_code);
   return val;
 }

// Display app
//
// Write messages to a Newhaven OLED display over SPI


#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrf_drv_spi.h"

#include "buckler.h"
#include "display.h"


//from i2c
 #include "lsm9ds1.h"
 #include "nrf_twi_mngr.h"
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);


int main(void) {



  
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");

  // initialize spi master
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  nrf_drv_spi_config_t LCD_CONFIG = {   .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };
  nrf_drv_spi_config_t spi_config = LCD_CONFIG;
 
  error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code);

  // initialize display driver
  display_init(&spi_instance);
  printf("Display initialized\n");
  nrf_delay_ms(1000);

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


  setDevices();





  lsm9ds1_start_gyro_integration();


  
  
  //Calibration shenanigans
  struct ThirdAngle Angles, xAngles, yAngles;
  float ax =0, ay =0, az = 0;
  float gx = 0 ,gy = 0, gz = 0;
  Angles.phi = 0;
  Angles.psi = 0;
  Angles.degree = 0;
  

  init_SDCard();
  bool button0, switch0;
  bool bToggle= false;





  void printAccel(){
  printf("Voltage = x: %6.3f\ty: %6.3f\tz: %6.3f\n", ax, ay, az);
  printf("Psi %f\t Phi  %f\t Delta %f\t", Angles.psi, Angles.phi, Angles.degree);
  printf(" Gx 6.3%f, gY 6.3%f, Gz %f\n\n", gx, gy, gz);
}

  void logThings(){
    float f = timestamp-baseTimeStamp;
    simple_logger_log("%f,%f, %f,%f,%f,%f,%f,%f,", timestamp, f, ax, ay, az, gx, gy, gz);
    simple_logger_log("%f, %f, %f.\n", Angles.psi, Angles.phi, Angles.degree);
    
  }
  
  simple_logger_log("Time,AdjTime, X,Y,Z,gx,gy,gz, psi, phi, delta,\n");

  // Write test numbers in a loop
  unsigned int i = 0;
  char clearbuf[16] ={0};
  printf("loop started");
  while(1) {


    button0 = getInput(28);
    switch0 = getInput(22);
    deviceLoop(bToggle);
   
    lsm9ds1_measurement_t acc_measurement = lsm9ds1_read_accelerometer();
    lsm9ds1_measurement_t gyr_measurement = lsm9ds1_read_gyro_integration();

    ax = acc_measurement.x_axis;
    ay = acc_measurement.y_axis;
    az = acc_measurement.z_axis;

    gx = gyr_measurement.x_axis;
    gy = gyr_measurement.y_axis;
    gz = gyr_measurement.z_axis;


    if(switch0){ //if switch0 is in high mode.
      if(!button0)
      {
        printf("%d Btoggle\n" , bToggle);
        bToggle = !bToggle;
        nrf_delay_ms(500);
        baseTimeStamp = get_timestamp(); 
      }

    
      if(bToggle){// If switch is in high mode then hitting 
        assign3D(&Angles, ax, ay, az);
        printAccel();
        timestamp = get_timestamp();    
        logThings();
      }
    }

    else{


      if(!button0){
        assign3D(&Angles, ax, ay, az);
        printAccel();

        timestamp = get_timestamp();   
        simple_logger_log(" Static Timestamp\n\n");   
        logThings();
        simple_logger_log("End of click \n\n");
        nrf_delay_ms(300);

      }

    }
  }

/*

    if(safetyCheck(Angles)){
    snprintf(hbuf, 16,"tilt=phi|psi|delta");
    display_write(hbuf,0);

    snprintf(buf, 16, "%2.2f|%2.2f|%2.2f", Angles.phi, Angles.psi, Angles.degree);
    display_write(buf, 1);
    printf("Acceleration (g): %10.3f\t%10.3f\t%10.3f\n", Angles.phi, Angles.psi, Angles.degree);
  

    }else{
      snprintf(buf, 16,"!!Danger OF OVERTURN");
      display_write(buf, 0);
      display_write(clearbuf,1);

    }
*/

/*    void question_2() {
  
    lsm9ds1_measurement_t acc_measurement = lsm9ds1_read_accelerometer();
    lsm9ds1_measurement_t gyr_measurement = lsm9ds1_read_gyro_integration();

    printf("                      X-Axis\t    Y-Axis\t    Z-Axis\n");
    printf("                  ----------\t----------\t----------\n");
    printf("Acceleration (g): %10.3f\t%10.3f\t%10.3f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    printf("Angle  (degrees): %10.3f\t%10.3f\t%10.3f\n", gyr_measurement.x_axis, gyr_measurement.y_axis, gyr_measurement.z_axis);
    printf("\n");


    char buf[16] = {0};
    char headerbuf[16] = {0};
    display_write(clearbuf,1);
    display_write(clearbuf, 0);

    
    if(i%2 == 0){
    snprintf(headerbuf, 16, "gX:%2.1f gY:",gyr_measurement.x_axis);
    snprintf(buf, 16, "%2.1f gZ:%2.1f\n",  gyr_measurement.y_axis, gyr_measurement.z_axis);
    //lsm9ds1_stop_gyro_integration();
    //lsm9ds1_start_gyro_integration();


    }
    else{
      snprintf(headerbuf, 16, "aX:  |aY:  |aZ:");
      snprintf(buf, 16, "%2.2f|%2.2f|%2.2f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    }
    
    //todo make (buf and snprintf header buf work well togethor)
    display_write(headerbuf, 0);
    display_write(buf,1);
    i += 7;
    nrf_delay_ms(1000);

  }
  */
}
  






