#include <screen/menu/menu.h>
#include <screen/menu/menu_elements.h>
#include "screen/lcd_ll.h"
#include "screen/lcd.h"
#include <string.h>


static void AppendValueToLine(const MenuElement * element, char * buff, uint32_t buffSize);


//initialize with start menu
static const MenuElement * currentMenu = NULL;
static uint8_t currentLine = 0;
static bool clearBeforeDrawing = true;


static const MenuElement * FindFirstElemToDisplay()
{
	if(currentLine > 3)
		currentLine = 3;
	if(currentMenu == NULL)
		return NULL;

	const MenuElement * first = currentMenu;
	for(int i = 0; i < currentLine; i++)
	{
		if(first->prev == NULL)
			break;
		first = first->prev;
	}
	return first;
}

void Menu_Init()
{
	currentMenu = MenuElements_initialMenuElement;
}

void Menu_Display()
{
	const MenuElement * menuElem = FindFirstElemToDisplay();
	if(menuElem == NULL)
		return;

	if(clearBeforeDrawing != false)
	{
		ST7920_ClearAll();
		clearBeforeDrawing = false;
	}

	char lineToDisplay[16] = {0};

	for(uint8_t i = 0; i < 4; i++)
	{
		memset(lineToDisplay, '\0', sizeof(lineToDisplay));
		strcpy(lineToDisplay, (menuElem == currentMenu) ? ">" : " ");
		strncat(lineToDisplay, menuElem->text, sizeof(lineToDisplay) - 2);
		if(menuElem->properties.valueType != VALUE_TYPE_NONE)
		{
			if(strlen(lineToDisplay) < sizeof(lineToDisplay) - 1)
				strcat(lineToDisplay, ": ");
			AppendValueToLine(menuElem, lineToDisplay, sizeof(lineToDisplay) - 1);
		}

		ST7920_SendText(lineToDisplay, 0, i);
		if(menuElem->next == NULL)
			break;
		menuElem = menuElem->next;
	}
}

void Menu_SelectNext()
{
	if(currentLine < 3)
		currentLine++;
	else
		clearBeforeDrawing = true;

	if(currentMenu->next != NULL)
	{
		currentMenu = currentMenu->next;
	}
	else
	{
		const MenuElement * tmp = currentMenu;
		while(tmp->prev != NULL)
			tmp = tmp->prev;
		currentLine = 0;
		currentMenu = tmp;
	}
	Lcd_RefreshRequest();
}

void Menu_SelectPrev()
{
	if(currentLine > 0)
		currentLine--;

	if(currentMenu->prev != NULL)
	{
		currentMenu = currentMenu->prev;
	}
	else
	{
		const MenuElement * tmp = currentMenu;
		uint8_t menuCnt = 0;
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
			menuCnt++;
		}
		if(menuCnt > 3)
		{
			currentLine = 3;
			clearBeforeDrawing = true;
		}
		else
		{
			currentLine = menuCnt;
		}
		currentMenu = tmp;
	}
	Lcd_RefreshRequest();
}

void Menu_GoToSubmenu()
{
	if(currentMenu->submenu == NULL)
		return;
	currentMenu = currentMenu->submenu;
	currentLine = 0;
	clearBeforeDrawing = true;
	Lcd_RefreshRequest();
}

void Menu_GoToParent()
{
	if(currentMenu->parent == NULL)
		return;
	currentMenu = currentMenu->parent;
	currentLine = 0;
	clearBeforeDrawing = true;
	Lcd_RefreshRequest();
}

void Menu_InvokeAction()
{
	if(currentMenu->submenu != NULL)
		Menu_GoToSubmenu();
	else if(currentMenu->parent != NULL)
		Menu_GoToParent();
}

static void AppendValueToLine(const MenuElement * element, char * buff, uint32_t buffSize)
{
	//prepare the buffer for appending
	buffSize -= strlen(buff);
	buff += strlen(buff);

	//save converted value to the buffer
	switch(element->properties.valueType)
	{
	case VALUE_TYPE_WAVE_TYPE:
	{
		const char * waveName = "";
		uint32_t val = element->value.uintData;
		if(val == 0)
			waveName = "sine";
		else if(val == 1)
			waveName = "square";
		else if(val == 2)
			waveName = "triangle";
		else if(val == 3)
			waveName = "sawtooth";
		strncpy(buff, waveName, buffSize);
		break;
	}
	default:
		break;
	}
}
