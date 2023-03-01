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
	vTaskDelay(pdMS_TO_TICKS(10));
}

void ST7920_SendStringBlocking(const char * str)
{
	for(uint8_t i = 0; i < strlen(str); i++)
	{
		ST7920_SendDataByte(str[i]);
		HAL_Delay(1);
	}
}

void ST7920_SendText(const char * text, uint8_t x, uint8_t y)
{
	ST7920_SetTextPos(x, y);
	vTaskDelay(1);
	if(strlen(text) % 2)
	{
		ST7920_SendDataByte(' ');
		vTaskDelay(1);
	}
	for(uint8_t i = 0; i < strlen(text); i++)
	{
		ST7920_SendDataByte(text[i]);
		vTaskDelay(1);
	}

}

void ST7920_SetTextPos(uint8_t x, uint8_t y)
{
	if(x > 15)
		x = 15;
	if(y > 3)
		y = 3;

	uint8_t addr = x / 2;
	switch(y)
	{
		case 1:
			addr += 16;
			break;
		case 2:
			addr += 8;
			break;
		case 3:
			addr += 24;
			break;
		default:
			break;
	}
	ST7920_SetAddress(addr);
}
