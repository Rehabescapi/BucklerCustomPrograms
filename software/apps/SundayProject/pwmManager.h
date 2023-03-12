#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "app_error.h"
#include "bsp.h"
#include "nrf_delay.h"
#include "app_pwm.h"




 


void pwm_ready_callback(uint32_t pwm_id);

void pwm_init();


void setTwoChannel(int);