/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <limits.h>
#include <math.h>

#include <thread>
#include <chrono>
#include <map>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "gpio.h"

using std::map;

#define GPIO0   0x13400000
#define GPIO3   0x14010000

#define GET_PORT(pin) ((pin) >> 3)
#define GET_OFFSET(pin) ((pin) & 7)

static uint8_t gpio_isinit = 0;
static volatile uint32_t *gpio_base[2];

map<gpio_pin_e, int> port_used;

#ifdef GPIO_DUMMY
map<gpio_pin_e, gpio_direction_e> gpio_direction;
map<gpio_pin_e, gpio_value_e> gpio_value;
#endif

int msg_queue_id;
msg_data data;

/* Data structure for message buffer
 * data_type: target process ID (1 for service, pid for other)
 * data_num: sender ID (1 for service, pid for other)
 * data_buff[0]: message ID
 * data_buff[1]: message parameter (value/direction for SET, undefined otherwise)
 * data_buff[2]: return value (-1 for error, 0 for OK, 0/1 for GET)
 * (uint32_t*)(data_buff + 4): offset for pin
 */

void msg_create(msg_data &data, long target, gpio_msg_e msg_type, gpio_pin_e pin, int return_value, int value=0) {
    data.data_type=target;
    data.data_num = 1;
    data.data_buff[0] = (char)msg_type;
    data.data_buff[1] = (char)value;
    data.data_buff[2] = (char)return_value;
    *(uint32_t *)(data.data_buff+4) = (uint32_t)pin;
}

inline gpio_msg_e msg_get_type(const msg_data &data) {
    return (gpio_msg_e)data.data_buff[0];
}

inline gpio_pin_e msg_get_pin(const msg_data &data) {
    return (gpio_pin_e)*(uint32_t *)(data.data_buff+4);
}

inline gpio_value_e msg_get_value(const msg_data &data) {
    return (gpio_value_e)data.data_buff[1];
}

inline gpio_direction_e msg_get_direction(const msg_data &data) {
    return (gpio_direction_e)data.data_buff[1];
}

inline void msg_set_return (msg_data &data, char value) {
    data.data_buff[2] = value;
}


void send_message(const void *msgp) {
    if (-1 == msgsnd(msg_queue_id, msgp, sizeof(msg_data) - sizeof(long), 0)) {
        perror("msgsnd() failed.");
        exit(1);
    }
    msg_data *data = (msg_data*)msgp;
    int msg_type = data->data_buff[0];
    int pin = data->data_buff[1];
    printf("\tMSG SENT. To: %ld, type=%d, port = 0x%x, pin=0x%x, bufs=[%d %d]\n", data->data_type, msg_type, GET_PORT(pin), GET_OFFSET(pin), data->data_buff[1], data->data_buff[2]);
    printf("\t Buffer: %08x %08x\n", *(uint32_t *)(data->data_buff), *(uint32_t *)(data->data_buff+4));
}


static int init_gpio() {
#ifndef GPIO_DUMMY
    int fd ;

    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        printf("Unable to open /dev/mem\n");
        return -1;
    }

    gpio_base[0] = (uint32_t*)mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                                GPIO0);
    if (gpio_base[0] < 0){
        printf("Mmap failed.\n");
        return -1;
    }

    gpio_base[1] = (uint32_t*)mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                                GPIO3);
    if (gpio_base[1] < 0){
        printf("Mmap failed.\n");
        return -1;
    }
#endif
    gpio_isinit = 1;
    return 0;
}

static int8_t set_pin_mode(gpio_pin_e pin, gpio_direction_e mode) {
    gpio_port_e port = (gpio_port_e)GET_PORT(pin);
    uint8_t offset = GET_OFFSET(pin) << 2;

    if (!gpio_isinit) init_gpio();
#ifndef GPIO_DUMMY
    volatile uint32_t *base = gpio_base[port > 0x100?0:1];
    if (mode) {
        printf("\nEXPECTED GPIO DIR REGISTER VALUE: 0x%08x\n", *(base+(port)) |= (1 << offset));
        *(base + port) |= (1 << offset);
    } else {
        printf("\nEXPECTED GPIO DIR REGISTER VALUE: 0x%08x\n", *(base+(port)) & ~(1 << offset));
        *(base + port) &= ~(1 << offset);
    }
    printf("\nGPIO DIR REGISTER VALUE: 0x%08x\n", *(base+(port)));
#else
    gpio_direction[pin] = mode;
#endif
    return 0;
}

static int8_t get_pin_mode(gpio_pin_e pin) {
    gpio_port_e port = (gpio_port_e)GET_PORT(pin);
    uint8_t offset = GET_OFFSET(pin) << 2;

    if (!gpio_isinit) {
        perror("GPIO is not initialized!!\n");
        return -1;
    }
#ifndef GPIO_DUMMY
    volatile uint32_t *base = gpio_base[port > 0x100?0:1];
    return !!(*(base + port) & (1 << offset));
#else
    return gpio_direction[pin];
#endif
}

static int8_t get_pin_value(gpio_pin_e pin) {
    gpio_port_e port = (gpio_port_e)GET_PORT(pin);
    uint8_t offset = GET_OFFSET(pin);

    if (!gpio_isinit) {
        perror("GPIO is not initialized!!\n");
        return -1;
    }
#ifndef GPIO_DUMMY
    volatile uint32_t *base = gpio_base[port > 0x100?0:1];
    return !!(*(base + (port + 1)) & (1 << offset));
#else
    return gpio_value[pin];
#endif
}


static int8_t set_pin_value(gpio_pin_e pin, gpio_value_e value) {
    if (get_pin_mode(pin) != GPIO_OUT) {
        perror("GPIO pin is not on write mode!\n");
        return -1;
    }
    gpio_port_e port = (gpio_port_e)GET_PORT(pin);
    uint8_t offset = GET_OFFSET(pin);

    if (!gpio_isinit) {
        perror("GPIO is not initialized!!\n");
        return -1;
    }
#ifndef GPIO_DUMMY
    volatile uint32_t *base = gpio_base[port > 0x100?0:1];

    if (value) {
        printf("\nEXPECTED GPIO REGISTER VALUE: 0x%08x\n", *(base+(port+1)) & ~(1 << offset));
        *(base + (port + 1)) &= ~(1 << offset);

    } else {
        printf("\nEXPECTED GPIO REGISTER VALUE: 0x%08x\n", *(base+(port+1)) | (1<<offset) );
        *(base + (port + 1)) |= (1 << offset);
    }
    printf("\nGPIO REGISTER VALUE: 0x%08x\n", *(base+(port+1)));
#else
    gpio_value[pin] = value;
#endif
    return 0;
}


int main() {
    init_gpio();
    
    if (-1 == (msg_queue_id = msgget((key_t)913, IPC_CREAT | 0666))) {
        perror("msgget() failed.");
        exit(1);
    }   

    msg_data return_data;
    while (1) {
        if (-1 == msgrcv(msg_queue_id, &data, sizeof(msg_data) - sizeof(long), 1, 0)) {
            perror("msgrcv() failed.");
            exit(1);
        }
        gpio_pin_e pin = msg_get_pin(data);
        gpio_msg_e msg_type = msg_get_type(data);
        printf("MSG received. from: %ld, type=%d, port = 0x%x, pin=0x%x, bufs=[%d %d]\n", data.data_num, msg_type, GET_PORT(pin), GET_OFFSET(pin), data.data_buff[1], data.data_buff[2]);
        printf(" Buffer: %08x %08x\n", *(uint32_t *)(data.data_buff), *(uint32_t *)(data.data_buff+4));
        int8_t res;
        switch(msg_type) {
        case GPIO_OPEN_PIN:
            if (!port_used[pin]) {
                //TODO: block port manipulation by other processes
                port_used[pin] = data.data_num;
                msg_create(data, data.data_num, msg_type, pin, 0);
            } else {
                msg_create(return_data, data.data_num, msg_type, pin, -1);
            }
            send_message(&return_data);
            break;

        case GPIO_CLOSE_PIN:
            if (port_used[pin]) {
                port_used[pin] = 0;
                msg_create(data, data.data_num, msg_type, pin, 0);
            } else {
                msg_create(return_data, data.data_num, msg_type, pin, -1);
            }
            send_message(&return_data);
            break;

        case GPIO_SET_DIRECTION:
            res = set_pin_mode(pin, msg_get_direction(data));
            msg_create(return_data, data.data_num, msg_type, pin, res, msg_get_direction(data));
            send_message(&return_data);
            break;

        case GPIO_GET_DIRECTION:
            res = get_pin_mode(pin);
            msg_create(return_data, data.data_num, msg_type, pin, res, res);
            send_message(&return_data);
            break;

        case GPIO_SET_VALUE:
            res = set_pin_value(pin, msg_get_value(data));
            msg_create(return_data, data.data_num, msg_type, pin, res, msg_get_value(data));
            send_message(&return_data);
            break;

        case GPIO_GET_VALUE:
            res = get_pin_value(pin);
            msg_create(return_data, data.data_num, msg_type, pin, res, res);
            send_message(&return_data);
            break;

        default:
            perror("undefined message");
            data.data_buff[BUFF_SIZE-1] = 0;
            printf("%s\n", data.data_buff+8);
            break;
        }
    }
}
