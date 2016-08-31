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

#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <tizen.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_MODULE
 * @{
 */

/**
 * @brief   Sensor handle.
 * @details The handle for controlling a specific sensor can be retrieved using sensor_get_default_sensor().@n
 *          The function returns the handle of the default sensor of a given type, and usually,
 *          a device has one sensor for one type.
 *          However, if the device supports multiple sensors of the same type,
 *          sensor_get_sensor_list() function can be used to get the list of all the sensors of the type.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef void* sensor_h;


/**
 * @brief   Enumeration for errors.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
	SENSOR_ERROR_NONE                  = TIZEN_ERROR_NONE,                 /**< Successful */
	SENSOR_ERROR_IO_ERROR              = TIZEN_ERROR_IO_ERROR,             /**< I/O error */
	SENSOR_ERROR_INVALID_PARAMETER     = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
	SENSOR_ERROR_NOT_SUPPORTED         = TIZEN_ERROR_NOT_SUPPORTED,        /**< Not supported */
	SENSOR_ERROR_PERMISSION_DENIED     = TIZEN_ERROR_PERMISSION_DENIED,    /**< Permission denied */
	SENSOR_ERROR_OUT_OF_MEMORY         = TIZEN_ERROR_OUT_OF_MEMORY,        /**< Out of memory */
	SENSOR_ERROR_NO_DATA               = TIZEN_ERROR_NO_DATA,              /**< No data available
                                                                                @if MOBILE (Since 3.0) @elseif WEARABLE (Since 2.3.2) @endif */
	SENSOR_ERROR_NOT_NEED_CALIBRATION  = TIZEN_ERROR_SENSOR | 0x03,        /**< Sensor doesn't need calibration */
	SENSOR_ERROR_OPERATION_FAILED      = TIZEN_ERROR_SENSOR | 0x06,        /**< Operation failed */
	SENSOR_ERROR_NOT_AVAILABLE         = TIZEN_ERROR_SENSOR | 0x07,        /**< The sensor is supported, but currently not available
                                                                                @if MOBILE (Since 3.0) @elseif WEARABLE (Since 2.3.2) @endif */
} sensor_error_e;


/**
 * @brief   Enumeration for proximity sensor events.
 * @details In its #sensor_event_s, #SENSOR_PROXIMITY reports the existence of
 *          nearby objects in front of the sensor as one of the followings.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
	SENSOR_PROXIMITY_NEAR = 0,    /**< An object is placed near the proximity sensor */
	SENSOR_PROXIMITY_FAR = 5,     /**< No object is placed near the proximity sensor */
} sensor_proximity_e;


/**
 * @brief   Enumeration for pedestrian state.
 * @details In its #sensor_event_s, #SENSOR_HUMAN_PEDOMETER reports the user's
 *          pedestrian state as one of the followings.
 * @since_tizen 3.0
 */
typedef enum
{
	SENSOR_PEDOMETER_STATE_UNKNOWN = -1, /**< Uncertain */
	SENSOR_PEDOMETER_STATE_STOP,         /**< The user is not moving */
	SENSOR_PEDOMETER_STATE_WALK,         /**< The user is walking */
	SENSOR_PEDOMETER_STATE_RUN,          /**< The user is running */
} sensor_pedometer_state_e;


/**
 * @brief   Enumeration for sleep state.
 * @details In its #sensor_event_s, #SENSOR_HUMAN_SLEEP_MONITOR reports the user's
 *          sleep state as one of the followings.
 * @since_tizen 3.0
 */
typedef enum
{
	SENSOR_SLEEP_STATE_UNKNOWN = -1, /**< Uncertain */
	SENSOR_SLEEP_STATE_WAKE,         /**< The user is awake */
	SENSOR_SLEEP_STATE_SLEEP,        /**< The user is asleep */
} sensor_sleep_state_e;


/**
 * @brief   Enumeration for sensor types.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
	SENSOR_ALL = -1,                        /**< All sensors. This can be used to retrieve #sensor_h for all available sensors. */
	SENSOR_ACCELEROMETER,                   /**< Accelerometer */
	SENSOR_GRAVITY,                         /**< Gravity sensor */
	SENSOR_LINEAR_ACCELERATION,             /**< Linear acceleration sensor */
	SENSOR_MAGNETIC,                        /**< Magnetic sensor */
	SENSOR_ROTATION_VECTOR,                 /**< Rotation vector sensor */
	SENSOR_ORIENTATION,                     /**< Orientation sensor */
	SENSOR_GYROSCOPE,                       /**< Gyroscope */
	SENSOR_LIGHT,                           /**< Light sensor */
	SENSOR_PROXIMITY,                       /**< Proximity sensor */
	SENSOR_PRESSURE,                        /**< Pressure sensor */
	SENSOR_ULTRAVIOLET,                     /**< Ultraviolet sensor */
	SENSOR_TEMPERATURE,                     /**< Temperature sensor */
	SENSOR_HUMIDITY,                        /**< Humidity sensor */
	SENSOR_HRM,                             /**< Heart-rate monitor @if MOBILE (Since 2.3.1) @endif
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_HRM_LED_GREEN,                   /**< Green LED sensor of HRM @if MOBILE (Since 2.3.1) @endif
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_HRM_LED_IR,                      /**< Infra-Red LED sensor of HRM @if MOBILE (Since 2.3.1) @endif
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_HRM_LED_RED,                     /**< Red LED sensor of HRM @if MOBILE (Since 2.3.1) @endif
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_GYROSCOPE_UNCALIBRATED,          /**< Uncalibrated Gyroscope sensor
	                                             @if MOBILE (Since 2.4) @elseif WEARABLE (Since 2.3.2) @endif */
	SENSOR_GEOMAGNETIC_UNCALIBRATED,        /**< Uncalibrated Geomagnetic sensor
	                                             @if MOBILE (Since 2.4) @elseif WEARABLE (Since 2.3.2) @endif */
	SENSOR_GYROSCOPE_ROTATION_VECTOR,       /**< Gyroscope-based rotation vector sensor
	                                             @if MOBILE (Since 2.4) @elseif WEARABLE (Since 2.3.2) @endif */
	SENSOR_GEOMAGNETIC_ROTATION_VECTOR,     /**< Geomagnetic-based rotation vector sensor
	                                             @if MOBILE (Since 2.4) @elseif WEARABLE (Since 2.3.2) @endif */
	SENSOR_HUMAN_PEDOMETER = 0x300,         /**< Pedometer (Since 3.0)
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_HUMAN_SLEEP_MONITOR,             /**< Sleep monitor (Since 3.0)
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_HUMAN_SLEEP_DETECTOR,            /**< Sleep detector (Since 3.0)
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_HUMAN_STRESS_MONITOR,            /**< Stress monitor (Since 3.0)
	                                             @n Privilege : http://tizen.org/privilege/healthinfo */
	SENSOR_LAST,                            /**< End of sensor enum values (Deprecated since 3.0) */
	SENSOR_CUSTOM = 0x2710,                 /**< Custom sensor (Deprecated since 3.0) */
} sensor_type_e;


/**
 * @brief   Checks whether a given sensor type is supported in the current device.
 * @details If the given sensor type is not supported, sensor_get_default_sensor() will return an error.
 *          It is thus recommended to check the availability of the sensor before actually acquiring #sensor_h.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   type        A sensor type to check
 * @param[out]  supported   If supported, @c true; Otherwise @c false
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_is_supported(sensor_type_e type, bool *supported);


/**
 * @brief   Checks whether a given sensor is a wake-up sensor or not.
 * @details If a sensor is a wake-up sensor, the sensor is able to wake-up the system
 *          to report its sensor data even if the system is in sleep mode.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]   sensor  A sensor handle to check
 * @param[out]  wakeup  If the sensor is a wake-up sensor, @c true;
 *                      Otherwise @c false
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @pre     The handle @c sensor needs to be initialized using
 *          sensor_get_default_sensor() or sensor_get_sensor_list() in advance.
 */
int sensor_is_wake_up(sensor_h sensor, bool *wakeup);

/**
 * @brief   Gets the handle for the default sensor of a given type.
 * @details This function returns the handle for the sensor of a given type,
 *          if the device has one sensor of the given type.
 *          In case that the device has more than one sensors of the type,
 *          this returns only the default sensor, which is designated by the device.@n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          http://tizen.org/privilege/healthinfo to get handles for the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type     A sensor type to get the handle of its default sensor
 * @param[out] sensor   The sensor handle of the default sensor
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        The sensor type is not supported in the current device
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 *
 * @see     sensor_get_sensor_list()
 */
int sensor_get_default_sensor(sensor_type_e type, sensor_h *sensor);


/**
 * @brief   Gets the handle list of the sensors of a given type.
 * @details A device may have more than one sensors of the given type.
 *          In such case, this function can be used to get the handles of all sensors of the type.@n
 *          The first element of the @c list denotes the default sensor,
 *          which can be retrieved by sensor_get_default_sensor().
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          http://tizen.org/privilege/healthinfo to get handles for the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.@n
 *          Instead of specifying a sensor type, by using #SENSOR_ALL,
 *          applications can get the list of handles for all available sensors.@n
 *          The @c list must be released using @c free(), if not being used anymore.@n
 *
 * @param[in]  type         A sensor type to get the list of sensor handles
 * @param[out] list         An array of the sensor handles
 * @param[out] sensor_count The number of handles contained in @c list
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        The sensor type is not supported in the current device
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 */
int sensor_get_sensor_list(sensor_type_e type, sensor_h **list, int *sensor_count);


/**
 * @brief   Gets the name of a sensor.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor  A sensor handle
 * @param[out]  name    The name of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_get_name(sensor_h sensor, char **name);


/**
 * @brief   Gets the vendor of a sensor.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor  A sensor handle
 * @param[out]  vendor  The vendor of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_get_vendor(sensor_h sensor, char **vendor);


/**
 * @brief   Gets the sensor type of a sensor.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor  A sensor handle
 * @param[out]  type    The type of the sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_get_type(sensor_h sensor, sensor_type_e *type);


/**
 * @brief   Gets the lower bound of the sensor reading of a sensor.
 * @details This function returns the lower bound of the range of possible sensor values,
 *          which are generated by the corresponding sensor denoted by a sensor handle.@n
 *          If all sensor values are in the same unit, e.g., \f$\mbox{m/s}^2\f$ or degrees,
 *          the lower bound of all sensor values is returned.
 *          Otherwise, the lower bound of the representative sensor value, e.g.,
 *          the step count of #SENSOR_HUMAN_PEDOMETER, is returned.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor      A sensor handle
 * @param[out]  min_range   The lower bound
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_get_max_range()
 */
int sensor_get_min_range(sensor_h sensor, float *min_range);


/**
 * @brief   Gets the upper bound of the sensor readings of a sensor.
 * @details This function returns the upper bound of the range of possible sensor values,
 *          which are generated by the corresponding sensor denoted by a sensor handle.@n
 *          If all sensor values are in the same unit, e.g., \f$\mbox{m/s}^2\f$ or degrees,
 *          the upper bound of all sensor values is returned.
 *          Otherwise, the upper bound of the representative sensor value, e.g.,
 *          the step count of #SENSOR_HUMAN_PEDOMETER, is returned.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor      A sensor handle
 * @param[out]  max_range   The upper bound
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_get_min_range()
 */
int sensor_get_max_range(sensor_h sensor, float *max_range);


/**
 * @brief   Gets the resolution of the sensor readings of a sensor.
 * @details This function returns the resolution of the sensor readings.
 *          The resolution denotes the smallest difference between sensor readings,
 *          each of which is in the range that can be verified by
 *          sensor_get_min_range() and sensor_get_max_range().
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor      A sensor handle
 * @param[out]  resolution  The resolution
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_get_resolution(sensor_h sensor, float *resolution);


/**
 * @brief   Gets the possible shorted update interval of a sensor.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor          A sensor handle
 * @param[out]  min_interval    The shorted interval in milliseconds
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_get_min_interval(sensor_h sensor, int *min_interval);


/**
 * @brief   Gets the size of the hardware FIFO of a sensor.
 * @details This function returns the size of the hardware FIFO that may be used by
 *          a specific sensor to support batching.
 *          However, regarding the underlying hardware configuration,
 *          the returned count may not mean the maximum number of sensor data that can be batched.
 *          See sensor_get_max_batch_count() for such purpose, finding out the
 *          possible maximum number of batched data.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor          A sensor handle
 * @param[out]  fifo_count      The FIFO count
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_get_fifo_count(sensor_h sensor, int *fifo_count);


/**
 * @brief   Gets the maximum batch count of a sensor.
 * @details This function returns the maximum number of sensor data events
 *          that can be possibly delivered when the batched data are flushed.
 *          Therefore, this count can be used to check whether the sensor supports
 *          batching or not.@n
 *          If this returns a positive count, i.e., the sensor supports batching,
 *          the count also can be used to guess the possible longest batch latency
 *          of the sensor, with respect to the update interval to use.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor          A sensor handle
 * @param[out]  max_batch_count If the sensor does not support batching, 0;
 *                              Otherwise a positive integer.
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_set_max_batch_latency()
 */
int sensor_get_max_batch_count(sensor_h sensor, int *max_batch_count);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_LISTENER_MODULE
 * @{
 */

/**
 * @brief   The upper bound of #sensor_event_s::value_count.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define MAX_VALUE_SIZE 16

/**
 * @brief   Sensor listener handle.
 * @details For each #sensor_h, one or more sensor listeners can be created by using sensor_create_listener().
 *          Then the sensor's data can observed asynchronously, can be read synchronously if available, via the listener.
 *          Applications are also able to control the behavior of each sensor, for example,
 *          update interval of sensor readings.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct sensor_listener_s *sensor_listener_h;


/**
 * @brief   Sensor data event delivered via sensor_event_cb().
 * @details A sensor data is delivered as a structure, which contains the accuracy of the data,
 *          the time when the data was observed, and the data array.
 *          The data array is a fixed size @c float array, and the number of data fields
 *          stored in the array varies with the sensor type.
 *          For example, #SENSOR_ACCELEROMETER reports 3-dimensional data,
 *          #sensor_event_s::value_count is thus set to 3.@n
 *          Note that, even if the data values are @c float, in some cases,
 *          it may contain one or more categorical data as in #sensor_proximity_e.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @see     #sensor_pedometer_state_e
 * @see     #sensor_sleep_state_e
 */
typedef struct
{
	int accuracy;                  /**< Accuracy of sensor data */
	unsigned long long timestamp;  /**< Time when the sensor data was observed */
	int value_count;               /**< Number of sensor data values stored in #sensor_event_s::values */
	float values[MAX_VALUE_SIZE];  /**< Sensor data values */
} sensor_event_s;


/**
 * @brief   Enumeration for sensor data accuracy.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
	SENSOR_DATA_ACCURACY_UNDEFINED   = -1,  /**< Undefined */
	SENSOR_DATA_ACCURACY_BAD         = 0,   /**< Not accurate */
	SENSOR_DATA_ACCURACY_NORMAL      = 1,   /**< Moderately accurate */
	SENSOR_DATA_ACCURACY_GOOD        = 2,   /**< Highly accurate */
	SENSOR_DATA_ACCURACY_VERYGOOD    = 3    /**< Very highly accurate */
} sensor_data_accuracy_e;


/**
 * @brief   Enumeration for sensor listener behavior attributes
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum
{
	SENSOR_ATTRIBUTE_AXIS_ORIENTATION = 1,  /**< Reference orientation of sensor data to be reported.@n
	                                             See #sensor_axis_e for available attribute values. */
	SENSOR_ATTRIBUTE_PAUSE_POLICY,          /**< Pause-and-resume policy of sensors.@n
	                                             See #sensor_pause_e for available attribute values. */
} sensor_attribute_e;


/**
 * @brief   Enumeration for sensor options.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#ifndef __SENSOR_COMMON_H__
typedef enum
{
	SENSOR_OPTION_DEFAULT,              /**< Does not receive data when the LCD is off and in the power save mode */
	SENSOR_OPTION_ON_IN_SCREEN_OFF,     /**< Receives data when the LCD is off */
	SENSOR_OPTION_ON_IN_POWERSAVE_MODE, /**< Receives data in the power save mode */
	SENSOR_OPTION_ALWAYS_ON,            /**< Receives data when the LCD is off and in the power save mode */
} sensor_option_e;
#endif


/**
 * @brief   Enumeration for reference orientations of sensor data
 * @details The sensor's physical orientation may differ from what applications are aware of,
 *          in cases that the device has a rotated screen, physically or logically.
 *          For example, a watch device may have right hand mode, which logically rotates
 *          the display 180 degrees.
 *          Applications may not be aware of such situations, thus they may receives
 *          sensor data inverted in X and Y directions.
 *          With #SENSOR_AXIS_DISPLAY_ORIENTED option, applications can get data that
 *          are properly aligned with the orientation of which they are aware.@n
 *          By default, #SENSOR_AXIS_DISPLAY_ORIENTED is used.
 *          If you need to use the data that are not affected by display orientations,
 *          #SENSOR_AXIS_DEVICE_ORIENTED needs to be set.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum
{
	SENSOR_AXIS_DEVICE_ORIENTED = 1,    /**< Using the device orientation as the reference coordinate system */
	SENSOR_AXIS_DISPLAY_ORIENTED,       /**< Using the display orientation as the reference coordinate system */
} sensor_axis_e;


/**
 * @brief   Enumeration for pause policies of sensor listeners
 * @details To be power-efficient, you can set the policy of how to pause and resume
 *          a sensor listener regarding the system status.
 *          By default, #SENSOR_PAUSE_ALL is used to obtain the maximum power efficiency.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum
{
	SENSOR_PAUSE_NONE = 0,              /**< The sensor will not pause, unless the system goes into sleep mode */
	SENSOR_PAUSE_ON_DISPLAY_OFF = 1,    /**< The sensor pauses while the display is off*/
	SENSOR_PAUSE_ON_POWERSAVE_MODE = 2, /**< The sensor pauses while the power-save mode is enabled */
	SENSOR_PAUSE_ALL = 3,               /**< The sensor pauses in all the above cases */
} sensor_pause_e;


/**
 * @brief   Called when a sensor event occurs.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] sensor    The corresponding sensor handle
 * @param[in] event     A sensor event
 * @param[in] data      The user data had passed to sensor_listener_set_event_cb()
 *
 * @pre     The sensor needs to be started regarding a listener handle, using sensor_listener_start().
 */
typedef void (*sensor_event_cb)(sensor_h sensor, sensor_event_s *event, void *data);


/**
 * @brief   Called when the accuracy of a sensor changes.
 * @details Sensors can be affected by the environment.
 *          For example, #SENSOR_MAGNETIC is sensitive to any surrounding objects that can influence
 *          electromagnetic fields. This function is called if the accuracy of the corresponding sensor is changed.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   sensor      A sensor handle
 * @param[in]   timestamp   The time in milliseconds when the accuracy changed
 * @param[in]   accuracy    The current accuracy of the sensor
 * @param[in]   data        The user data had passed to sensor_listener_set_accuracy_cb()
 */
typedef void (*sensor_accuracy_changed_cb)(sensor_h sensor, unsigned long long timestamp, sensor_data_accuracy_e accuracy, void *data);


/**
 * @brief   Creates a sensor listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks The @c listener must be released using sensor_destroy_listener().
 *
 * @param[in]  sensor          A sensor handle
 * @param[out] listener        A listener handle of @c sensor
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The handle @c sensor needs to be initialized using
 *          sensor_get_default_sensor() or sensor_get_sensor_list() in advance.
 */
int sensor_create_listener(sensor_h sensor, sensor_listener_h *listener);


/**
 * @brief   Releases all the resources allocated for a listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks If this function is called while the sensor is still running,
 *          that is, sensor_listener_start() was called but sensor_listener_stop() was not,
 *          then it is implicitly stopped.
 *
 * @param[in] listener  A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see     sensor_create_listener()
 */
int sensor_destroy_listener(sensor_listener_h listener);


/**
 * @brief   Starts observing the sensor events regarding a given sensor listener.
 * @details If a sensor listener is started, its event callback function starts to be called
 *          whenever the corresponding sensor events occur.@n
 *          For example, #SENSOR_ACCELEROMETER reports its sensor readings repeatedly,
 *          with a specific update interval.
 *          Note that, unlike the accelerometer, sensors like #SENSOR_PROXIMITY emit events
 *          only if their states change.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener  A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The @c listener needs to be created in advance, by using sensor_create_listener().
 *          Then the callback function needs to be attached to the @c listener, by using
 *          sensor_listener_set_event_cb().
 * @see     sensor_listener_stop()
 */
int sensor_listener_start(sensor_listener_h listener);


/**
 * @brief   Stops observing the sensor events regarding a given sensor listener.
 * @details The listener's event callback function stops being called.
 *          But the sensor itself may not be stopped if there are other listeners
 *          that are using the same sensor.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener  A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_start()
 */
int sensor_listener_stop(sensor_listener_h listener);


/**
 * @brief   Registers the callback function to be invoked when sensor events are delivered via a sensor listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener    A listener handle
 * @param[in]   interval_ms A desired update interval between sensor events in milliseconds.@n
 *                          If 0, it will be automatically set to the default interval of the corresponding sensor.@n
 *                          See sensor_listener_set_interval() for more details.
 * @param[in]   callback    A callback function to attach with the @c listener handle
 * @param[in]   data        A user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see sensor_listener_unset_event_cb()
 */
int sensor_listener_set_event_cb(sensor_listener_h listener, unsigned int interval_ms, sensor_event_cb callback, void *data);


/**
 * @brief   Unregisters the sensor event callback function attached to a given sensor listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener    A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_set_event_cb()
 */
int sensor_listener_unset_event_cb(sensor_listener_h listener);


/**
 * @brief   Registers the callback function to be invoked when the accuracy of a sensor changes.
 * @details In addition to sensor_event_cb(), sensor_accuracy_changed_cb() also can be attached
 *          to sensor listeners. With this accuracy callback function, applications can be notified
 *          the changes of the corresponding sensors separately.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener    A listener handle
 * @param[in]   callback    A callback function to attach with the @c listener handle
 * @param[in]   data        A user data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @pre     The @c listener needs to be started to get the change callbacks.
 * @see     sensor_listener_unset_accuracy_cb()
 */
int sensor_listener_set_accuracy_cb(sensor_listener_h listener, sensor_accuracy_changed_cb callback, void *data);


/**
 * @brief   Unregisters the sensor accuracy change callback function attached to a given sensor listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]   listener    A listener handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_listener_set_accuracy_cb()
 */
int sensor_listener_unset_accuracy_cb(sensor_listener_h listener);


/**
 * @brief   Reads the current sensor data via a given sensor listener.
 * @details This function synchronously reads the sensor reading of the corresponding sensor, if available.
 *          Otherwise, if the sensor is not ready to report its values, this function fails and returns
 *          #SENSOR_ERROR_OPERATION_FAILED.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remark  As a sensor usually works in an event-driven manner, it may not be able to read its data on demand.
 *          Then this function tries to return the last known values.@n
 *          To be able to read the current values or the last known values, the sensor needs to be
 *          enabled in advance. As an application cannot be sure that the sensor is already enabled
 *          by other applications, it is recommended to start the sensor explicitly by using sensor_listener_start().@n
 *          But note that, even if the sensor is started, on-demand reading can fail always,
 *          thus it would be better to use the asynchronous callback approach.
 *
 * @param[in]   listener    A listener handle
 * @param[out]  event       The retrieved sensor data
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_listener_read_data(sensor_listener_h listener, sensor_event_s *event);


/**
 * @brief   Changes the update interval of a sensor.
 * @details The specified interval is only a suggested interval between sensor measurements.
 *          You will get at least one sensor measurement within the interval you specify,
 *          but the actual interval between sensor measurements can be affected by other applications and the system.
 *          To reduce the system overhead, it is recommended to set the longest interval that you can,
 *          because the system usually chooses the shortest interval among all intervals specified.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Normally, a sensor's default update interval is 100 ms,
 *          and you can use the default interval by setting the interval to 0.
 *          However, please note that, the default interval varies with the sensor and the device.@n
 *          In addition, a sensor has the lower and the upper bound of its update interval,
 *          usually 10 and 1000 ms respectively.
 *          These lower and upper bounds also can vary with the sensor and the device,
 *          any invalid input values exceeding the bounds will be implicitly adjusted into the valid range.
 *
 * @param[in]   listener    A listener handle
 * @param[in]   interval_ms A desired update interval between sensor events in milliseconds.
 *                          If 0, it will be automatically set to the default interval of the corresponding sensor.
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_get_min_interval()
 */
int sensor_listener_set_interval(sensor_listener_h listener, unsigned int interval_ms);


/**
 * @brief   Sets the desired max batch latency of a sensor.
 * @details Sensors that support batching may allow applications to change their maximum batch latencies.
 *          For example, if you set the latency as 10,000 ms, the sensor may store its data
 *          up to 10,000 ms, before delivering the data through the HAL.@n
 *          In cases of non-batching sensors, this function returns #SENSOR_ERROR_NONE,
 *          but nothing is affected by the input latency value.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Even if you set a batch latency, the sensor may not work as you intended,
 *          as one sensor can be used by more than one listeners.
 *          In addition, some batch sensors may already have fixed batching latency
 *          or batching queue size, which cannot be altered by applications.
 *
 * @param[in]   listener           A listener handle
 * @param[in]   max_batch_latency  A desired batch latency in milliseconds
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_listener_set_max_batch_latency(sensor_listener_h listener, unsigned int max_batch_latency);


/**
 * @brief   Sets an attribute to control the behavior of a sensor listener.
 * @details Applications can change the behavior of a sensor listener, for example,
 *          what is the reference coordinate of the sensor values,
 *          and when the system is allowed to turn off the sensor implicitly to reduce the power consumption.
 *          See #sensor_attribute_e for more details about the available control parameters.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]   listener	A listener handle
 * @param[in]   attribute	An attribute to change
 * @param[in]   value		An attribute value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_listener_set_attribute_int(sensor_listener_h listener, sensor_attribute_e attribute, int value);


/**
 * @brief   Changes the power-saving behavior of a sensor listener.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remark  sensor_listener_set_attribute_int() with #SENSOR_ATTRIBUTE_PAUSE_POLICY replaces this function.
 *
 * @param[in]   listener        A listener handle
 * @param[in]   option          A sensor option
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_listener_set_option(sensor_listener_h listener, sensor_option_e option);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_RECORDER_MODULE
 * @{
 */

/**
 * @brief   Option handle to contain recording policies and parameters.
 * @details one or more sensor options can be created by using sensor_recorder_create_option().
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef void *sensor_recorder_option_h;

/**
 * @brief   Query handle to contain filtering and aggregation parameters for recorded data.
 * @details one or more sensor options can be created by using sensor_recorder_create_query().
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef void *sensor_recorder_query_h;

/**
 * @brief   Data handle to contain retrieved sensor records.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef void *sensor_recorder_data_h;

/**
 * @brief   Enumeration for option parameters for sensor recording.
 * @details None, one, or more option parameters can be set to #sensor_recorder_option_h,
 *          then applications can request to record a specific sensor with the parameters via
 *          sensor_recorder_start().
 *          If a parameter is not supported for the specified sensor type, it will be ignored.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum
{
	SENSOR_RECORDER_OPTION_RETENTION_PERIOD,      /**< Desired retention period for the recorded sensor data (hours); int; */
	SENSOR_RECORDER_OPTION_INTERVAL,              /**< Desired interval between data records (minutes); int; see #sensor_recorder_interval_e */
} sensor_recorder_option_e;

/**
 * @brief   Enumeration for intervals between data records.
 * @details Some sensor types including #SENSOR_HRM may not fit to continuous monitoring and recording.
 *          To reduce the battery use for recoridng such sensors, recording interval between each
 *          sensor data is chosen. For example, an application set the interval of 1 hour,
 *          the platform tries to retrieve the sensor data only once per hour, thus it will be more
 *          battery efficient than to listen the sensor data continuously.@n
 *          If the application does not set the interval explicitely, the default value is chosen.
 *          The default values are differ from sensor to sensor.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @remarks If more than one applications set different intervals, the shortest value is chosen.
 */
typedef enum
{
	SENSOR_RECORDER_INTERVAL_10_MINUTES = 10,   /**< 10 Minutes */
	SENSOR_RECORDER_INTERVAL_1_HOUR = 60,       /**< 1 Hour */
	SENSOR_RECORDER_INTERVAL_3_HOURS = 180,     /**< 3 Hours */
	SENSOR_RECORDER_INTERVAL_6_HOURS = 360,     /**< 6 Hours */
	SENSOR_RECORDER_INTERVAL_12_HOURS = 720,    /**< 12 Hours */
	SENSOR_RECORDER_INTERVAL_1_DAY = 1440,      /**< 1 Day */
} sensor_recorder_interval_e;

/**
 * @brief   Enumeration for filtering and aggregation parameters for queyring sensor records.
 * @details None, one, or more query parameters can be set to #sensor_recorder_query_h,
 *          to specify the data to be retrieved via sensor_recorder_read().
 *          If a necessary parameter is not set, the default value is chosen.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 */
typedef enum
{
	SENSOR_RECORDER_QUERY_START_TIME = 0,   /**< Start time of the data to be queried (Epoch); time_t; If unspecified, 1 day ago */
	SENSOR_RECORDER_QUERY_END_TIME,         /**< End time of the data to be queried (Epoch); time_t; If unspecified, the current time */
	SENSOR_RECORDER_QUERY_ANCHOR_TIME,      /**< The anchor time to slice the querying duration of time (Epoch); time_t;
	                                             It needs to be set with #SENSOR_RECORDER_QUERY_TIME_INTERVAL, otherwise, it is ignored */
	SENSOR_RECORDER_QUERY_TIME_INTERVAL,    /**< The interval of each sliced querying duration (minutes); int */
} sensor_recorder_query_e;

/**
 * @brief   Enumeration for data attributes can be contained in #sensor_recorder_data_h.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 * @see     sensor_recorder_data_get_int()
 * @see     sensor_recorder_data_get_double()
 */
typedef enum
{
	SENSOR_RECORDER_DATA_STEPS = 0x00,           /**< Count of both walking and running steps; int
	                                                  @if WEARABLE (Since 3.0) @endif */
	SENSOR_RECORDER_DATA_WALK_STEPS,             /**< Count of walking steps; int
	                                                  @if WEARABLE (Since 3.0) @endif */
	SENSOR_RECORDER_DATA_RUN_STEPS,              /**< Count of running steps; int
	                                                  @if WEARABLE (Since 3.0) @endif */
	SENSOR_RECORDER_DATA_DISTANCE,               /**< Distance walked or ran (m); double
	                                                  @if WEARABLE (Since 3.0) @endif */
	SENSOR_RECORDER_DATA_CALORIE,                /**< Calorie burned (kcal); double
	                                                  @if WEARABLE (Since 3.0) @endif */
	SENSOR_RECORDER_DATA_HEART_RATE = 0x10,      /**< Heart Rate (BPM); int
	                                                  @if WEARABLE (Since 3.0) @endif */
	SENSOR_RECORDER_DATA_SLEEP_STATE = 0x20,     /**< Sleep state; int; One of #sensor_sleep_state_e
	                                                  @if WEARABLE (Since 3.0) @endif */
	SENSOR_RECORDER_DATA_PRESSURE = 0x30,        /**< Pressure; double */
	SENSOR_RECORDER_DATA_MAX_PRESSURE,           /**< Max pressure; double */
	SENSOR_RECORDER_DATA_MIN_PRESSURE,           /**< Min pressure; double */
	SENSOR_RECORDER_DATA_AVERAGE_PRESSURE,       /**< Average pressure; double */
} sensor_recorder_data_e;

/**
 * @brief   Checks whether it is suppored to record a given sensor type.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]   type        A sensor type to check
 * @param[out]  supported   If supported, @c true; Otherwise @c false
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_is_supported(sensor_type_e type, bool *supported);

/**
 * @brief   Starts to record a given sensor type.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          http://tizen.org/privilege/healthinfo to get access to the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type    A sensor type to be recorded
 * @param[in]  option  Option
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        Not supported to record the sensor type
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NOT_AVAILABLE        The sensor is already being recorded by the request of the current application
 *
 * @see sensor_recorder_stop()
 */
int sensor_recorder_start(sensor_type_e type, sensor_recorder_option_h option);

/**
 * @brief   Stops recording a given sensor type.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  type    A sensor type being recorded
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        The sensor type is not supported
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see sensor_recorder_start()
 */
int sensor_recorder_stop(sensor_type_e type);

/**
 * @brief   Creates a recorder option handle.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @remarks The @a option must be released using sensor_recorder_destroy_option().
 *
 * @param[out]  option  Option handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_create_option(sensor_recorder_option_h *option);

/**
 * @brief   Destroys a recorder option handle.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  option  Option handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_destroy_option(sensor_recorder_option_h option);

/**
 * @brief   Sets a recording option parameter.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  option      Option handle
 * @param[in]  param       Option parameter
 * @param[in]  value       Value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_option_set_int(sensor_recorder_option_h option, sensor_recorder_option_e param, int value);

/**
 * @brief   Creates a recorder query handle.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @remarks The @a query must be released using sensor_recorder_destroy_query().
 *
 * @param[out]  query  Query handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OUT_OF_MEMORY        Out of memory
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_create_query(sensor_recorder_query_h *query);

/**
 * @brief   Destroys a recorder query handle.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  query  Query handle
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_destroy_query(sensor_recorder_query_h query);

/**
 * @brief   Sets an integer-type query parameter
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  query       Query handle
 * @param[in]  param       Query parameter
 * @param[in]  value       Value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_query_set_int(sensor_recorder_query_h query, sensor_recorder_query_e param, int value);

/**
 * @brief   Sets a time-type query parameter.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  query       Query handle
 * @param[in]  param       Query parameter
 * @param[in]  t           Time
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_query_set_time(sensor_recorder_query_h query, sensor_recorder_query_e param, time_t t);

/**
 * @brief   Called when the query results are retieved.
 * @details One of the following errors can be delivered.\n
 *          #SENSOR_ERROR_NONE, Successful\n
 *          #SENSOR_ERROR_OPERATION_FAILED, Operation failed\n
 *          #SENSOR_ERROR_NO_DATA, No data retrieved.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  type        Sensor type
 * @param[in]  data        Retrieved data record
 * @param[in]  remains     Number of remaining records to be delivered
 * @param[in]  error       Error
 * @param[in]  user_data   The user data passed from sensor_recorder_read() or sensor_recorder_read_sync()
 *
 * @return  If @c true, it continues to iterate to the next record; If @c false, the iteration stops
 */
typedef bool (*sensor_recorder_data_cb)(sensor_type_e type, sensor_recorder_data_h data, int remains, sensor_error_e error, void *user_data);

/**
 * @brief    Queries the recorded data asynchronously.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          http://tizen.org/privilege/healthinfo to get access to the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type        Sensor type
 * @param[in]  query       Query handle
 * @param[in]  cb          Callback function to receive the queried data
 * @param[in]  user_data   User data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        Not supported to record the sensor type
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 *
 * @see     sensor_recorder_read_sync()
 */
int sensor_recorder_read(sensor_type_e type, sensor_recorder_query_h query, sensor_recorder_data_cb cb, void *user_data);

/**
 * @brief    Queries the recorded data synchronously.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @remarks Some sensor types are privileged. An application should have the privilege
 *          http://tizen.org/privilege/healthinfo to get access to the following sensors:
 *          #SENSOR_HRM, #SENSOR_HRM_LED_GREEN, #SENSOR_HRM_LED_IR, #SENSOR_HRM_LED_RED,
 *          #SENSOR_HUMAN_PEDOMETER, #SENSOR_HUMAN_SLEEP_MONITOR, #SENSOR_HUMAN_SLEEP_DETECTOR,
 *          and #SENSOR_HUMAN_STRESS_MONITOR.
 *
 * @param[in]  type        Sensor type
 * @param[in]  query       Query handle
 * @param[in]  cb          Callback function to receive the queried data
 * @param[in]  user_data   User data to be passed to the callback function
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_NOT_SUPPORTED        Not supported to record the sensor type
 * @retval  #SENSOR_ERROR_PERMISSION_DENIED    Permission denied
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NO_DATA              No data retrieved
 *
 * @see     sensor_recorder_read()
 */
int sensor_recorder_read_sync(sensor_type_e type, sensor_recorder_query_h query, sensor_recorder_data_cb cb, void *user_data);

/**
 * @brief    Gets the start and the end time of the time period of a given record data.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  data        Record data handle
 * @param[out] start_time  Start time of the time period of the record
 * @param[out] end_time    End time of the time period of the record
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 */
int sensor_recorder_data_get_time(sensor_recorder_data_h data, time_t *start_time, time_t *end_time);

/**
 * @brief    Gets an integer value from a record data.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  data        Record data handle
 * @param[in]  key         Data attribute to retrieve
 * @param[out] value       Retrieved value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NO_DATA              No data retrieved
 */
int sensor_recorder_data_get_int(sensor_recorder_data_h data, sensor_recorder_data_e key, int *value);

/**
 * @brief    Gets a double value from a record data.
 * @since_tizen @if MOBILE 3.0 @elseif WEARABLE 2.3.2 @endif
 *
 * @param[in]  data        Record data handle
 * @param[in]  key         Data attribute to retrieve
 * @param[out] value       Retrieved value
 *
 * @return  #SENSOR_ERROR_NONE on success, otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval  #SENSOR_ERROR_OPERATION_FAILED     Operation failed
 * @retval  #SENSOR_ERROR_NO_DATA              No data retrieved
 */
int sensor_recorder_data_get_double(sensor_recorder_data_h data, sensor_recorder_data_e key, double *value);

/**
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_SENSOR_UTILITY_MODULE
 * @{
 */

/**
 * @brief   Enumeration of the axis used in sensor_util_remap_coordinate_system().
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    SENSOR_UTIL_AXIS_MINUS_X = 0,  /**< -X */
    SENSOR_UTIL_AXIS_MINUS_Y,      /**< -Y */
    SENSOR_UTIL_AXIS_MINUS_Z,      /**< -Z */
    SENSOR_UTIL_AXIS_X,            /**< +X */
    SENSOR_UTIL_AXIS_Y,            /**< +Y */
    SENSOR_UTIL_AXIS_Z,            /**< +Z */
    sensor_util_axis_minus_x = SENSOR_UTIL_AXIS_MINUS_X,
    sensor_util_axis_minus_y,
    sensor_util_axis_minus_z,
    sensor_util_axis_x,
    sensor_util_axis_y,
    sensor_util_axis_z,
} sensor_util_axis_e;

/**
 * @brief   Gets the inclination matrix "I" and rotation matrix "R" transforming a vector from the device coordinate to the world's coordinate.
 *
 * @details [0 0 g] = R * gravity (g = magnitude of gravity) \n
 *          [0 m 0] = I * R * geomagnetic (m = magnitude of the geomagnetic field) \n
 *          R is the identity matrix when the device is aligned with the world's coordinate system, that is, when the device's X axis points towards the East, the Y axis points to the North Pole and the device is facing the sky. \n
 *          I is a rotation matrix transforming the geomagnetic vector into the same coordinate space as gravity (the world's coordinate space). I is a simple rotation around the X axis. \n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Parameters Gx, Gy, and Gz can be obtained from the values returned by #SENSOR_GRAVITY. \n
 *          Parameters Mx, My, and Mz can be obtained from the values returned by #SENSOR_MAGNETIC.
 *          Output parameter R and I are always returned as a 3x3 matrix array of 9 floats like this form:
 *          <pre>
 *          { R[0], R[1], R[2],
 *            R[3], R[4], R[5],
 *            R[6], R[7], R[6] }
 *          </pre>
 *
 *
 * @param[in]  Gx   The X-axis gravity vector in the device's coordinate
 * @param[in]  Gy   The Y-axis gravity vector in the device's coordinate
 * @param[in]  Gz   The Z-axis gravity vector in the device's coordinate
 * @param[in]  Mx   The X-axis geomagnetic vector in the device's coordinate
 * @param[in]  My   The Y-axis geomagnetic vector in the device's coordinate
 * @param[in]  Mz   The Z-axis geomagnetic vector in the device's coordinate
 * @param[out] R    The array of 9 floats that represent the rotation matrix "R" \n
 *                  It can be null.
 * @param[out] I    The array of 9 floats that represent the inclination matrix "I" \n
 *                  It can be null.
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_util_get_rotation_matrix(float Gx, float Gy, float Gz,
        float Mx, float My, float Mz,
        float R[], float I[]);

/**
 * @brief Converts a rotation vector to a rotation matrix.
 *
 * @details Rotation vectors (Vx, Vy, Vz) can be obtained from #SENSOR_ROTATION_VECTOR.
 *          It returns a 9 element rotation matrix in the array R. R must have length as 9.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  Vx   The X-axis rotation vector
 * @param[in]  Vy   The Y-axis rotation vector
 * @param[in]  Vz   The Z-axis rotation vector
 * @param[out] R    A 9 element rotation matrix in the array R that must have length as 9
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_util_get_rotation_matrix_from_vector(float Vx, float Vy, float Vz, float R[]);

/**
 * @brief Rotates the supplied rotation matrix so that it is expressed in a different coordinate system.
 *
 * @details This is typically used when an application needs to compute the three orientation angles of the device in a different coordinate system.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks inR and outR can be the same array, but this is not recommended for performance reasons.
 *          This returns an error when X and Y define the same axis.
 *
 * @param[in]  inR  The rotation matrix (3x3) to be transformed
 * @param[in]  x    The world axis and direction on which the X axis of the device is mapped
 * @param[in]  y    The world axis and direction on which the Y axis of the device is mapped
 * @param[out] outR The transformed rotation matrix (3x3)
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 */
int sensor_util_remap_coordinate_system(float inR[], sensor_util_axis_e x, sensor_util_axis_e y, float outR[]);

/**
 * @brief Computes the geomagnetic inclination angle in radians from the inclination matrix I returned by sensor_util_get_rotation_matrix().
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  I            The inclination matrix from sensor_util_get_rotation_matrix()
 * @param[out] inclination  The geomagnetic inclination angle in radians
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see sensor_util_get_rotation_matrix()
 */
int sensor_util_get_inclination(float I[], float* inclination);

/**
 * @brief Computes the device's orientation based on the rotation matrix.
 *
 * @details When it returns, the array values are filled with the result:
 *          - values[0]: azimuth, rotation around the Z axis.
 *          - values[1]: pitch, rotation around the X axis.
 *          - values[2]: roll, rotation around the Y axis.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Parameter R must be an array of 9 floats from sensor_util_get_rotation_matrix() \n
 *          Returned values are always arrays of 3 floats.
 *
 * @param[in]  R         A 9 element rotation matrix in the array
 * @param[out] values    An array of 3 floats to hold the result
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 *
 * @see sensor_util_get_rotation_matrix()
 *
 */
int sensor_util_get_orientation(float R[], float values[]);

/**
 * @brief Computes the angle change between two rotation matrices.
 *
 * @details Given a current rotation matrix (R) and a previous rotation matrix (prevR), it computes
 *          the rotation around the x,y, and z axes which transforms prevR to R.
 *          It outputs a 3 element vector containing the x,y, and z angle change at indexes 0, 1, and 2 respectively. \n
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Each input matrix is a 3x3 matrix like this form:
 *          <pre>
 *          { R[0], R[1], R[2],
 *            R[3], R[4], R[5],
 *            R[6], R[7], R[6] }
 *          </pre>
 *
 * @param[in] R             The current rotation matrix
 * @param[in] prevR         The previous rotation matrix
 * @param[out] angleChange  An array of floats in which the angle change is stored
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_util_get_angle_change(float R[], float prevR[], float angleChange[]);

/**
 * @brief Gets the declination of the horizontal component of the magnetic field from true north, in degrees.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  latitude     The latitude in geodetic coordinates
 * @param[in]  longitude    The longitude in geodetic coordinates
 * @param[in]  altitude     The altitude in geodetic coordinates
 * @param[out] declination  The declination of the horizontal component of the magnetic field in degrees
 *
 * @return  #SENSOR_ERROR_NONE on success; Otherwise a negative error value
 * @retval  #SENSOR_ERROR_NONE                 Successful
 * @retval  #SENSOR_ERROR_INVALID_PARAMETER    Invalid parameter
 */
int sensor_util_get_declination(float latitude, float longitude, float altitude, float* declination);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_H__ */
