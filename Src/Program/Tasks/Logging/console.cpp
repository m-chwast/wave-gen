/*
 * console.cpp
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include "Tasks/Logging/console.h"
#include <cstring>

void Console::Write(const char* msg) {
	assert(msg != nullptr);

	size_t len = strlen(msg);
	assert(len < _queueLength * _queueElemSize);

	const char* currentStr = msg;

	for(uint32_t i = 0; i <= len / _queueElemSize; i++) {
		if(i != 0) {
			currentStr += _queueElemSize - 1;
		}

		char tmp[_queueElemSize] = { 0 };
		strncpy(tmp, currentStr, _queueElemSize - 1);
		xQueueSend(GetQueue(), tmp, portMAX_DELAY);
	}
}


