#ifndef INC_SCREEN_ST7920_H
#define INC_SCREEN_ST7920_H

#include <stdbool.h>
#include <stdint.h>


void ST7920_SetGraphicModeBlocking();
void ST7920_SetTextModeBlocking();
void ST7920_SetAddress(uint8_t addr);
void ST7920_SetResetPinState(bool state);
void ST7920_SendSetupNibble();
void ST7920_SetFunctionSet();
void ST7920_SetEntryMode();
void ST7920_SetupDisplay();
void ST7920_SendClearCommand();
void ST7920_SendHomeCommand();
void ST7920_SendDataByte(uint8_t data);



#endif
