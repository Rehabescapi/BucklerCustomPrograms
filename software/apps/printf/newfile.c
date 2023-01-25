
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
int main(void) {
 
  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // loop forever
  uint32_t i = 0;
  printf(&i);

  float a = 0;
  int b = 0;
  char c = '0';

  printf("Float A : %p\n Int B = %p\n  Char C =%p\n", &a, &b, &c);
  while (1) {
    nrf_delay_ms(10000);
    printf("Iteration: %lu\n", i++);
  }
}

