#include "screen/menu/menu_converter.h"
#include <string.h>


static void WriteWaveType(uint32_t val, char * str, uint32_t maxChars);
static void ValueChangeWaveType(bool increase, MenuElement * element);

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

void Menu_ChangeValue(bool increase, MenuElement * element)
{
	switch(element->properties.valueType)
	{
		case VALUE_TYPE_WAVE_TYPE:
		{
			ValueChangeWaveType(increase, element);
			break;
		}
		default:
		{
			break;
		}
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


static void ValueChangeWaveType(bool increase, MenuElement * element)
{
	uint32_t newVal = element->value.uintData;
	if(increase)
	{
		newVal++;
		newVal %= 4;	//3 is max value
	}
	else
	{
		if(newVal > 0)
			newVal--;
		else
			newVal = 3;
	}
	element->value.uintData = newVal;

}
