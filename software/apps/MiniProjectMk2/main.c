// Kobuki sensors app
//
// Read and print sensor values from the Kobuki robot

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorTypes.h"
#include "kobukiSensorPoll.h"
#include "kobukiUtilities.h"


#include "lsm9ds1.h"
#include "helperfunctions.h"

//extern KobukiSensors_t sensors;

#define CONVERSION 0.000677 // 2*pi/65535 * Wheel_dia in cm
// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 4, 0);

float distance = 0.0f;
float bias = 0.0f;



int main(void) {
   KobukiSensors_t sensors;
 
  ret_code_t error_code = NRF_SUCCESS;

  // intitialize Kobuki library
 
  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Initialized RTT!\n");





  float distance_measure(uint16_t encoder){
    kobukiSensorPoll(&sensors);
    uint16_t current_encoder = sensors.leftWheelEncoder;
    float update_dist = current_encoder - encoder;
    return (float) (update_dist * CONVERSION) - bias;
}

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
  


  display_write("Hello, uman!", DISPLAY_LINE_0);
  printf("Display initialized!\n");



  kobukiInit();
  bias = distance_measure(0);
  float goal = .5;
  float goalAngle = 90;
  states state = OFF;
   int buf[16] = {0};
  printf("Kobuki Initialized initial bias of %f\n", bias);
  stop_kobuki();
  bool startCycle = false;


  float angle = 0.0f;
  start_gyro();

  /** Helper function to reset
   * 
   * 
   * */

   void setNextAngle(int target){
    stop_gyro();
    stop_kobuki();              
    angle = 0;
    goalAngle=target;
    nrf_delay_ms(100);
    start_gyro();
}
  int phaseCount = 0;
  float weight =.5;
  
  uint16_t prev_encoder = sensors.leftWheelEncoder - bias;


  // loop forever
  while (1) {

    // read kobuki sensor values
    
    kobukiSensorPoll(&sensors);
    angle = read_gyro();
    if(startCycle)
    {
      

    switch(state){
    case OFF: 
      if(sensors.bumps_wheelDrops.bumpCenter== 1 &&sensors.bumps_wheelDrops.bumpRight ==0){
       

        if(phaseCount %2 == 0)
        { 
          
          state = DRIVING;
          goal = distance_measure(prev_encoder) + weight;

        }else if(phaseCount %2 ==1)
        {
           state = RIGHT;
        }
        phaseCount++;
       
       
       
      }
      snprintf(buf, 16, "OFF! C:%d", phaseCount);
       display_write(buf, DISPLAY_LINE_0);
        snprintf(buf, 16, "D:%.3f G:%.3f", distance, goal);
          display_write(buf, DISPLAY_LINE_1);



       stop_kobuki();

      break;
    case RIGHT:
      if(is_button_pressed(&sensors)){
        
        state = OFF;
      }
      if(abs(angle)<= goalAngle){
        printf("Angle : %f \n", angle);
        display_write("Turning", DISPLAY_LINE_0);

         snprintf(buf, 16, "g: %.2f c %.2f", goalAngle, (goalAngle -abs(angle)));
                  display_write(buf, DISPLAY_LINE_1);
                  nrf_delay_ms(50);




        drive_kobuki(50,-50);

      }

      else {
        nrf_delay_ms(300);
        setNextAngle(135);
        state = OFF;
      }
      break;

   
    case DRIVING:
       distance = distance_measure(prev_encoder);
    if (distance<=goal){

           drive_kobuki(50,50);
           printf("Encoders: %d - %d\n bias\t %f \n", sensors.leftWheelEncoder, sensors.rightWheelEncoder, bias);
           snprintf(buf, 16, "D:%.3f G:%.3f", distance, goal);
          display_write(buf, DISPLAY_LINE_1);


          
     }
     else
     {
       stop_kobuki();
       printf("Distance Reached! \n");
      
       if(phaseCount <=12)
       {
        weight = .5;
       }
       else{
        weight =.77;
       }
       state = OFF;
     }


    break;
}
    
  }




    printf("Angle from gyro read at: %f\n\n", angle);
    



    // print results
    printf("\n");
    printf("### Kobuki Sensor Values ###\n");
    printf("Wheel drop left:\t%d\n", sensors.bumps_wheelDrops.wheeldropLeft);
    printf("Wheel drop right:\t%d\n", sensors.bumps_wheelDrops.wheeldropRight);
    printf("\n");
    printf("Bump left:\t%d\n", sensors.bumps_wheelDrops.bumpLeft);
    printf("Bump center:\t%d\n", sensors.bumps_wheelDrops.bumpCenter);
    printf("Bump right:\t%d\n", sensors.bumps_wheelDrops.bumpRight);
    printf("Encoder left:\t%d\n", sensors.leftWheelEncoder);
    printf("Encoder right\t%d\n", sensors.rightWheelEncoder);
    printf("\n");
    printf("Current left:\t%d\n", sensors.leftWheelCurrent);
    printf("Current right\t%d\n", sensors.rightWheelCurrent);
    printf("\n");
    printf("PWM left:\t%d\n", sensors.leftWheelPWM);
    printf("PWM right\t%d\n", sensors.rightWheelPWM);
    printf("\n");
    printf("Over current left:\t%d\n", sensors.leftWheelOverCurrent);
    printf("Over current right\t%d\n", sensors.rightWheelOverCurrent);
    printf("\n");
    printf("Battery voltage:\t%d\n", sensors.batteryVoltage);
    printf("Charging state:\t%d\n", sensors.chargingState);
    printf("\n");
    printf("Angle:\t%d\n", sensors.angle);
    printf("Rate:\t%d\n", sensors.angleRate);


    printf("Is buutton pressed: %d\n", is_button_pressed(&sensors));


    if(sensors.bumps_wheelDrops.bumpLeft ==1  && sensors.bumps_wheelDrops.bumpRight ==1 && sensors.bumps_wheelDrops.bumpCenter ==0)
    {
      printf("GO GO GO \n\n\n");
      display_write("L 1 R 1",DISPLAY_LINE_1);
      startCycle = true;
      state = OFF;
     
    }else{
      
    }
    nrf_delay_ms(50);
  }
}

