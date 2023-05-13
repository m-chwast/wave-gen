#include "screen/menu.h"
#include "screen/lcd_ll.h"
#include "screen/lcd.h"
#include <string.h>


typedef enum
{
	VALUE_TYPE_NONE = 0,
	VALUE_TYPE_UINT,
	VALUE_TYPE_INT,
	VALUE_TYPE_FLOAT,
	VALUE_TYPE_BOOL,
} MenuElementValueType;

typedef struct
{
	MenuElementValueType valueType;
	bool isSelectable;
} MenuElementProperties;

typedef union
{
	uint32_t uintData;
	int32_t intData;
	float floatData;
	bool boolData;
} MenuElementValue;

typedef struct MenuElementStruct
{
	const char * text;
	MenuElementProperties properties;
	MenuElementValue value;
	void (*callback)(void);
	const struct MenuElementStruct * parent;
	const struct MenuElementStruct * submenu;
	const struct MenuElementStruct * prev;
	const struct MenuElementStruct * next;
} MenuElement;

static void Callback_WaveSetup_Type();

static const MenuElement menuRun;
static const MenuElement menuMode;
static const MenuElement waveSetup;
static const MenuElement waveSetup_type;
static const MenuElement waveSetup_return;

static const MenuElement menuRun =
{
		.text = "Run",
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = NULL,
		.next = &menuMode,
};

static const MenuElement menuMode =
{
		.text = "Mode Select",
		.callback = NULL,
		.parent = NULL,
		.submenu = NULL,
		.prev = &menuRun,
		.next = &waveSetup,
};

static const MenuElement waveSetup =
{
		.text = "Wave Setup...",
		.callback = NULL,
		.parent = NULL,
		.submenu = &waveSetup_type,
		.prev = &menuMode,
		.next = NULL,
};

static const MenuElement waveSetup_type =
{
		.text = "Type",
		.properties.valueType = VALUE_TYPE_UINT,
		.callback = Callback_WaveSetup_Type,
		.next = &waveSetup_return,
};

static const MenuElement waveSetup_return =
{
	.text = "Return...",
	.parent = &menuRun,
	.prev = &waveSetup_type,
};


//initialize with start menu
static const MenuElement * currentMenu = &menuRun;
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
		strncat(lineToDisplay, menuElem->text, sizeof(lineToDisplay) - 1);
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

static void Callback_WaveSetup_Type()
{

}
