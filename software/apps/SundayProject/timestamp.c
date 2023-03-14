#include "timestamp.h"

float init_time;

//I think this would work.

float get_timestamp(){
  
  return (float) (read_timer() - init_time)*1/31250;
}

void init_SDCard(){
  ret_code_t error_code = NRF_SUCCESS;
  // SD Card Checking
  printf("Started SD card demo app...\n");
  timer_init();

  // Enable SoftDevice (used to get RTC running)
  nrf_sdh_enable_request();

  // Initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    error_code = nrfx_gpiote_init();
  }
  APP_ERROR_CHECK(error_code);
  // Configure GPIOs
  nrf_gpio_cfg_output(BUCKLER_SD_ENABLE);
  nrf_gpio_cfg_output(BUCKLER_SD_CS);
  nrf_gpio_cfg_output(BUCKLER_SD_MOSI);
  nrf_gpio_cfg_output(BUCKLER_SD_SCLK);
  nrf_gpio_cfg_input(BUCKLER_SD_MISO, NRF_GPIO_PIN_NOPULL);

  nrf_gpio_pin_set(BUCKLER_SD_ENABLE);
  nrf_gpio_pin_set(BUCKLER_SD_CS);
  gpio_config(BUCKLER_BUTTON0, 0);
  // Initialize SD card
  const char filename[] = "testfile.csv";
  const char permissions[] = "w"; // w = write, a = append
  // Start file
  simple_logger_init(filename, permissions);
  // simple_logger_log_header("HEADER for file \'%s\', written on %s \n", filename, "DATE");
  simple_logger_log_header("Count, Time, TimeAverage \n");
  printf("Wrote header to SD card\n");
  
  // Activate BUTTON0 as the start record button
  gpio_config(BUCKLER_BUTTON0, 0);
  
  // Indicator LED0 for data recording.
  gpio_config(BUCKLER_LED0, 1);
  // initial time recorded
  init_time = read_timer();
}