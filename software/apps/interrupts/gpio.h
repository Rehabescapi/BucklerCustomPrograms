#pragma once

#include "nrf.h"
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
#include "nrf_serial.h"

typedef struct 
{
    uint32_t OUT;
    uint32_t OUTSET;
    uint32_t OUTCLEAR;
    uint32_t IN; 
    uint32_t DIR;
    uint32_t DIRSET;
    uint32_t DIRCLR;

    
    
} gpio;

typedef struct {
    uint32_t PIN0;
    uint32_t PIN1;
    uint32_t PIN2;
    uint32_t PIN3;
    uint32_t PIN4;
    uint32_t PIN5;
    uint32_t PIN6;
    uint32_t PIN7;
    uint32_t PIN8;
    uint32_t PIN9;
    uint32_t PIN10;
    uint32_t PIN11;
    uint32_t PIN12;
    uint32_t PIN13;
    uint32_t PIN14;
    uint32_t PIN15;
    uint32_t PIN16;
    uint32_t PIN17;
    uint32_t PIN18;
    uint32_t PIN19;
    uint32_t PIN20;
    uint32_t PIN21;
    uint32_t SWITCH0;//22
    uint32_t BLUE;//23
    uint32_t YLLO;//24
    uint32_t RED;//25
    uint32_t PIN26;
    uint32_t PIN27;
    uint32_t BUTTON0;//28
    uint32_t PIN29;
    uint32_t PIN30;
    uint32_t PIN31;
} pins;

typedef enum {
    INPUT = 0,
    OUTPUT,
} gpio_direction_t;

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir);

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num);

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num);

// Inputs: 
//  gpio_num - gpio number 0-31
// Returns:
//  current state of the specified gpio pin
bool gpio_read(uint8_t gpio_num);

bool gpio_readButton();
void setDevices();
