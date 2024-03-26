/*
 * task_logging.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Mateusz Chwast
 */

#pragma once

#include "Tasks/task_base.h"
#include "usart.h"
#include <cstring>
#include "Tasks/Logging/console.h"

class TaskLogging final : public Task {
private:

	constexpr static const char* _name = "Logging";
	constexpr static uint16_t _stackSize = 128;
	constexpr static uint8_t _priority = 1;

	UART_HandleTypeDef& _huart;

	Console& _console;

	void Loop() override;

	void Transmit(const char* msg);

public:

	TaskLogging(UART_HandleTypeDef& huart, Console& console)
		: Task(_name, _stackSize, _priority),
		  _huart{huart}, _console{console} {}
};
