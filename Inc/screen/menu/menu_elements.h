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
	MenuElementProperties properties;
	MenuElementValue value;
	void (*callback)(void);
	const struct MenuElementStruct * parent;
	const struct MenuElementStruct * submenu;
	const struct MenuElementStruct * prev;
	const struct MenuElementStruct * next;
} MenuElement;


extern const MenuElement * const MenuElements_initialMenuElement;
