#include "encoder.h"
#include "logging.h"
#include "FreeRTOS.h"
#include "task.h"


#define TASK_STACK_SIZE 256
#define TASK_NAME "Encoder Task"
#define TASK_PRIORITY 1


static void EncoderTask();


bool Setup_Encoder_CreateTask()
{
	BaseType_t res =  xTaskCreate(EncoderTask, TASK_NAME, TASK_STACK_SIZE,
			NULL, TASK_PRIORITY, NULL);
	if(res != pdPASS)
	{
		Logging_Error(TASK_NAME " could not be created!\r\n");
		return false;
	}
	Logging_Info(TASK_NAME " created\r\n");
	return true;
}


static void EncoderTask()
{

	while(true)
	{

	}
}
