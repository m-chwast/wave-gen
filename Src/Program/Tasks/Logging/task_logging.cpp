/*
 * task_logging.cpp
 *
 *  Created on: Oct 17, 2023
 *      Author: Mateusz Chwast
 */

#include "FreeRTOS.h"
#include "task.h"
#include "Tasks/Logging/task_logging.h"


void TaskLogging::Loop() {
	char buff[Console::GetMaxElemSize()] = { 0 };

	if(xQueueReceive(_console.GetQueue(), buff, pdMS_TO_TICKS(2500)) == pdPASS) {
		Transmit(buff);
	}
	else {
		Transmit("ping!");
	}
}

void TaskLogging::Transmit(const char* msg) {
	//TODO non-blocking transmission
	taskENTER_CRITICAL();
	const uint8_t* tmpMsg = reinterpret_cast<const uint8_t*>(msg);
	HAL_UART_Transmit(&_huart, const_cast<uint8_t*>(tmpMsg), strlen(msg), HAL_MAX_DELAY);
	taskEXIT_CRITICAL();
}
