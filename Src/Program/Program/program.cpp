#include "Program/cprogram.h"
#include "Program/program.h"
#include "FreeRTOS.h"
#include "usart.h"


//function called from C code
void Program_CreateTasks(void) {
	Program::CreateTasks();
}

//cpp logic

void* operator new(size_t size) {
	return pvPortMalloc(size);
}

void* operator new[](size_t size) {
    return pvPortMalloc(size);
}

void operator delete(void* p) {
	vPortFree(p);
}

void operator delete[](void* p) {
	vPortFree(p);
}

// program logic

Console Program::_console;
TaskLogging* Program::_taskLogging;

void Program::CreateTasks() {
	_taskLogging = new TaskLogging(huart2, _console);

	_console.Write("Tasks created!\r\n");
}
