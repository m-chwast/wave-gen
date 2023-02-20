/*
 * low-level lcd api, it uses st7920 api calls but is independent from the MCU hardware
 * all functions are assumed to be called from freertos task
 */

#include "screen/lcd_ll.h"
#include "screen/st7920.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include "main.h"


void ST7920_Init()
{
	ST7920_SetResetPinState(true);
	vTaskDelay(pdMS_TO_TICKS(10));
	ST7920_SetResetPinState(false);
	vTaskDelay(pdMS_TO_TICKS(20));
	ST7920_SetResetPinState(true);
	vTaskDelay(pdMS_TO_TICKS(10));
	ST7920_SendSetupNibble();
	vTaskDelay(pdMS_TO_TICKS(10));
	ST7920_SetFunctionSet();
	vTaskDelay(pdMS_TO_TICKS(10));
	ST7920_SetupDisplay();
	vTaskDelay(pdMS_TO_TICKS(10));
	ST7920_SendClearCommand();
	vTaskDelay(pdMS_TO_TICKS(20));
	ST7920_SetEntryMode();
	vTaskDelay(pdMS_TO_TICKS(10));
	ST7920_SendHomeCommand();
	vTaskDelay(pdMS_TO_TICKS(50));
}

void ST7920_SendStringBlocking(const char * str)
{
	for(uint8_t i = 0; i < strlen(str); i++)
	{
		ST7920_SendDataByte(str[i]);
		HAL_Delay(1);
	}
	if((strlen(str) % 2) != 0)	//because single lcd address contains two characters
	{
		ST7920_SendDataByte(' ');
		HAL_Delay(1);
	}
}
