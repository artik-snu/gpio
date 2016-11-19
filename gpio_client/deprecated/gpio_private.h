/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <thread>
#include <atomic>

#include "gpio.h"

#ifndef __GPIO_PRIVATE_H__
#define __GPIO_PRIVATE_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct gpio_listener_s {
	int id;
	gpio_pin_e pin;
	gpio_direction_e direction;
	gpio_value_e data;
	int pause;
	std::atomic<unsigned int> batch_latency;
	unsigned int magic;
	std::thread *read_thread;
	std::atomic<bool> active; //if listener is started
	gpio_h gpio;
	gpio_event_cb callback;
	void *user_data;
	void *accu_callback;
	void *accu_user_data;
};

#ifdef __cplusplus
}
#endif

#endif // __GPIO_PRIVATE_H__
