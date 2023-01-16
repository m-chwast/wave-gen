#include "logging.h"
#include "cmsis_os.h"
#include "usart.h"
#include <string.h>


#define TASK_NAME "Logging Task"


struct MessageBuffer
{
	const char ** messages;
	uint8_t count;
};

volatile static const char * currentMessage;

static void LoggingTask();


bool Setup_Logging_CreateTask()
{
	BaseType_t res = xTaskCreate(LoggingTask, TASK_NAME, 128, NULL, 0, NULL);
	if(res != pdPASS)
	{
		return false;
		Logging_LogBlocking("[ERROR]" TASK_NAME " was not created.\r\n");
	}
		Logging_Info(TASK_NAME " was created successfully\r\n");
	return true;
}

void Logging_LogBlocking(const char * message)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
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
	struct MessageBuffer messageBuffer;

	while(true)
	{
		//TODO
	}
}

void ISR_LoggingUARTTx()
{

}
