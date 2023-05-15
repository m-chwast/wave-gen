#pragma once

#include "screen/menu/menu_elements.h"
#include "encoder.h"

void Menu_WriteValueToStr(const MenuElement * element, char * str, uint32_t maxChars);
void Menu_ChangeValue(bool increase, Encoder_Speed speed, MenuElement * element);
