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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <limits.h>
#include <unistd.h>
#include <thread>
#include <chrono>

#include "gpio.h"
#include "gpio_private.h"
#include <libgen.h>
#include <memory>
#include "gpio_log.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <sys/types.h>

#include <map>

using std::map;

#define GPIO0   0x13400000
#define GPIO3   0x14010000

#define GET_PORT(pin) ((pin) >> 3)
#define GET_OFFSET(pin) ((pin) & 7)

#define RETURN_VAL_IF(expr, err) \
	do { \
		if (expr) { \
			_E_MSG(err); \
			return (err); \
		} \
	} while (0)

#define RETURN_ERROR(err) \
	do { \
		_E_MSG(err); \
		return (err); \
	} while (0)

#define GPIO_SHIFT_TYPE 16
#define GPIO_UNDEFINED_ID -1

#define GPIO_BATCH_LATENCY_DEFAULT UINT_MAX

#define GPIO_LISTENER_MAGIC 0xCAFECAFE

#define CONVERT_AXIS_ENUM(X) ((X) < 3 ? (X) + 0x81 : (X) - 2)

#define CONVERT_OPTION_PAUSE_POLICY(option) ((option) ^ 0b11)

#define WARN_DEPRECATED_GPIO(X) \
	do { \
		if ((X) == GPIO_LAST || (X) == GPIO_CUSTOM) { \
			_W("DEPRECATION WARNING: This gpio pin is deprecated and will be removed from next release."); \
		} \
	} while (0)

static uint8_t gpio_isinit = 0;

static int msg_queue_id;
static pid_t p_num;

map<gpio_pin_e, gpio_listener_h> listener_map;

map<gpio_pin_e, gpio_direction_e> gpio_direction;
map<gpio_pin_e, gpio_value_e> gpio_value;


int gpio_inject_data(gpio_pin_e pin, gpio_value_e value) {
  gpio_value[pin] = value;
  return 0;
}

static uint8_t msg_count=0;

void msg_create(msg_data *data, gpio_msg_e msg_type, gpio_pin_e pin, uint8_t value=0) {
    data->data_type = 1;
    data->data_num = p_num;
    memset(data->data_buff, 0, BUFF_SIZE);
    data->data_buff[0] = (char)msg_type;
    data->data_buff[1] = (char)value;
    data->data_buff[2] = ++msg_count;
    *(uint32_t *)(data->data_buff+4) = (uint32_t)pin;
}

inline gpio_msg_e msg_get_type(const msg_data data) {
    return (gpio_msg_e)data.data_buff[0];
}

inline gpio_pin_e msg_get_pin(const msg_data data) {
    return (gpio_pin_e)*(uint32_t *)(data.data_buff+4);
}

inline gpio_value_e msg_get_value(const msg_data data) {
    return (gpio_value_e)data.data_buff[1];
}

inline gpio_direction_e msg_get_direction(const msg_data data) {
    return (gpio_direction_e)data.data_buff[1];
}

inline int8_t msg_get_return (msg_data data) {
    return data.data_buff[2];
}


int send_message(const void *msgp) {
    if (-1 == msgsnd(msg_queue_id, msgp, sizeof(msg_data) - sizeof(long), 0)) {
        perror("msgsnd() failed.");
        return -1;
    }
    return 0;
}

int send_debug_message(const char* msg) {
	msg_data debug_msg;
	debug_msg.data_type = 1;
	debug_msg.data_num = p_num;
	debug_msg.data_buff[0] = 10;
	strcpy(debug_msg.data_buff+8, msg);
	return send_message(&debug_msg);
}

static void message_listener() {
    msg_data data;
    while(1) {
        if (-1 == msgrcv(msg_queue_id, &data, sizeof(msg_data) - sizeof(long), p_num, 0)) {
            perror("msgrcv() failed.");
            exit(1);
        }
        gpio_pin_e pin = msg_get_pin(data);
        gpio_msg_e msg_type = msg_get_type(data);
        switch(msg_type) {
        case GPIO_OPEN_PIN:
            break;

        case GPIO_CLOSE_PIN:
            break;

        case GPIO_SET_DIRECTION:
            if (listener_map.find(pin) != listener_map.end() && listener_map[pin]) {
                if (msg_get_return(data) != -1) listener_map[pin]->direction = msg_get_direction(data);
            }
            break;

        case GPIO_GET_DIRECTION: //Should not be called
            if (listener_map.find(pin) != listener_map.end() && listener_map[pin]) {
                 if (msg_get_return(data) != -1) listener_map[pin]->direction = msg_get_direction(data);
            }
            break;

        case GPIO_SET_VALUE:
            if (listener_map.find(pin) != listener_map.end() && listener_map[pin]) {
                if (msg_get_return(data) != -1) listener_map[pin]->data = msg_get_value(data);
            }
            break;

        case GPIO_GET_VALUE:
            if (listener_map.find(pin) != listener_map.end() && listener_map[pin]) {
				if (msg_get_return(data) == -1) break;
                gpio_listener_h listener = listener_map[pin];
                int prev_value = listener->data;
                listener->data = msg_get_value(data);

		        if (prev_value != listener->data && listener->callback) {
			        gpio_event_s *event = new gpio_event_s;
			        event->timestamp =
				        std::chrono::duration_cast<std::chrono::milliseconds>(
					        std::chrono::system_clock::now().time_since_epoch()).count();
			        event->value = listener->data;

			        send_debug_message("Callback fired");
			        gpio_event_cb
			        (*listener->callback)(
					        listener->gpio,
					        event,
					        listener->user_data);
			        delete event;
		        }
            }
            break;
        case 10:
          printf("Got debug message\n");
          break;
        default:
            perror("undefined message");
            break;
        }
    }
}

static int init_gpio() {
    if (-1 == (msg_queue_id = msgget((key_t)913, IPC_CREAT | 0666))) {
        perror("msgget() failed.");
        return -1;
    }
    gpio_isinit = 1;
    p_num = getpid();
    std::thread message_thread(message_listener);
    message_thread.detach();
    return 0;
}


static int set_pin_mode(gpio_pin_e pin, gpio_direction_e mode) {
    msg_data data;
    msg_create(&data, GPIO_SET_DIRECTION, pin, mode);
    return send_message(&data);
}

static int request_pin_mode(gpio_pin_e pin) {
    msg_data data;
    msg_create(&data, GPIO_GET_DIRECTION, pin);
    return send_message(&data);
}

static int8_t request_pin_value(gpio_pin_e pin) {
    msg_data data;
    msg_create(&data, GPIO_GET_VALUE, pin);
    return send_message(&data);
}


static int set_pin_value(gpio_pin_e pin, gpio_value_e value) {
    msg_data data;
    msg_create(&data, GPIO_SET_VALUE, pin, value);
    return send_message(&data);
}

static void gpio_data_listener(gpio_listener_h listener) {
	while(listener->active) {
		std::this_thread::sleep_for(
			std::chrono::nanoseconds(listener->batch_latency*1000*1000));
		if (request_pin_value(listener->pin) < 0) {
			_D("PIN ERROR");
			return;
		}
	}
}

//finished
static int gpio_connect(gpio_h gpio, gpio_listener_h listener)
{
	gpio_pin_e pin = (gpio_pin_e)gpio->pin;
	int id = pin;

	if (!listener)
		return GPIO_ERROR_INVALID_PARAMETER;

	_D("called gpio_connect : listener[0x%x], gpio[0x%x]", listener, gpio);

	if (listener_map.find(pin) != listener_map.end() && listener_map[pin]) {
		_D("Listener associated with pin %d is destroyed\n", pin);
		gpio_destroy_listener(listener_map[pin]);
	}

	listener_map[pin] = listener;
	listener->id = id;
	listener->pin = pin;
	listener->direction = gpio->direction;

	_D("success gpio_connect: id[%d]", id);

	return id;
}

int gpio_is_supported(gpio_pin_e pin, bool *supported)
{
	//TODO: gpio port check
	return GPIO_ERROR_NONE;
}

//finished
int gpio_get_default_gpio(gpio_pin_e pin, gpio_h *gpio, gpio_direction_e direction)
{
	if (!gpio_isinit) init_gpio();
	gpio_t *_gpio;
	
	_D("called gpio_get_default_gpio : pin[%d], gpio[0x%x]", pin, gpio);

	if (set_pin_mode(pin, direction) != 0)
		return GPIO_ERROR_INVALID_PARAMETER;
	if(!gpio)
		return GPIO_ERROR_INVALID_PARAMETER;

	_gpio = new(std::nothrow) gpio_t;

	if (!_gpio)
		return GPIO_ERROR_OUT_OF_MEMORY;

	_gpio->pin = pin;
	_gpio->direction = direction;

	*gpio = (gpio_h) _gpio;
	_D("success gpio_get_default_gpio gpio[0x%x]", _gpio);

	return GPIO_ERROR_NONE;
}

int gpio_get_gpio_list(gpio_pin_e pin, gpio_h **list, int *gpio_count)
{
	return GPIO_ERROR_NOT_SUPPORTED;
}

int gpio_is_wake_up(gpio_h gpio, bool *wakeup)
{
	return GPIO_ERROR_NONE;
}

//finished
int gpio_create_listener(gpio_h gpio, gpio_listener_h *listener)
{
	struct gpio_listener_s *_listener;
	int error;

	_D("called gpio_create_listener : listener[0x%x]", listener);

	if (!gpio || !listener)
		return GPIO_ERROR_INVALID_PARAMETER;

	_listener = new(std::nothrow) struct gpio_listener_s;

	if (!_listener)
		return GPIO_ERROR_OUT_OF_MEMORY;

	error = gpio_connect(gpio, _listener);

	if (error < 0) {
		delete (struct gpio_listener_s *)_listener;
		return GPIO_ERROR_IO_ERROR;
	}

	_listener->gpio = gpio;
	_listener->pause = GPIO_PAUSE_ALL;
	_listener->batch_latency = GPIO_BATCH_LATENCY_DEFAULT;
	_listener->magic = GPIO_LISTENER_MAGIC;

	*listener = (gpio_listener_h) _listener;

	_D("success gpio_create_listener");

	return GPIO_ERROR_NONE;
}

int gpio_destroy_listener(gpio_listener_h listener)
{
	_D("called gpio_destroy : listener[0x%x]", listener);

	if (!listener)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->magic != GPIO_LISTENER_MAGIC)
		return GPIO_ERROR_INVALID_PARAMETER;

	gpio_listener_stop(listener);
	//wait for batch latency+10ms for thread to finish
	//TODO: replace this with safer code e.g. using conditional variable
	std::this_thread::sleep_for(
			std::chrono::nanoseconds((listener->batch_latency+10)*1000*1000));
	
	listener_map[listener->pin] = NULL;

	listener->magic = 0;

	delete (struct gpio_listener_s *)listener;

	_D("success gpio_destroy");

	return GPIO_ERROR_NONE;
}

int gpio_listener_start(gpio_listener_h listener)
{
	_D("called gpio_listener_start : listener[0x%x]", listener);

	if (!listener)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->magic != GPIO_LISTENER_MAGIC)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->direction == GPIO_IN) {
		listener->active = true;
		listener->read_thread = new std::thread(gpio_data_listener, listener);
		listener->read_thread->detach();
	}

	_D("success gpio_listener_start : pin[%x]", listener->pin);

	return GPIO_ERROR_NONE;
}

int gpio_listener_stop(gpio_listener_h listener)
{
	_D("called gpio_listener_stop : listener[0x%x]", listener);

	if (!listener)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->magic != GPIO_LISTENER_MAGIC)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->direction == GPIO_IN) {
		listener->active = false;
	}

	_D("success gpio_listener_stop");

	return GPIO_ERROR_NONE;
}

int gpio_listener_set_event_cb(gpio_listener_h listener,
		unsigned int interval, gpio_event_cb callback, void *user_data)
{
	if (!listener || !callback)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->magic != GPIO_LISTENER_MAGIC)
		return GPIO_ERROR_INVALID_PARAMETER;

	listener->batch_latency = interval;
	listener->callback = callback;
	listener->user_data = user_data;

	_D("success gpio_listener_set_event");

	return GPIO_ERROR_NONE;
}

int gpio_listener_unset_event_cb(gpio_listener_h listener)
{
	_D("called gpio_unregister_event : listener[0x%x]", listener);

	if (!listener)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->magic != GPIO_LISTENER_MAGIC)
		return GPIO_ERROR_INVALID_PARAMETER;

	listener->callback = NULL;
	listener->user_data = NULL;

	_D("success gpio_unregister_event");

	return GPIO_ERROR_NONE;
}

int gpio_listener_set_interval(gpio_listener_h listener, unsigned int interval)
{
	_D("called gpio_set_interval : listener[0x%x], interval[%d]", listener, interval);

	if (!listener)
		return GPIO_ERROR_INVALID_PARAMETER;

	if (listener->magic != GPIO_LISTENER_MAGIC)
		return GPIO_ERROR_INVALID_PARAMETER;

	listener->batch_latency = interval;

	_D("success gpio_set_interval");

	return GPIO_ERROR_NONE;
}

int gpio_listener_set_max_batch_latency(gpio_listener_h listener, unsigned int max_batch_latency)
{
	return gpio_listener_set_interval(listener, max_batch_latency);
}


int gpio_listener_set_data(gpio_listener_h listener, gpio_value_e data)
{
	_D("called gpio_set_data : listener[0x%x], data[%d]", listener, data);

	if (listener->direction != GPIO_OUT) {
		return GPIO_ERROR_INVALID_PARAMETER;
	}
	set_pin_value(listener->pin, data);

	_D("success sensor_set_data");

	return GPIO_ERROR_NONE;
}

int gpio_listener_read_data(gpio_listener_h listener, gpio_event_s *event)
{
	_D("called gpio_read_data : listener[0x%x]", listener);

	event->value = (gpio_value_e)listener->data;
	event->timestamp =
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	_D("success gpio_read_data");

	return GPIO_ERROR_NONE;
}

const char * __gpio_get_name(gpio_pin_e pin) {
	switch(pin) {
		case GPX0_0: return "GPX0_0";
		case GPX0_1: return "GPX0_1";
		case GPX1_0: return "GPX1_0";
		case GPX1_5: return "GPX1_5";
		case GPX1_6: return "GPX1_6";
		default: return "UNDEFINED";
	}
}
const char * __gpio_get_pin(gpio_pin_e pin) {
	switch(pin) {
		case J27_11: return "J27_11";
		case J27_12: return "J27_12";
		case J27_13: return "J27_13";

		default: return "UNDEFINED";
	}
}

int gpio_get_name(gpio_h gpio, char** name)
{
	_D("called gpio_get_name");

	if (!gpio || !name)
		return GPIO_ERROR_INVALID_PARAMETER;

	*name = strdup(__gpio_get_name(gpio->pin));

	_D("success gpio_get_name : [%s]", *name);

	return GPIO_ERROR_NONE;
}

int gpio_get_pin(gpio_h gpio, gpio_pin_e *pin)
{
	_D("called gpio_get_pin");

	if (!gpio || !pin)
		return GPIO_ERROR_INVALID_PARAMETER;

	*pin = (gpio_pin_e) gpio->pin;

	_D("success gpio_get_pin : [%d]", *pin);

	return GPIO_ERROR_NONE;
}
