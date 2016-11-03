#include "gpio.h"

uint8_t gpio_isinit = 0;
static volatile uint32_t *gpio_base[2];

int init_gpio() {
    int fd ;

    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        printf("Unable to open /dev/mem\n");
        return -1;
    }

    gpio_base[0] = mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                                GPIO0);
    if (gpio_base[0] < 0){
        printf("Mmap failed.\n");
        return -1;
    }

    gpio_base[1] = mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                                GPIO3);
    if (gpio_base[1] < 0){
        printf("Mmap failed.\n");
        return -1;
    }
    gpio_isinit = 1;
    return 0;
}

int8_t set_pin_mode(gpio_pin pin, uint8_t mode) {
    gpio_port port = pin >> 3;
    uint8_t offset = pin & 7;

    if (!gpio_isinit) init_gpio();

    volatile uint32_t *base = gpio_base[port > 0x100?0:1];
    if (mode)
        *(base + port) |= (1 << offset);
    else
        *(base + port) &= ~(1 << offset);
    return 0;
}

int8_t get_pin_mode(gpio_pin pin) {
    gpio_port port = pin >> 3;
    uint8_t offset = pin & 7;

    if (!gpio_isinit) {
        printf("GPIO is not initialized!!\n");
        return -1;
    }

    volatile uint32_t *base = gpio_base[port > 0x100?0:1];
    return !!(*(base + port) & (1 << offset));
}

int8_t get_pin_value(gpio_pin pin) {
    gpio_port port = pin >> 3;
    uint8_t offset = pin & 7;

    if (!gpio_isinit) {
        printf("GPIO is not initialized!!\n");
        return -1;
    }

    volatile uint32_t *base = gpio_base[port > 0x100?0:1];
    return !!(*(base + (port + 1)) & (1 << offset));
}

int8_t set_pin_value(gpio_pin pin, uint8_t value) {
    if (get_pin_mode(pin) != GPIO_OUT) {
        printf("GPIO pin is not on write mode!\n");
        return -1;
    }
    gpio_port port = pin >> 3;
    uint8_t offset = pin & 7;

    if (!gpio_isinit) {
        printf("GPIO is not initialized!!\n");
        return -1;
    }

    volatile uint32_t *base = gpio_base[port > 0x100?0:1];

    if (value)
        *(base + (port + 1)) &= ~(1 << offset);
    else
        *(base + (port + 1)) |= (1 << offset);
    return 0;
}

void set_gpio_callback(gpio_pin pin, void* callback) {
    printf("Not Implemented");
}
