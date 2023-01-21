#include "logging.h"
#include "cmsis_os.h"
#include "usart.h"
#include "string_list.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>


#define TASK_NAME "Logging Task"
#define MAX_TX_TIME 50
#define VERBOSE_LOGGING true


static StringList messageBuffer = NULL;
static SemaphoreHandle_t txCpltSemaphore = NULL;
static bool sendingInProgress;


static void LoggingTask();
static void LogWithPrefix(const char * prefix, const char * message);
static void StartSendingNextMessage();

bool Setup_Logging_CreateTask()
{
	BaseType_t res = xTaskCreate(LoggingTask, TASK_NAME, 128, NULL, 0, NULL);
	if(res != pdPASS)
	{
		Logging_LogBlocking("[ERROR]" TASK_NAME " was not created.\r\n");
		return false;
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
	StartSendingNextMessage();
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
	char buffer_stats[30];
#if VERBOSE_LOGGING == true
	snprintf(buffer_stats, sizeof(buffer_stats), "(msg: %d, chr: %d): ",
			(int)StringList_GetElemsCount(messageBuffer) + 1,
			(int)StringList_GetCharsCount(messageBuffer) +
			strlen(prefix) + strlen(message));
#endif
	uint16_t len = strlen(prefix) + strlen(message) + strlen(buffer_stats) + 1;
	char msgWithPrefix[len];
	strcpy(msgWithPrefix, prefix);
	strcat(msgWithPrefix, buffer_stats);
	strcat(msgWithPrefix, message);
	Logging_Log(msgWithPrefix);
}

static void LoggingTask()
{
	txCpltSemaphore = xSemaphoreCreateBinary();
	assert(txCpltSemaphore != NULL);

	StartSendingNextMessage();

	while(true)
	{

		if(xSemaphoreTake(txCpltSemaphore, pdMS_TO_TICKS(MAX_TX_TIME)) == pdPASS)
		{
			//sending completed
			StringList_DeleteElem(&messageBuffer);
			sendingInProgress = false;
			StartSendingNextMessage();
		}
		else
		{
			if(UART_IsTransmitting(&huart2) == false) //nothing is being sent now
			{
				//in case something is left in the buffer
				if(StringList_GetFirstElem(messageBuffer) != NULL)
				{
					sendingInProgress = false;
					StartSendingNextMessage();
				}
			}
		}
	}
}

static void StartSendingNextMessage()
{
	char * msg = StringList_GetFirstElem(messageBuffer);
	if(msg == NULL)
		return;

	HAL_StatusTypeDef res = HAL_OK;
	if(sendingInProgress == false)
	{
		sendingInProgress = true;
		res =  HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));
	}
	assert(res == HAL_OK);
}

void ISR_LoggingUARTTx()
{
	BaseType_t higherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(txCpltSemaphore, &higherPriorityTaskWoken);
	portYIELD_FROM_ISR(higherPriorityTaskWoken);
}
