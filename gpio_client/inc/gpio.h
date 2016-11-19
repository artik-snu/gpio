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

#ifndef __GPIO_H__
#define __GPIO_H__

#include <tizen.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_SYSTEM_GPIO_MODULE
 * @{
 */

/**
 * @brief   GPIO handle.
 * @details The handle for controlling a specific gpio can be retrieved using gpio_get_default_gpio().@n
 *          The function returns the handle of the default gpio of a given type, and usually,
 *          a device has one gpio for one type.
 *          However, if the device supports multiple gpios of the same type,
 *          gpio_get_gpio_list() function can be used to get the list of all the gpios of the type.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */


EXPORT_API int send_debug_message(const char* msg);
/**
 * @brief   Enumeration for errors.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    GPIO_ERROR_NONE                  = TIZEN_ERROR_NONE,                 /**< Successful */
    GPIO_ERROR_IO_ERROR              = TIZEN_ERROR_IO_ERROR,             /**< I/O error */
    GPIO_ERROR_INVALID_PARAMETER     = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
    GPIO_ERROR_NOT_SUPPORTED         = TIZEN_ERROR_NOT_SUPPORTED,        /**< Not supported */
    GPIO_ERROR_PERMISSION_DENIED     = TIZEN_ERROR_PERMISSION_DENIED,    /**< Permission denied */
    GPIO_ERROR_OUT_OF_MEMORY         = TIZEN_ERROR_OUT_OF_MEMORY,        /**< Out of memory */
    GPIO_ERROR_NO_DATA               = TIZEN_ERROR_NO_DATA,              /**< No data available
                                                                                @if MOBILE (Since 3.0) @elseif WEARABLE (Since 2.3.2) @endif */
 } gpio_error_e;

typedef enum {
    GPIO_IN = 0,
    GPIO_OUT = 1
} gpio_direction_e;

typedef enum {
    LOW = 0,
    HIGH = 1
} gpio_value_e;

#define BUFF_SIZE 128

typedef struct {
  long data_type;
  long data_num;
  char data_buff[BUFF_SIZE];
} msg_data;

typedef enum {
    GPIO_OPEN_PIN,
    GPIO_CLOSE_PIN,
    GPIO_SET_DIRECTION,
    GPIO_GET_DIRECTION,
    GPIO_SET_VALUE,
    GPIO_GET_VALUE
} gpio_msg_e;

/**
 * @brief   Enumeration for gpio types.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
  GPX0 = 0x300,
  GPX1 = 0x308,
  GPA0 = 0x00,
  GPA1 = 0x08,
  GPA2 = 0x10,
  GPD0 = 0x28,
  GPB2 = 0x28
} gpio_port_e;

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
} gpio_pin_e;


typedef struct {
  gpio_pin_e pin;
  gpio_direction_e direction;
} gpio_t;

typedef gpio_t* gpio_h;


#define _DEBUG 1
#ifdef _DEBUG
int gpio_inject_data(gpio_pin_e pin, gpio_value_e value);
#endif

/**
 * @brief   Checks whether a given gpio type is supported in the current device.
 * @details If the given gpio type is not supported, gpio_get_default_gpio() will return an error.
 *          It is thus recommended to check the availability of the gpio before actually acquiring #gpio_h.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   type        A gpio type to check
 * @param[out]  supported   If supported, @c true; Otherwise @c false
 *
 * @return  #GPIO_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 */
EXPORT_API int gpio_is_supported(gpio_pin_e pin, bool *supported);

/**
 * @brief   Gets the handle for the default gpio of a given type.
 * @details This function returns the handle for the gpio of a given type,
 *          if the device has one gpio of the given type.
 *          In case that the device has more than one gpios of the type,
 *          this returns only the default gpio, which is designated by the device.@n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Some gpio types are privileged. An application should have the privilege
 *          http://tizen.org/privilege/healthinfo to get handles for the following gpios:
 *          #GPIO_HRM, #GPIO_HRM_LED_GREEN, #GPIO_HRM_LED_IR, #GPIO_HRM_LED_RED,
 *          #GPIO_HUMAN_PEDOMETER, #GPIO_HUMAN_SLEEP_MONITOR, #GPIO_HUMAN_SLEEP_DETECTOR,
 *          and #GPIO_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type     A gpio type to get the handle of its default gpio
 * @param[out] gpio   The gpio handle of the default gpio
 *
 * @return  #GPIO_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_NOT_SUPPORTED        The gpio type is not supported in the current device
 * @retval  #GPIO_ERROR_PERMISSION_DENIED    Permission denied
 *
 * @see     gpio_get_gpio_list()
 */
EXPORT_API int gpio_get_default_gpio(gpio_pin_e pin, gpio_h *gpio, gpio_direction_e direction);


/**
 * @brief   Gets the handle list of the gpios of a given type.
 * @details A device may have more than one gpios of the given type.
 *          In such case, this function can be used to get the handles of all gpios of the type.@n
 *          The first element of the @c list denotes the default gpio,
 *          which can be retrieved by gpio_get_default_gpio().
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Some gpio types are privileged. An application should have the privilege
 *          http://tizen.org/privilege/healthinfo to get handles for the following gpios:
 *          #GPIO_HRM, #GPIO_HRM_LED_GREEN, #GPIO_HRM_LED_IR, #GPIO_HRM_LED_RED,
 *          #GPIO_HUMAN_PEDOMETER, #GPIO_HUMAN_SLEEP_MONITOR, #GPIO_HUMAN_SLEEP_DETECTOR,
 *          and #GPIO_HUMAN_STRESS_MONITOR.@n
 *          Instead of specifying a gpio type, by using #GPIO_ALL,
 *          applications can get the list of handles for all available gpios.@n
 *          The @c list must be released using @c free(), if not being used anymore.@n
 *
 * @param[in]  type         A gpio type to get the list of gpio handles
 * @param[out] list         An array of the gpio handles
 * @param[out] gpio_count The number of handles contained in @c list
 *
 * @return  #GPIO_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_NOT_SUPPORTED        The gpio type is not supported in the current device
 * @retval  #GPIO_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #GPIO_ERROR_OUT_OF_MEMORY        Out of memory
 */
EXPORT_API int gpio_get_gpio_list(gpio_pin_e pin, gpio_h **list, int *gpio_count);


/**
 * @brief   Gets the name of a gpio.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   gpio  A gpio handle
 * @param[out]  name    The name of the gpio
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 */
EXPORT_API int gpio_get_name(gpio_h gpio, char **name);

EXPORT_API int gpio_get_pin(gpio_h gpio, gpio_pin_e *pin);
/**
 * @brief   Gets the possible shorted update interval of a gpio.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   gpio          A gpio handle
 * @param[out]  min_interval    The shorted interval in milliseconds
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 */
EXPORT_API int gpio_get_min_interval(gpio_h gpio, int *min_interval);


/**
 * @brief   Gets the maximum batch count of a gpio.
 * @details This function returns the maximum number of gpio data events
 *          that can be possibly delivered when the batched data are flushed.
 *          Therefore, this count can be used to check whether the gpio supports
 *          batching or not.@n
 *          If this returns a positive count, i.e., the gpio supports batching,
 *          the count also can be used to guess the possible longest batch latency
 *          of the gpio, with respect to the update interval to use.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   gpio          A gpio handle
 * @param[out]  max_batch_count If the gpio does not support batching, 0;
 *                              Otherwise a positive integer.
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     gpio_listener_set_max_batch_latency()
 */
EXPORT_API int gpio_get_max_batch_count(gpio_h gpio, int *max_batch_count);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_GPIO_LISTENER_MODULE
 * @{
 */

/**
 * @brief   The upper bound of #gpio_event_s::value_count.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define MAX_VALUE_SIZE 16

/**
 * @brief   GPIO listener handle.
 * @details For each #gpio_h, one or more gpio listeners can be created by using gpio_create_listener().
 *          Then the gpio's data can observed asynchronously, can be read synchronously if available, via the listener.
 *          Applications are also able to control the behavior of each gpio, for example,
 *          update interval of gpio readings.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct gpio_listener_s *gpio_listener_h;


/**
 * @brief   GPIO data event delivered via gpio_event_cb().
 * @details A gpio data is delivered as a structure, which contains the accuracy of the data,
 *          the time when the data was observed, and the data array.
 *          The data array is a fixed size @c float array, and the number of data fields
 *          stored in the array varies with the gpio type.
 *          For example, #GPIO_ACCELEROMETER reports 3-dimensional data,
 *          #gpio_event_s::value_count is thus set to 3.@n
 *          Note that, even if the data values are @c float, in some cases,
 *          it may contain one or more categorical data as in #gpio_proximity_e.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @see     #gpio_pedometer_state_e
 * @see     #gpio_sleep_state_e
 */
typedef struct
{
    unsigned long long timestamp;  /**< Time when the gpio data was observed */
    gpio_value_e value;  /**< GPIO data values */
} gpio_event_s;


/**
 * @brief   Enumeration for gpio data accuracy.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    GPIO_DATA_ACCURACY_UNDEFINED   = -1,  /**< Undefined */
    GPIO_DATA_ACCURACY_BAD         = 0,   /**< Not accurate */
    GPIO_DATA_ACCURACY_NORMAL      = 1,   /**< Moderately accurate */
    GPIO_DATA_ACCURACY_GOOD        = 2,   /**< Highly accurate */
    GPIO_DATA_ACCURACY_VERYGOOD    = 3    /**< Very highly accurate */
} gpio_data_accuracy_e;


/**
 * @brief   Enumeration for gpio listener behavior attributes
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum
{
    GPIO_ATTRIBUTE_AXIS_ORIENTATION = 1,  /**< Reference orientation of gpio data to be reported.@n
                                                 See #gpio_axis_e for available attribute values. */
    GPIO_ATTRIBUTE_PAUSE_POLICY,          /**< Pause-and-resume policy of gpios.@n
                                                 See #gpio_pause_e for available attribute values. */
} gpio_attribute_e;


/**
 * @brief   Enumeration for gpio options.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#ifndef __GPIO_COMMON_H__


/**
 * @brief   Enumeration for pause policies of gpio listeners
 * @details To be power-efficient, you can set the policy of how to pause and resume
 *          a gpio listener regarding the system status.
 *          By default, #GPIO_PAUSE_ALL is used to obtain the maximum power efficiency.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum
{
    GPIO_PAUSE_NONE = 0,              /**< The gpio will not pause, unless the system goes into sleep mode */
    GPIO_PAUSE_ON_DISPLAY_OFF = 1,    /**< The gpio pauses while the display is off*/
    GPIO_PAUSE_ON_POWERSAVE_MODE = 2, /**< The gpio pauses while the power-save mode is enabled */
    GPIO_PAUSE_ALL = 3,               /**< The gpio pauses in all the above cases */
} gpio_pause_e;


/**
 * @brief   Called when a gpio event occurs.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] gpio    The corresponding gpio handle
 * @param[in] event     A gpio event
 * @param[in] data      The user data had passed to gpio_listener_set_event_cb()
 *
 * @pre     The gpio needs to be started regarding a listener handle, using gpio_listener_start().
 */
typedef void (*gpio_event_cb)(gpio_h gpio, gpio_event_s *event, void *data);


/**
 * @brief   Creates a gpio listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks The @c listener must be released using gpio_destroy_listener().
 *
 * @param[in]  gpio          A gpio handle
 * @param[out] listener        A listener handle of @c gpio
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The handle @c gpio needs to be initialized using
 *          gpio_get_default_gpio() or gpio_get_gpio_list() in advance.
 */
EXPORT_API int gpio_create_listener(gpio_h gpio, gpio_listener_h *listener);


/**
 * @brief   Releases all the resources allocated for a listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks If this function is called while the gpio is still running,
 *          that is, gpio_listener_start() was called but gpio_listener_stop() was not,
 *          then it is implicitly stopped.
 *
 * @param[in] listener  A listener handle
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see     gpio_create_listener()
 */
EXPORT_API int gpio_destroy_listener(gpio_listener_h listener);


/**
 * @brief   Starts observing the gpio events regarding a given gpio listener.
 * @details If a gpio listener is started, its event callback function starts to be called
 *          whenever the corresponding gpio events occur.@n
 *          For example, #GPIO_ACCELEROMETER reports its gpio readings repeatedly,
 *          with a specific update interval.
 *          Note that, unlike the accelerometer, gpios like #GPIO_PROXIMITY emit events
 *          only if their states change.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener  A listener handle
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The @c listener needs to be created in advance, by using gpio_create_listener().
 *          Then the callback function needs to be attached to the @c listener, by using
 *          gpio_listener_set_event_cb().
 * @see     gpio_listener_stop()
 */
EXPORT_API int gpio_listener_start(gpio_listener_h listener);


/**
 * @brief   Stops observing the gpio events regarding a given gpio listener.
 * @details The listener's event callback function stops being called.
 *          But the gpio itself may not be stopped if there are other listeners
 *          that are using the same gpio.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener  A listener handle
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     gpio_listener_start()
 */
EXPORT_API int gpio_listener_stop(gpio_listener_h listener);

/**
 * @brief   Registers the callback function to be invoked when gpio events are delivered via a gpio listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener    A listener handle
 * @param[in]   interval_ms A desired update interval between gpio events in milliseconds.@n
 *                          If 0, it will be automatically set to the default interval of the corresponding gpio.@n
 *                          See gpio_listener_set_interval() for more details.
 * @param[in]   callback    A callback function to attach with the @c listener handle
 * @param[in]   data        A user data to be passed to the callback function
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see gpio_listener_unset_event_cb()
 */
EXPORT_API int gpio_listener_set_event_cb(gpio_listener_h listener, unsigned int interval_ms, gpio_event_cb callback, void *data);

/**
 * @brief   Unregisters the gpio event callback function attached to a given gpio listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener    A listener handle
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     gpio_listener_set_event_cb()
 */
EXPORT_API int gpio_listener_unset_event_cb(gpio_listener_h listener);

/**
 * @brief   Reads the current gpio data via a given gpio listener.
 * @details This function synchronously reads the gpio reading of the corresponding gpio, if available.
 *          Otherwise, if the gpio is not ready to report its values, this function fails and returns
 *          #GPIO_ERROR_OPERATION_FAILED.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remark  As a gpio usually works in an event-driven manner, it may not be able to read its data on demand.
 *          Then this function tries to return the last known values.@n
 *          To be able to read the current values or the last known values, the gpio needs to be
 *          enabled in advance. As an application cannot be sure that the gpio is already enabled
 *          by other applications, it is recommended to start the gpio explicitly by using gpio_listener_start().@n
 *          But note that, even if the gpio is started, on-demand reading can fail always,
 *          thus it would be better to use the asynchronous callback approach.
 *
 * @param[in]   listener    A listener handle
 * @param[out]  event       The retrieved gpio data
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 */
EXPORT_API int gpio_listener_read_data(gpio_listener_h listener, gpio_event_s *event);

/**
 * @brief   Changes the update interval of a gpio.
 * @details The specified interval is only a suggested interval between gpio measurements.
 *          You will get at least one gpio measurement within the interval you specify,
 *          but the actual interval between gpio measurements can be affected by other applications and the system.
 *          To reduce the system overhead, it is recommended to set the longest interval that you can,
 *          because the system usually chooses the shortest interval among all intervals specified.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Normally, a gpio's default update interval is 100 ms,
 *          and you can use the default interval by setting the interval to 0.
 *          However, please note that, the default interval varies with the gpio and the device.@n
 *          In addition, a gpio has the lower and the upper bound of its update interval,
 *          usually 10 and 1000 ms respectively.
 *          These lower and upper bounds also can vary with the gpio and the device,
 *          any invalid input values exceeding the bounds will be implicitly adjusted into the valid range.
 *
 * @param[in]   listener    A listener handle
 * @param[in]   interval_ms A desired update interval between gpio events in milliseconds.
 *                          If 0, it will be automatically set to the default interval of the corresponding gpio.
 *
 * @return  #GPIO_ERROR_NONE on success, otherwise a negative error value
 * @retval  #GPIO_ERROR_NONE                 Successful
 * @retval  #GPIO_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #GPIO_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     gpio_get_min_interval()
 */
EXPORT_API int gpio_listener_set_interval(gpio_listener_h listener, unsigned int interval_ms);

EXPORT_API int gpio_listener_set_data(gpio_listener_h listener, gpio_value_e data);

#endif
/**
 * @}
 */




#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H__ */

