#include "screen/menu/menu_converter.h"
#include "wave/wave_config.h"
#include <string.h>


static void WriteWaveType(WaveType val, char * str, uint32_t maxChars);
static void ValueChangeWaveType(bool increase, MenuElement * element);

void Menu_WriteValueToStr(const MenuElement * element, char * str, uint32_t maxChars)
{
	switch(element->properties.valueType)
	{
		case VALUE_TYPE_WAVE_TYPE:
		{
			WriteWaveType(*(WaveType *)(element->value.data), str, maxChars);
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

static void WriteWaveType(WaveType val, char * str, uint32_t maxChars)
{
	const char * waveName = "";
	switch(val)
	{
		case WAVE_TYPE_SINE:
			waveName = "sine";
			break;
		case WAVE_TYPE_SQUARE:
			waveName = "square";
			break;
		case WAVE_TYPE_TRIANGLE:
			waveName = "triangle";
			break;
		case WAVE_TYPE_SAWTOOTH:
			waveName = "sawtooth";
			break;
		default:
			waveName = "undef";
			break;
	}
	strncpy(str, waveName, maxChars);
}


static void ValueChangeWaveType(bool increase, MenuElement * element)
{
	WaveType newVal = *(WaveType *)element->value.data;
	if(increase)
	{
		newVal++;
		newVal %= WAVE_TYPE_CNT;
	}
	else
	{
		if(newVal > 0)
			newVal--;
		else
			newVal = WAVE_TYPE_CNT - 1;
	}
	*(WaveType *)element->value.data = newVal;
}
