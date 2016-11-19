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

#ifndef _GPIO_LOG_H_
#define _GPIO_LOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

#define _DEBUG 1

#undef LOG_TAG
#define LOG_TAG	"TIZEN_SYSTEM_GPIO"

#define _MSG_GPIO_ERROR_IO_ERROR "Io Error"
#define _MSG_GPIO_ERROR_INVALID_PARAMETER "Invalid Parameter"
#define _MSG_GPIO_ERROR_OUT_OF_MEMORY "Out of Memory"
#define _MSG_GPIO_ERROR_NOT_NEED_CALIBRATION "Not need calibration"
#define _MSG_GPIO_ERROR_NOT_SUPPORTED "Not supported"
#define _MSG_GPIO_ERROR_OPERATION_FAILED "Operation failed"

//#define _E_MSG(err) SLOGE(_MSG_##err "(0x%08x)", (err))

	#define _E(...)
	#define _W(...)
	#define _I(...)
	#define _D(...)


#ifdef __cplusplus
}
#endif

#endif /*_GPIO_LOG_H_*/
