#pragma once

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
	VALUE_TYPE_NONE = 0,
	VALUE_TYPE_UINT,
	VALUE_TYPE_INT,
	VALUE_TYPE_FLOAT,
	VALUE_TYPE_BOOL,
	VALUE_TYPE_WAVE_TYPE,
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
	const MenuElementProperties properties;
	MenuElementValue value;
	const void (*callback)(void);
	struct MenuElementStruct * const parent;
	struct MenuElementStruct * const submenu;
	struct MenuElementStruct * const prev;
	struct MenuElementStruct * const next;
} MenuElement;


extern MenuElement * const MenuElements_initialMenuElement;
