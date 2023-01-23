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


void ST7920_Init()
{
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	SendNibble(0b0010);
	HAL_Delay(1);
	SendBlocking(FUNCTION_SET(0,0), 0);
	SendBlocking(DISPLAY_ON_OFF(1,0,0), 0);
	SendBlocking(CLEAR, 0);
	HAL_Delay(15);
	SendBlocking(ENTRY_MODE(1,0), 0);
	HAL_Delay(5);
	SendBlocking(HOME, 0);
	HAL_Delay(200);
	SendBlocking(CLEAR, 0);
	HAL_Delay(5);
	ST7920_SetTextModeBlocking();
	HAL_Delay(50);
}

void ST7920_SetAddress(uint8_t addr)
{
	SendByte(SET_GDRAM_ADDR(addr), 0);
}

void ST7920_SendStringBlocking(const char * str)
{
	for(uint8_t i = 0; i < strlen(str); i++)
		SendBlocking(str[i], 1);
	if((strlen(str) % 2) != 0)	//because single lcd address contains two characters
		SendBlocking(' ', 1);
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

static inline void SendNibble(uint8_t nibble)
{
	LCD_E_GPIO_Port->BSRR = LCD_E_Pin;	//set E
	LCD_D4_GPIO_Port->BSRR = LCD_D4_Pin & (nibble & 0b1);	//set data bits
	LCD_D5_GPIO_Port->BSRR = LCD_D5_Pin & (nibble & 0b01);
	LCD_D6_GPIO_Port->BSRR = LCD_D6_Pin & (nibble & 0b001);
	LCD_D7_GPIO_Port->BSRR = LCD_D7_Pin & (nibble & 0b0001);

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
	HAL_Delay(1);
}
