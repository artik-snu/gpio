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
    set_pin_mode(pin, GPIO_OUT);
    int i;
    for (i = 0; i < 10; i++) {
        set_pin_value(pin, HIGH);
        sleep(1);
        set_pin_value(pin, LOW);
        sleep(1);
    }

    return 0;
}