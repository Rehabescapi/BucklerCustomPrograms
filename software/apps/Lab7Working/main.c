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




/*
  //Might be redundant.
  nrf_drv_spi_config_t ACCE_CONFIG = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST

  }*/

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

  lsm9ds1_start_gyro_integration();







  //end of gyroscope.




  //Calibration shenanigans
  struct ThirdAngle Angles, xAngles, yAngles;
  float ax =0, ay =0, az = 0;
  Angles.phi = 0;
  Angles.psi = 0;
  Angles.degree = 0;
  //setDevices();



  //Variables initialized




  // Write test numbers in a loop
  unsigned int i = 0;
  while(1) {
    lsm9ds1_measurement_t acc_measurement = lsm9ds1_read_accelerometer();
    lsm9ds1_measurement_t gyr_measurement = lsm9ds1_read_gyro_integration();

    printf("                      X-Axis\t    Y-Axis\t    Z-Axis\n");
    printf("                  ----------\t----------\t----------\n");
    printf("Acceleration (g): %10.3f\t%10.3f\t%10.3f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    printf("Angle  (degrees): %10.3f\t%10.3f\t%10.3f\n", gyr_measurement.x_axis, gyr_measurement.y_axis, gyr_measurement.z_axis);
    printf("\n");


    char buf[16] = {0};
    
    display_write("", 0);

    nrf_delay_ms(2000);
    if(i%2 == 0){
    snprintf(buf, 16, "gX%2.2fgY%2.2fgZ%2.2f\n", gyr_measurement.x_axis, gyr_measurement.y_axis, gyr_measurement.z_axis);
    }
    else{
      snprintf(buf, 16, "X%2.2fY%2.2fZ%2.2f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    }
    
    //todo make (buf and snprintf header buf work well togethor)

    display_write(buf,0);
    i += 7;
    nrf_delay_ms(2000);
  }

    void printAccel(){
  printf("X: %6.3f Y: %6.3fZ: %6.3f\n", ax, ay, az);
  printf("Psi %f\t Phi  %f\t Delta %f\n\n", Angles.psi, Angles.phi, Angles.degree);
}

}







/*
int main (void) {
  struct ThirdAngle Angles, xAngles, yAngles;
  setDevices();

  ret_code_t error_code = NRF_SUCCESS;

  // // initialize RTT ENDS_INIT();

  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // // initialize analog inputs
  // // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

  // // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // initialization complete
  printf("Buckler initialized!\n");


  
  
  ////////////////////

  // calibration of the voltage output from the ADC

  float LSB = 0.000879; // 3.6V/ 2^12 -- where 3.6V = 0.6/(1/6) 
  float supplyV = 2.91;  // Volts
  float vx = 0.0;
  float vy = 0.0; 
  float vz = 0.0;
  
  float ax, ay, az;

  float mx = .42/3 * supplyV;
  float my = .42/3 * supplyV;
  float mz = .42/3 * supplyV;

  float bx = 1.43;// from the observation//1.5/3 * supplyV;
  float by = 1.45;// from the observation//1.5/3 * supplyV;
  float bz = 1.48;// from the observation //1.5/3 * supplyV;


  // tilt variables

  //float phi = 0.0; // angle about x-axis
  //float theta = 0.0;  // angle about z-axis
  //float psi = 0.0; // angle about y-axis


   nrf_saadc_value_t x_val = sample_value(X_CHANNEL);
    nrf_saadc_value_t y_val = sample_value(Y_CHANNEL);
    nrf_saadc_value_t z_val = sample_value(Z_CHANNEL);
  
  // Initialize the SD Card for logging data in TESTFILE.csv
  init_SDCard();
  bool button0, switch0;
  bool bToggle= false;



  void logThings(){
    float f = timestamp-baseTimeStamp;
    simple_logger_log("%f,%f, %f,%f,%f,%f,%f,%f,", timestamp, f, ax, ay, az, Angles.phi, Angles.psi, Angles.degree);
    simple_logger_log(",%f,%f, %f,%f,%f,%f,\n", xAngles.phi, xAngles.psi, xAngles.degree, yAngles.phi, yAngles.psi, yAngles.degree);
  }
  
  simple_logger_log("Time,AdjTime, X,Y,Z,phi,psi,degree, ,xPhi,xPsi,xDegree, yPhi,yPsi,yDegree,\n");

  // loop forever
  while (1) {
    
    button0 = getInput(28);
    switch0 = getInput(22);
    deviceLoop(bToggle);
    // sample analog inputs
   x_val = sample_value(X_CHANNEL);
   y_val = sample_value(Y_CHANNEL);
   z_val = sample_value(Z_CHANNEL);

    vx = LSB*x_val;
    vy = LSB*y_val;
    vz = LSB*z_val;

	// Calculation of the acceleration from the calibrated voltage output of the accelerometer
    ax = (vx - bx)/mx;
    ay = (vy - by)/my;
    az = (vz - bz)/mz;
    // display results
     
    

    if(switch0){ //if switch0 is in high mode.



      if(!button0)
      {
        printf("%d Btoggle\n" , bToggle);
        bToggle = !bToggle;
        nrf_delay_ms(500);
        baseTimeStamp = get_timestamp(); 
        //simple_logger_log("%d Btoggle\n\n", bToggle);

      }

    
      if(bToggle){// If switch is in high mode then hitting 
        
        assign3D(&Angles, ax, ay, az);
        assign3D(&xAngles, ay, az, ax);
        assign3D(&yAngles, az,ax,ay);
        printAccel();
          timestamp = get_timestamp();    
          logThings()  ;   
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

      }else{
        

      }

    }
    








  }   
}


*/