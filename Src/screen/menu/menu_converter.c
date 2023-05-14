#include "screen/menu/menu_converter.h"
#include <string.h>


static void WriteWaveType(uint32_t val, char * str, uint32_t maxChars);


void Menu_WriteValueToStr(const MenuElement * element, char * str, uint32_t maxChars)
{
	switch(element->properties.valueType)
	{
		case VALUE_TYPE_WAVE_TYPE:
		{
			WriteWaveType(element->value.uintData, str, maxChars);
			break;
		}
		default:
			break;
	}
}

static void WriteWaveType(uint32_t val, char * str, uint32_t maxChars)
{
	const char * waveName = "";
	if(val == 0)
		waveName = "sine";
	else if(val == 1)
		waveName = "square";
	else if(val == 2)
		waveName = "triangle";
	else if(val == 3)
		waveName = "sawtooth";
	strncpy(str, waveName, maxChars);
}
