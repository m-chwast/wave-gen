/*
 * program.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Mateusz Chwast
 */

#pragma once

#include "Tasks/Logging/task_logging.h"


//all the main tasks are here
class Program final {
private:

	static Console _console;

	static TaskLogging* _taskLogging;


public:

	static void CreateTasks();
};
