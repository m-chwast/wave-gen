#include "logging.h"
#include "cmsis_os.h"
#include "usart.h"
#include "string_list.h"
#include <string.h>


#define TASK_NAME "Logging Task"


StringList messageBuffer = NULL;


static void LoggingTask();
static void LogWithPrefix(const char * prefix, const char * message);

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
	StringList_AddElem(&messageBuffer, message);
}

void Logging_Info(const char * message)
{
	LogWithPrefix("[INFO]", message);
}

void Logging_Warn(const char * message)
{
	LogWithPrefix("[WARN]", message);
}

void Logging_Error(const char * message)
{
	LogWithPrefix("[ERROR]", message);
}

static void LogWithPrefix(const char * prefix, const char * message)
{
	char msgWithPrefix[strlen(prefix) + strlen(message) + 1];
	strcpy(msgWithPrefix, prefix);
	strcat(msgWithPrefix, message);
	Logging_Log(msgWithPrefix);
}

static void LoggingTask()
{


	while(true)
	{
		//TODO
	}
}

void ISR_LoggingUARTTx()
{

}
