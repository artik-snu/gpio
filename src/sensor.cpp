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

#include <sensor_internal.h>
#include <sensor.h>
#include <sensor_private.h>
#include <libgen.h>
#include <memory>
#include "include/sensor_log.h"

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

#define SENSOR_SHIFT_TYPE 16
#define SENSOR_UNDEFINED_ID -1
#define SENSOR_BATCH_LATENCY_DEFAULT UINT_MAX

#define SENSOR_LISTENER_MAGIC 0xCAFECAFE

#define CONVERT_AXIS_ENUM(X) ((X) < 3 ? (X) + 0x81 : (X) - 2)

#define CONVERT_OPTION_PAUSE_POLICY(option) ((option) ^ 0b11)

#define WARN_DEPRECATED_SENSOR(X) \
	do { \
		if ((X) == SENSOR_LAST || (X) == SENSOR_CUSTOM) { \
			_W("DEPRECATION WARNING: This sensor type is deprecated and will be removed from next release."); \
		} \
	} while (0)

static int sensor_connect(sensor_h sensor, sensor_listener_h listener)
{
	int id = SENSOR_UNDEFINED_ID;
	int event_type;
	sensor_type_t type;
	bool support = false;

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	_D("called sensor_connect : listener[0x%x], sensor[0x%x]", listener, sensor);

	sensord_get_type(sensor, &type);
	event_type = type << SENSOR_SHIFT_TYPE | 0x1;

	if (!sensord_is_supported_event_type(sensor, event_type, &support))
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!support)
		return SENSOR_ERROR_NOT_SUPPORTED;

	id = sensord_connect(sensor);

	if (id < 0)
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_connect: id[%d]", id);

	listener->id = id;
	listener->type = type;

	return id;
}

int sensor_is_supported(sensor_type_e type, bool *supported)
{
	int result;
	sensor_t sensor;

	WARN_DEPRECATED_SENSOR(type);

	if (type < SENSOR_ALL)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!supported)
		return SENSOR_ERROR_INVALID_PARAMETER;

	_D("called sensor_is_supported : type[%d]", type);

	 result = sensord_get_default_sensor((sensor_type_t)type, &sensor);

	if (result == -EPERM)
		return SENSOR_ERROR_OPERATION_FAILED;

	*supported = false;

	/*
	 * because is_supported() API is N/P API,
	 * it must not returns PERMISSION_DENIED error.
	 * -EACCES means that there is at least one sensor.
	 */
	if (sensor || (result == -EACCES))
		*supported = true;

	_D("success sensor(%d) is supported[%d]", type, *supported);

	return SENSOR_ERROR_NONE;
}

int sensor_get_default_sensor(sensor_type_e type, sensor_h *sensor)
{
	int result;
	sensor_t _sensor;
	sensor_privilege_t privilege;

	WARN_DEPRECATED_SENSOR(type);

	_D("called sensor_get_default_sensor : type[%d], sensor[0x%x]", type, sensor);

	if (type < SENSOR_ALL)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensor)
		return SENSOR_ERROR_INVALID_PARAMETER;

	result = sensord_get_default_sensor((sensor_type_t)type, &_sensor);
	if (result == -EACCES)
		return SENSOR_ERROR_PERMISSION_DENIED;
	else if (result == -EPERM)
		return SENSOR_ERROR_OPERATION_FAILED;

	if (!_sensor)
		return SENSOR_ERROR_NOT_SUPPORTED;

	sensord_get_privilege(_sensor, &privilege);
	if (privilege != SENSOR_PRIVILEGE_PUBLIC)
		return SENSOR_ERROR_PERMISSION_DENIED;

	*sensor = _sensor;

	_D("success sensor_get_default_sensor sensor[0x%x]", _sensor);

	return SENSOR_ERROR_NONE;
}

int sensor_get_sensor_list(sensor_type_e type, sensor_h **list, int *sensor_count)
{
	int result;
	sensor_h *_list;
	int count;

	WARN_DEPRECATED_SENSOR(type);

	_D("called sensor_get_list : type[%d]", type);

	if (type < SENSOR_ALL)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensor_count || !list)
		return SENSOR_ERROR_INVALID_PARAMETER;

	result = sensord_get_sensors((sensor_type_t)type, &_list, &count);

	if (result == -EACCES)
		return SENSOR_ERROR_PERMISSION_DENIED;
	else if (result == -EPERM)
		return SENSOR_ERROR_OPERATION_FAILED;
	else if (result == -ENODATA)
		return SENSOR_ERROR_NOT_SUPPORTED;

	int i, j;
	int count_public = 0;

	for (i = 0; i < count; ++i) {
		sensor_privilege_t privilege;

		sensord_get_privilege(_list[i], &privilege);
		if (privilege != SENSOR_PRIVILEGE_PUBLIC)
			continue;

		count_public++;
	}

	if (count_public == 0) {
		free(_list);
		return SENSOR_ERROR_PERMISSION_DENIED;
	}

	*list = (sensor_h *) malloc((sizeof(int *)) * count_public);

	if (!*list) {
		free(_list);
		return SENSOR_ERROR_OUT_OF_MEMORY;
	}

	for (i = 0, j = 0; i < count; ++i) {
		sensor_privilege_t privilege;

		sensord_get_privilege(_list[i], &privilege);
		if (privilege != SENSOR_PRIVILEGE_PUBLIC)
			continue;

		*(*list + j) = _list[i];
		j++;
	}

	free(_list);

	*sensor_count = count_public;

	_D("success sensor_get_list");

	return SENSOR_ERROR_NONE;
}

int sensor_is_wake_up(sensor_h sensor, bool *wakeup)
{
	_D("called sensor_get_type");

	if (!sensor || !wakeup)
		return SENSOR_ERROR_INVALID_PARAMETER;

	*wakeup = sensord_is_wakeup_supported(sensor);

	_D("success sensor_is_wake_up : [%d]", *wakeup);

	return SENSOR_ERROR_NONE;
}

int sensor_create_listener(sensor_h sensor, sensor_listener_h *listener)
{
	struct sensor_listener_s *_listener;
	int error;

	_D("called sensor_create_listener : listener[0x%x]", listener);

	if (!sensor || !listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	_listener = new(std::nothrow) struct sensor_listener_s;

	if (!_listener)
		return SENSOR_ERROR_OUT_OF_MEMORY;

	error = sensor_connect(sensor, _listener);

	if (error < 0) {
		delete (struct sensor_listener_s *)_listener;
		return SENSOR_ERROR_OPERATION_FAILED;
	}

	_listener->sensor = sensor;
	_listener->pause = SENSOR_PAUSE_ALL;
	_listener->batch_latency = SENSOR_BATCH_LATENCY_DEFAULT;
	_listener->magic = SENSOR_LISTENER_MAGIC;

	*listener = (sensor_listener_h) _listener;

	_D("success sensor_create_listener");

	return SENSOR_ERROR_NONE;
}

int sensor_destroy_listener(sensor_listener_h listener)
{
	_D("called sensor_destroy : listener[0x%x]", listener);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	sensord_disconnect(listener->id);
	listener->magic = 0;

	delete (sensor_listener_s *)listener;

	_D("success sensor_destroy");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_start(sensor_listener_h listener)
{
	int id;
	unsigned int pause = SENSOR_PAUSE_ALL;

	_D("called sensor_listener_start : listener[0x%x]", listener);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;
	pause = listener->pause;

	if (!sensord_start(id, 0))
		return SENSOR_ERROR_OPERATION_FAILED;

	if (sensord_set_attribute_int(id, SENSOR_ATTRIBUTE_PAUSE_POLICY, pause) < 0)
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_listener_start : id[%d]", id);

	return SENSOR_ERROR_NONE;
}

int sensor_listener_stop(sensor_listener_h listener)
{
	int id;

	_D("called sensor_listener_stop : listener[0x%x]", listener);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;

	if (!sensord_stop(id))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_listener_stop");

	return SENSOR_ERROR_NONE;
}

static void sensor_callback(sensor_t sensor, unsigned int event_type, sensor_data_t *data, void *user_data)
{
	sensor_event_s *event;
	sensor_listener_h listener;
	listener = (sensor_listener_h)user_data;

	_D("success sensor_callback, sensor[%p] listener[%p] listener->callback[%p]", sensor, listener, listener->callback);
	if (!sensor || !listener->callback)
		return;

	event = (sensor_event_s *)data;

	((sensor_event_cb) listener->callback)(sensor, event, listener->user_data);
	return;
}

int sensor_listener_set_event_cb(sensor_listener_h listener,
		unsigned int interval, sensor_event_cb callback, void *user_data)
{
	int id;
	unsigned int event_id;
	unsigned int batch_latency;

	if (!listener || !callback)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	_D("called sensor_listener_set_event : listener[0x%x], interval[%d], callback[0x%x], user_data[0x%x], id[%d]",
			listener, interval, callback, user_data, listener->id);

	id = listener->id;
	event_id = (listener->type) << SENSOR_SHIFT_TYPE | 0x1;
	batch_latency = listener->batch_latency;

	listener->callback = (void *)callback;
	listener->user_data = user_data;

	if (!sensord_register_event(id, event_id, interval, batch_latency,
				sensor_callback, listener)) {
		listener->callback = NULL;
		listener->user_data = NULL;

		return SENSOR_ERROR_OPERATION_FAILED;
	}

	_D("success sensor_listener_set_event");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_unset_event_cb(sensor_listener_h listener)
{
	int id;
	int type;
	unsigned int event_id;

	_D("called sensor_unregister_event : listener[0x%x]", listener);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;
	type = (int)listener->type;
	event_id = type << SENSOR_SHIFT_TYPE | 0x1;

	if (!sensord_unregister_event(id, event_id))
		return SENSOR_ERROR_OPERATION_FAILED;

	listener->callback = NULL;
	listener->user_data = NULL;

	_D("success sensor_unregister_event");

	return SENSOR_ERROR_NONE;
}

static void accuracy_changed_callback(sensor_t sensor,
		unsigned long long timestamp, int accuracy, void *data)
{
	sensor_listener_h listener = (sensor_listener_h)data;

	if (!sensor || !listener->accu_callback)
		return;

	((sensor_accuracy_changed_cb)listener->accu_callback)
			(sensor, timestamp, (sensor_data_accuracy_e)accuracy, listener->accu_user_data);

	return;
}

int sensor_listener_set_accuracy_cb(sensor_listener_h listener,
		sensor_accuracy_changed_cb callback, void *data)
{
	int id;

	_D("called sensor_register_accuracy_cb : listener[0x%x], callback[0x%x], user_data[0x%x] cb[%p]",
			listener, callback, data, accuracy_changed_callback);

	if (!listener || !callback)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;
	listener->accu_callback = (void *)callback;
	listener->accu_user_data = data;

	if (!sensord_register_accuracy_cb(id, accuracy_changed_callback, listener)) {
		listener->accu_callback = NULL;
		listener->accu_user_data = NULL;

		return SENSOR_ERROR_OPERATION_FAILED;
	}

	_D("success sensor_register_accuracy_cb");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_unset_accuracy_cb(sensor_listener_h listener)
{
	int id;

	_D("called sensor_unregister_accuracy_cb : listener[0x%x]", listener);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;

	if (!sensord_unregister_accuracy_cb(id))
		return SENSOR_ERROR_OPERATION_FAILED;

	listener->accu_callback = NULL;
	listener->accu_user_data = NULL;

	_D("success sensor_unregister_accuracy_cb");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_set_interval(sensor_listener_h listener, unsigned int interval)
{
	int id;
	int type;
	unsigned int event_id;

	_D("called sensor_set_interval : listener[0x%x], interval[%d]", listener, interval);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;
	type = (int)listener->type;
	event_id = type << SENSOR_SHIFT_TYPE | 0x1;

	if (!sensord_change_event_interval(id, event_id, interval))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_set_interval");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_set_max_batch_latency(sensor_listener_h listener, unsigned int max_batch_latency)
{
	int id;
	int type;
	int max_batch_count;
	unsigned int event_id;

	_D("called sensor_set_max_batch_latency : listener[0x%x], max_batch_latency[%d]", listener, max_batch_latency);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_max_batch_count(listener->sensor, &max_batch_count))
		return SENSOR_ERROR_OPERATION_FAILED;

	if (max_batch_count == 0)
		return SENSOR_ERROR_NOT_SUPPORTED;

	id = listener->id;
	type = (int)listener->type;
	event_id = type << SENSOR_SHIFT_TYPE | 0x1;

	if (!sensord_change_event_max_batch_latency(id, event_id, max_batch_latency))
		return SENSOR_ERROR_NOT_SUPPORTED;

	listener->batch_latency = max_batch_latency;

	_D("success sensor_set_max_batch_latency");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_set_attribute_int(sensor_listener_h listener, sensor_attribute_e attribute, int value)
{
	int id;
	int error;

	_D("called sensor_set_attribute_int : listener[0x%x], attribute[%d], value[%d]", listener, attribute, value);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;

	error = sensord_set_attribute_int(id, (int)attribute, (int)value);

	if (error == -EINVAL)
		return SENSOR_ERROR_INVALID_PARAMETER;
	else if (error != SENSOR_ERROR_NONE)
		return SENSOR_ERROR_OPERATION_FAILED;

	if (attribute == SENSOR_ATTRIBUTE_PAUSE_POLICY)
		listener->pause = value;

	_D("success sensor_set_attribute_int");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_set_option(sensor_listener_h listener, sensor_option_e option)
{
	int id;

	_D("called sensor_set_option : listener[0x%x], option[%d]", listener, option);

	if (!listener)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;

	if (sensord_set_attribute_int(id, SENSOR_ATTRIBUTE_PAUSE_POLICY, CONVERT_OPTION_PAUSE_POLICY(option)) < 0)
		return SENSOR_ERROR_OPERATION_FAILED;

	listener->pause = CONVERT_OPTION_PAUSE_POLICY(option);

	_D("success sensor_set_option");

	return SENSOR_ERROR_NONE;
}

int sensor_listener_read_data(sensor_listener_h listener, sensor_event_s *event)
{
	int id;
	int type;
	sensor_data_t data;
	unsigned int data_id;

	_D("called sensor_read_data : listener[0x%x]", listener);

	if (!listener || !event)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (listener->magic != SENSOR_LISTENER_MAGIC)
		return SENSOR_ERROR_INVALID_PARAMETER;

	id = listener->id;
	type = (int)listener->type;
	data_id = type << SENSOR_SHIFT_TYPE | 0x1;

	if (!sensord_get_data(id, data_id, &data))
		return SENSOR_ERROR_OPERATION_FAILED;

	event->accuracy = data.accuracy;
	event->timestamp = data.timestamp;
	event->value_count = data.value_count;

	for (int i = 0; i < data.value_count; ++i)
		event->values[i] = data.values[i];

	_D("success sensor_read_data");

	return SENSOR_ERROR_NONE;
}

int sensor_get_name(sensor_h sensor, char** name)
{
	_D("called sensor_get_name");

	if (!sensor || !name)
		return SENSOR_ERROR_INVALID_PARAMETER;

	*name = strdup(sensord_get_name(sensor));

	_D("success sensor_get_vendor : [%s]", *name);

	return SENSOR_ERROR_NONE;
}

int sensor_get_vendor(sensor_h sensor, char** vendor)
{
	_D("called sensor_get_vendor");

	if (!sensor || !vendor)
		return SENSOR_ERROR_INVALID_PARAMETER;

	*vendor = strdup(sensord_get_vendor(sensor));

	_D("success sensor_vendor : [%s]", *vendor);

	return SENSOR_ERROR_NONE;
}

int sensor_get_type(sensor_h sensor, sensor_type_e *type)
{
	sensor_type_t _type;
	_D("called sensor_get_type");

	if (!sensor || !type)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_type(sensor, &_type))
		return SENSOR_ERROR_OPERATION_FAILED;

	*type = (sensor_type_e) _type;

	_D("success sensor_get_type : [%d]", *type);

	return SENSOR_ERROR_NONE;
}

int sensor_get_min_range(sensor_h sensor, float *min_range)
{
	_D("called sensor_get_min_range");

	if (!sensor || !min_range)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_min_range(sensor, min_range))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_get_min_range : [%d]", *min_range);

	return SENSOR_ERROR_NONE;
}

int sensor_get_max_range(sensor_h sensor, float *max_range)
{
	_D("called sensor_get_max_range");

	if (!sensor || !max_range)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_max_range(sensor, max_range))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_get_max_range : [%d]", *max_range);

	return SENSOR_ERROR_NONE;
}

int sensor_get_resolution(sensor_h sensor, float *resolution)
{
	_D("called sensor_get_resolution");

	if (!sensor || !resolution)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_resolution(sensor, resolution))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_get_resolution : [%d]", *resolution);

	return SENSOR_ERROR_NONE;
}

int sensor_get_min_interval(sensor_h sensor, int *min_interval)
{
	_D("called sensor_get_min_interval");

	if (!sensor || !min_interval)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_min_interval(sensor, min_interval))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_get_min_interval : [%d]", *min_interval);

	return SENSOR_ERROR_NONE;
}

int sensor_get_fifo_count(sensor_h sensor, int *fifo_count)
{
	_D("called sensor_get_fifo_count");

	if (!sensor || !fifo_count)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_fifo_count(sensor, fifo_count))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_get_fifo_count : [%d]", *fifo_count);

	return SENSOR_ERROR_NONE;
}

int sensor_get_max_batch_count(sensor_h sensor, int *max_batch_count)
{
	_D("called sensor_get_max_batch_count");

	if (!sensor || !max_batch_count)
		return SENSOR_ERROR_INVALID_PARAMETER;

	if (!sensord_get_max_batch_count(sensor, max_batch_count))
		return SENSOR_ERROR_OPERATION_FAILED;

	_D("success sensor_get_max_batch_count : [%d]", *max_batch_count);

	return SENSOR_ERROR_NONE;
}

/*
 *	FUNCTIONS : SENSOR_UTIL_*
 */

int sensor_util_get_declination(float latitude, float longitude, float altitude, float *declination)
{
	if (!declination)
		return SENSOR_ERROR_INVALID_PARAMETER;

	setCoordinate(latitude, longitude, altitude, declination, NULL, 1);

	return SENSOR_ERROR_NONE;
}

int sensor_util_get_angle_change(float R[], float prevR[], float angleChange[])
{
	if (getAngleChange(R, prevR, angleChange) < 0)
		return SENSOR_ERROR_INVALID_PARAMETER;

	return SENSOR_ERROR_NONE;
}

int sensor_util_get_orientation(float R[], float values[])
{
	if (!R || !values)
		return SENSOR_ERROR_INVALID_PARAMETER;

	values[0] = (float) atan2(R[1], R[4]);
	values[1] = (float) asin(-R[7]);
	values[2] = (float) atan2(-R[6], R[8]);

	return SENSOR_ERROR_NONE;
}

int sensor_util_get_inclination(float I[], float* inclination)
{
	if (!I || !inclination)
		return SENSOR_ERROR_INVALID_PARAMETER;

	*inclination = atan2(I[5], I[4]);

	return SENSOR_ERROR_NONE;
}

int sensor_util_remap_coordinate_system(float inR[], sensor_util_axis_e x, sensor_util_axis_e y, float outR[])
{
	if (remapCoordinateSystem(inR, CONVERT_AXIS_ENUM(x), CONVERT_AXIS_ENUM(y), outR) < 0)
		return SENSOR_ERROR_INVALID_PARAMETER;

	return SENSOR_ERROR_NONE;
}

int sensor_util_get_rotation_matrix_from_vector(float Vx, float Vy, float Vz, float R[])
{
	float RV[4] = {0, Vx, Vy, Vz};

	RV[0] = 1 - Vx * Vx - Vy * Vy - Vz * Vz;
	RV[0] = (Vx > 0) ? (float) (sqrt(Vx)) : 0;

	if (quatToMatrix(RV, R) < 0)
		return SENSOR_ERROR_INVALID_PARAMETER;

	return SENSOR_ERROR_NONE;
}

int sensor_util_get_rotation_matrix(float Gx, float Gy, float Gz, float Mx, float My, float Mz, float R[], float I[])
{
	float G[3] = {Gx, Gy, Gz};
	float M[3] = {Mx, My, Mz};

	if (getRotationMatrix(G, M, R, I) < 0)
		return SENSOR_ERROR_INVALID_PARAMETER;

	return SENSOR_ERROR_NONE;
}

