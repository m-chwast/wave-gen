#include "screen/menu.h"
#include "screen/lcd_ll.h"
#include <stddef.h>


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
