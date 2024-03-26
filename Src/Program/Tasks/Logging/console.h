/*
 * console.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Mateusz Chwast
 */

#pragma once

#include "FreeRTOS.h"
#include "queue.h"

class Console {
private:

	QueueHandle_t _queue;
	constexpr static uint16_t _queueLength = 20;
	constexpr static uint16_t _queueElemSize = 50;

public:

	Console() {
		_queue = xQueueCreate(_queueLength, _queueElemSize);
	}

	void Write(const char* msg);

	QueueHandle_t GetQueue() const { return _queue; }
	constexpr static size_t GetMaxElemSize() { return _queueElemSize; }
};
