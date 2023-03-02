#include "screen/menu.h"
#include "screen/lcd_ll.h"
#include <string.h>


typedef enum
{
	MENU_TYPE_TEXT,
	MENU_TYPE_SELECTABLE,
} MenuElementType;

typedef struct MenuElementStruct
{
	const char * text;
	MenuElementType type;
	void (*callback)(void);
	const struct MenuElementStruct * parent;
	const struct MenuElementStruct * submenu;
	const struct MenuElementStruct * prev;
	const struct MenuElementStruct * next;
} MenuElement;


static const MenuElement menuRun;
static const MenuElement menuMode;

static const MenuElement menuRun =
{
		.text = "Run",
		.type = MENU_TYPE_SELECTABLE,
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = NULL,
		.next = &menuMode,
};

static const MenuElement menuMode =
{
		.text = "Mode Select",
		.type = MENU_TYPE_TEXT,
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = &menuRun,
		.next = NULL,
};


//initialize with start menu
static const MenuElement * currentMenu = &menuRun;
static uint8_t currentLine = 0;


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

void Menu_Display()
{
	const MenuElement * menuElem = FindFirstElemToDisplay();
	if(menuElem == NULL)
		return;

	ST7920_ClearAll();

	char lineToDisplay[16] = {0};

	for(uint8_t i = 0; i < 4; i++)
	{
		memset(lineToDisplay, '\0', sizeof(lineToDisplay));
		if(menuElem == currentMenu)
			strcpy(lineToDisplay, ">");
		strncat(lineToDisplay, menuElem->text, sizeof(lineToDisplay) - 1);
		ST7920_SendText(lineToDisplay, 1, i);
		if(menuElem->next == NULL)
			break;
		menuElem = menuElem->next;
	}
}
