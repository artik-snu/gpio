/*
 * memory mapped gpio code for ARTIK 10
 * Source code adapted from http://odroid.com/dokuwiki/doku.php?id=en:xu3_gpio_register
 * modified by Taehee Jeong 13-10-2016
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include "gpio.h"

int main(int argc, char **argv)
{
    gpio_pin pin = J27_13;
    set_pin_mode(pin, GPIO_IN);
    int i;
    int val = get_pin_value(pin);
    while(1) {
        nanosleep(10000000);
        //sleep(1);
        //printf("val: %d\n", val);
        int newval = get_pin_value(pin);
        if (newval != val) {
            val = newval;
            printf("Value changed to %s\n", val?"HIGH":"LOW");
        }
    }
    return 0;
}