#ifndef __TIZEN_GPIO_H__
#define __TIZEN_GPIO_H__

//#include <tizen.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file artik10_gpio.h
 * @brief This file contains GPIO API for ARTIK 10 development board related structures and enumerations.
 */

/**
 * @addtogroup CAPI_SYSTEM_GPIO_MODULE
 * @{
 */

#define GPIO_IN     0
#define GPIO_OUT    1
#define LOW         0
#define HIGH        1

#define GPIO0   0x13400000
#define GPIO3   0x14010000

typedef enum {
  GPX0 = 0x300,
  GPX1 = 0x308,
  GPA0 = 0x00,
  GPA1 = 0x08,
  GPA2 = 0x10,
  GPD0 = 0x28,
  GPB2 = 0x28
} gpio_port;

typedef enum {
  GPX0_0 = (GPX0 << 3) + 0,
  GPX0_1 = (GPX0 << 3) + 1,
  /* More to be added */
  GPX1_0 = (GPX1 << 3) + 0,
  GPX1_5 = (GPX1 << 3) + 5,
  GPX1_6 = (GPX1 << 3) + 6,
  J27_11 = GPX1_0,
  J27_12 = GPX1_5,
  J27_13 = GPX1_6
} gpio_pin;


int8_t set_pin_mode (gpio_pin pin, uint8_t mode);
int8_t get_pin_mode(gpio_pin pin);
int8_t get_pin_value(gpio_pin pin);
int8_t set_pin_value(gpio_pin pin, uint8_t value);

#ifdef __cplusplus
}
#endif


#endif //__TIZEN_GPIO_H__
