#include "serial_receiver.h"
#include "logging.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include <string.h>


#define TASK_NAME "Serial Rx Task"
#define TASK_STACK_SIZE 256

#define BUFFER_SIZE 256
#define TERMINATING_CHAR '\r'


struct RxBuffer
{
	uint8_t data[BUFFER_SIZE];
	uint16_t currentPos;
};


static volatile struct RxBuffer rxBuffer;
static TaskHandle_t receiverTask = NULL;


static void ReceiverTask();


bool Setup_SerialReceiver_CreateTask()
{
	BaseType_t res;
	res = xTaskCreate(ReceiverTask, TASK_NAME, TASK_STACK_SIZE, NULL, 0, &receiverTask);
	if(res != pdPASS || receiverTask == NULL)
	{
		Logging_LogBlocking("[ERROR]" TASK_NAME " was not created.\r\n");
		return false;
	}
	Logging_Info(TASK_NAME " was created successfully\r\n");
	return true;
}

static void BufferClear()
{
	memset((uint8_t *)rxBuffer.data, 0, BUFFER_SIZE);
	rxBuffer.currentPos = 0;
}

static void RequestNextRead(uint16_t charsToRead)
{
	HAL_UART_Receive_IT(&huart2,
			(uint8_t *)(rxBuffer.data + rxBuffer.currentPos), charsToRead);
}

static void ReceiverTask()
{
	BufferClear();
	RequestNextRead(1);

	while(true)
	{
		//wait indefinitely for a full message (finished with terminating char)
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		//replace term char with new line for convenient displaying
		rxBuffer.data[rxBuffer.currentPos] = '\n';
		Logging_Info((char *)rxBuffer.data);
		BufferClear();
		RequestNextRead(1);
	}
}

void ISR_SerialReceiver_RxCplt()
{
	if(rxBuffer.currentPos >= BUFFER_SIZE)
	{
		BufferClear();
		return;
	}
	if(rxBuffer.data[rxBuffer.currentPos] == TERMINATING_CHAR)
	{
		BaseType_t higherPriorityTaskWoken = pdFAIL;
		//notify task, don't start new rx operation
		vTaskNotifyGiveFromISR(receiverTask, &higherPriorityTaskWoken);
		portYIELD_FROM_ISR(higherPriorityTaskWoken);
		return;
	}

	rxBuffer.currentPos++;
	RequestNextRead(1);
}
