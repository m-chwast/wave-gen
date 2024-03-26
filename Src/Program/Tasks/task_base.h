/*
 * task.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Mateusz Chwast
 */

#pragma once

#include "FreeRTOS.h"
#include "task.h"

class Task {
private:
	TaskHandle_t handle;

	//Wrapper that invokes the member function runner on passed object
	static void RunWrapper(void* obj) {
		static_cast<Task*>(obj)->TaskRunner(nullptr);
	}

	void TaskRunner(void* param) {
		Init();
		while(true) {
			Loop();
		}
	}

	virtual void Init() {}

	virtual void Loop() = 0;

public:

	Task(const char* name, uint32_t stackSize, uint8_t priority) {
		xTaskCreate(&Task::RunWrapper, name, stackSize, this, priority, &handle);
	}

	virtual ~Task() {
#if INCLUDE_vTaskDelete
		vTaskDelete(handle);
#endif
	}
};
