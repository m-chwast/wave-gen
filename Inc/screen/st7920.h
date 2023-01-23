#ifndef INC_SCREEN_ST7920_H
#define INC_SCREEN_ST7920_H

#include <stdbool.h>
#include <stdint.h>

void ST7920_Init();


void ST7920_SetGraphicModeBlocking();
void ST7920_SetTextModeBlocking();
void ST7920_SendStringBlocking(const char * str);
void ST7920_SetAddress(uint8_t addr);


#endif
