#ifndef _SCREEN_MENU_H_
#define _SCREEN_MENU_H_

#include "encoder.h"

void Menu_Init();
void Menu_Display();
void Menu_SelectNext(Encoder_Speed speed);
void Menu_SelectPrev(Encoder_Speed speed);
void Menu_InvokeAction();	//called when button is pressed
void Menu_GoToSubmenu();
void Menu_GoToParent();


#endif
