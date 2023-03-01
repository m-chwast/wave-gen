#include "serial_receiver.h"
#include "logging.h"
#include "FreeRTOS.h"
#include "task.h"


#define TASK_NAME "Serial Rx Task"
#define TASK_STACK_SIZE 256


static void ReceiverTask();


bool Setup_SerialReceiver_CreateTask()
{
	BaseType_t res;
	res = xTaskCreate(ReceiverTask, TASK_NAME, TASK_STACK_SIZE, NULL, 0, NULL);
	if(res != pdPASS)
	{
		Logging_LogBlocking("[ERROR]" TASK_NAME " was not created.\r\n");
		return false;
	}
	Logging_Info(TASK_NAME " was created successfully\r\n");
	return true;
}

static void ReceiverTask()
{

	while(true)
	{
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
