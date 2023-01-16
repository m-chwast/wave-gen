#include "logging.h"
#include "cmsis_os.h"
#include "usart.h"


#define TASK_NAME "Logging Task"


static void LoggingTask();


bool Setup_Logging_CreateTask()
{
	BaseType_t res = xTaskCreate(LoggingTask, TASK_NAME, 128, NULL, 0, NULL);
	if(res != pdPASS)
		return false;
	Logging_Info(TASK_NAME "was created successfully");
	return true;
}

void Logging_Log(const char * message)
{
	//TODO
}

void Logging_Info(const char * message)
{
	//TODO
}

void Logging_Warn(const char * message)
{
	//TODO
}

void Logging_Error(const char * message)
{
	//TODO
}


static void LoggingTask()
{
	while(true)
	{
		//TODO
	}
}
