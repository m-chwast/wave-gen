#ifndef INC_SCREEN_ST7920_LL_H
#define INC_SCREEN_ST7920_LL_H


#include <stdint.h>


void ST7920_Init();

void ST7920_SendText(const char * text, uint8_t x, uint8_t y);
void ST7920_SetTextPos(uint8_t x, uint8_t y);
void ST7920_SendStringBlocking(const char * str);
void ST7920_ClearAll();


#endif
