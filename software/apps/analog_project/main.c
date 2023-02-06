// Analog accelerometer app
//
// Reads data from the ADXL327 analog accelerometer

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <math.h>
#include <gpio.h>

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
#include "nrfx_saadc.h"

#include "buckler.h"

#include <support.h>

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2

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


float revert (float Voltage){ 
  float VoltAndSlope = 131* 2.9;
  //conversion slope is 1V / (131 mV/g)
  float A = .001 *VoltAndSlope; // mV/g
  float B = 1.45; //listed default bias 3 at 2.9V it would be 1/2
  
  return (Voltage - B )/A;
}

float getAngle(float X , float Y){
  float rad, rad2,degree;
  rad =  (float)atan2f(X, Y);


  
  //printf("Arc Tan %f and %f is  %f\n\n",X, Y, rad* (180.0/ M_PI));



  
  return  rad * (180.0/ M_PI);


}

float singleAngle( float A){
  return asinf(A/9.8) * (180/M_PI);
}





int main (void) {
   struct ThirdAngle Angles;
  setDevices();
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // initialize analog inputs
  // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.gain = NRF_SAADC_GAIN1_6; // input gain of 1/6 Volts/Volt, multiply incoming signal by (1/6)
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference, input after gain can be 0 to 0.6 Volts

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // initialization complete
  printf("Buckler initialized! Nerds!!\n");
  float LSB = 0.0008789; // (reference/ internal Gain) / (2^SAADC Resolution)
  float volt_x, volt_y, volt_z, g_x, g_y , g_z;

  //todo calculate actual LSB; GAIN1_6 , REFERENCEINTERNAL
  // loop forever
   
  bool button0, switch0;
  bool bToggle= true;

void printAccel(){
  printf("Voltage = x: %6.3f\ty: %6.3f\tz: %6.3f\n", volt_x, volt_y, volt_z);

  printf("Single Angle measurement of X%f\t Y = %f\t Z = %f\t\n\n", g_z, g_y, g_x);


  printf("Double Angle of Things %f\t   %f\t\n\n", getAngle(volt_y, volt_z), getAngle(volt_x, volt_z));


  
}


  while (1) {
    button0 = getInput(28);
    switch0 = getInput(22);
    

    deviceLoop();

    nrf_saadc_value_t x_val = sample_value(X_CHANNEL);
    nrf_saadc_value_t y_val = sample_value(Y_CHANNEL);
    nrf_saadc_value_t z_val = sample_value(Z_CHANNEL);


     volt_x = revert((float) x_val *LSB);
     volt_y = revert((float) y_val *LSB);
     volt_z = revert((float) z_val *LSB);

    // display results
     g_z = singleAngle(volt_z);
     g_y = singleAngle(volt_y);
     g_x = singleAngle(volt_x);

     





    if(switch0 && bToggle){ 

    
     printAccel();
    nrf_delay_ms(2000);


    
    }
    else{


      if(!button0){
        assign3D(&Angles, volt_x, volt_y, volt_z);
        printAccel();



        printf("PSI    %f\t PHI  %f\t, Degree %f\n\n", Angles.psi, Angles.phi, Angles.degree);

        nrf_delay_ms(300);

      }else{
        

      }

    }
    
    // sample analog inputs
   




    //printf("Angle of Z/X %f\n Angle of Z/Y = %f\n\n", getAngle(g_z, g_x), getAngle(g_z, g_y));
    
  }

}

