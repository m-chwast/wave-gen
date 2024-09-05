#include "screen/lcd.h"
#include "screen/lcd_ll.h"
#include "logging.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <assert.h>
#include <screen/menu/menu.h>


#define TASK_STACK_SIZE 256
#define TASK_NAME "LCD Task"
#define TASK_PRIORITY 1
#define TASK_DELAY_TIME pdMS_TO_TICKS(500)


static SemaphoreHandle_t lcdRefreshSemaphore = NULL;


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
	lcdRefreshSemaphore = xSemaphoreCreateBinary();
	assert(lcdRefreshSemaphore != NULL);

	ST7920_Init();
	Menu_Init();
	Menu_Display();

	while(true)
	{
		xSemaphoreTake(lcdRefreshSemaphore, TASK_DELAY_TIME);
		Menu_Display();
	}
}

void Lcd_RefreshRequest()
{
	xSemaphoreGive(lcdRefreshSemaphore);
}

