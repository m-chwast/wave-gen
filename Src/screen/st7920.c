/*low level driver for ST7920 LCD by Mateusz Chwast
*
*/

#include "screen/st7920.h"
#include "tim.h"
#include <string.h>


/////commands:
//command macros
#define INT_TO_BOOL(X) ((X) ? 1 : 0)
//basic instructions:
#define CLEAR 0b1
#define HOME 0b10
#define ENTRY_MODE(I_D,S) (0b100 + (INT_TO_BOOL((I_D)) << 1) + INT_TO_BOOL((S)))
#define DISPLAY_ON_OFF(D,C,B) (0b1000 + (INT_TO_BOOL((D)) << 2) + (INT_TO_BOOL((C)) << 1) + INT_TO_BOOL((B)))
#define CURSOR_DISPLAY_CONTROL(S_C,R_L) (0b10000 + (INT_TO_BOOL((S_C)) << 3) + (INT_TO_BOOL((R_L)) << 2))
#define FUNCTION_SET(DL,RE) (0b100000 + (INT_TO_BOOL((DL)) << 4) + (INT_TO_BOOL((RE)) << 2))
#define SET_CGRAM_ADDR(ADDR) (0b1000000 + (ADDR))
#define SET_DDRAM_ADDR(ADDR) (0b10000000 + (uint8_t)(ADDR))
//extended instruction set:
#define STAND_BY CLEAR
#define SCROLL 0b11
#define REVERSE(R1,R0) (0b100 + (INT_TO_BOOL((R1)) << 1) + INT_TO_BOOL((R0)))
#define EXTENDED_FUNCTION_SET(DL, G) (0b100100 + (INT_TO_BOOL((DL)) << 4) + (INT_TO_BOOL((G)) << 1))
#define SET_IRAM_ADDR(ADDR) SET_CGRAM_ADDR((ADDR))
#define SET_GDRAM_ADDR(ADDR) SET_DDRAM_ADDR((ADDR))

#define ST7920_DATA_WAIT_US 80
#define ST7920_TIMER htim10


static void SendBlocking(uint8_t cmd, bool rs);
static inline void SendNibble(uint8_t nibble);
static void SendByte(uint8_t byteToSend, bool rs);


void ST7920_SetFunctionSet()
{
	SendBlocking(FUNCTION_SET(0,0), 0);
}

void ST7920_SetEntryMode()
{
	SendBlocking(ENTRY_MODE(1,0), 0);
}

void ST7920_SetupDisplay()
{
	SendBlocking(DISPLAY_ON_OFF(1,0,0), 0);
}

void ST7920_SendHomeCommand()
{
	SendBlocking(HOME, 0);
}

void ST7920_SendClearCommand()
{
	SendBlocking(CLEAR, 0);
}

void ST7920_SetResetPinState(bool state)
{
	GPIO_PinState pinState = state == true ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, pinState);
}

void ST7920_SendSetupNibble()
{
	SendNibble(0b0010);
}

void ST7920_SetAddress(uint8_t addr)
{
	SendByte(SET_GDRAM_ADDR(addr), 0);
}

void ST7920_SetTextModeBlocking()
{
	SendBlocking(EXTENDED_FUNCTION_SET(0, 0), 0);
	SendBlocking(FUNCTION_SET(0, 0), 0);
}

void ST7920_SetGraphicModeBlocking()
{
	SendBlocking(EXTENDED_FUNCTION_SET(0, 0), 0);
	SendBlocking(EXTENDED_FUNCTION_SET(0, 1), 0);
}

void ST7920_SendDataByte(uint8_t data)
{
	SendByte(data, 1);
}

static inline void SendNibble(uint8_t nibble)
{
	LCD_E_GPIO_Port->BSRR = LCD_E_Pin;	//set E
	//set data bits
	LCD_D4_GPIO_Port->BSRR = (nibble & 0b1) ? LCD_D4_Pin : 0;
	LCD_D5_GPIO_Port->BSRR = (nibble & 0b10) ? LCD_D5_Pin : 0;
	LCD_D6_GPIO_Port->BSRR = (nibble & 0b100) ? LCD_D6_Pin : 0;
	LCD_D7_GPIO_Port->BSRR = (nibble & 0b1000) ? LCD_D7_Pin : 0;

	LCD_E_GPIO_Port->BRR = LCD_E_Pin;	//reset E

	LCD_D4_GPIO_Port->BRR = LCD_D4_Pin;	//reset data bits
	LCD_D5_GPIO_Port->BRR = LCD_D5_Pin;	//reset data bits
	LCD_D6_GPIO_Port->BRR = LCD_D6_Pin;	//reset data bits
	LCD_D7_GPIO_Port->BRR = LCD_D7_Pin;	//reset data bits
}

static void SendByte(uint8_t byteToSend, bool rs)
{
	if(rs != false)
		LCD_RS_GPIO_Port->BSRR = LCD_RS_Pin;

	SendNibble(byteToSend >> 4);
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	SendNibble(byteToSend & 0x0F);

	LCD_RS_GPIO_Port->BRR = LCD_RS_Pin;
}

static void SendBlocking(uint8_t cmd, bool rs)
{
	SendByte(cmd, rs);
	//HAL_Delay(1);
}
