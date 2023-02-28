#include "gpio.h"

// Inputs:
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
    uint32_t* addr = (uint32_t*)(0x50000700 + (gpio_num * 4));
    *addr = dir;
}

// Set gpio_num high
// Inputs:
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
  uint32_t* addr = (uint32_t*)(0x50000700 + (gpio_num * 4));
  *addr |= (uint32_t)1;
}

// Set gpio_num low
// Inputs:
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
  // Get pin address
  uint32_t* addr = (uint32_t*)(0x50000700 + (gpio_num * 4));
  *addr &= (uint32_t)0;
}

// Inputs:
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // The address of the IN register
    uint32_t* in = (uint32_t*)0x50000510;

    // Create our bitwise mask
    uint32_t mask = 1;
    for (uint8_t i = 0; i < gpio_num; i++) {
      mask *= 2;
    }

    // Use a bitwise mask to get the value for our printf
    // Bitshift to get the single bit
    return (*in & mask) >> gpio_num;
}
