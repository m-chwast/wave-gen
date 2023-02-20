#include "screen/lcd.h"
#include "screen/lcd_ll.h"
#include "logging.h"
#include "FreeRTOS.h"
#include "task.h"


#define TASK_STACK_SIZE 256
#define TASK_NAME "LCD Task"
#define TASK_PRIORITY 1
#define TASK_DELAY_TIME pdMS_TO_TICKS(50)


static void LcdTask();


bool Setup_Lcd_CreateTask()
{
	BaseType_t res =  xTaskCreate(LcdTask, TASK_NAME, TASK_STACK_SIZE,
			NULL, TASK_PRIORITY, NULL);
	if(res != pdPASS)
	{
		Logging_Error(TASK_NAME " could not be created!\r\n");
		return false;
	}
	Logging_Info(TASK_NAME " created\r\n");
	return true;
}


static void LcdTask()
{
	ST7920_Init();
	ST7920_SendStringBlocking("Hello World!");
	vTaskDelay(pdMS_TO_TICKS(1000));
	ST7920_SendStringBlocking("Now this!");

	while(true)
	{
		vTaskDelay(TASK_DELAY_TIME);
	}
}
