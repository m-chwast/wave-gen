#include "screen/menu/menu_converter.h"
#include "wave/wave_config.h"
#include <string.h>


static void StrInvert(char * str);
static void WriteWaveType(WaveType val, char * str, uint32_t maxChars);
static void ValueChangeWaveType(bool increase, MenuElement * element);
static void WriteFrequency(uint32_t val, char * str, uint32_t maxChars);
static void ValueChangeFrequency(bool increase, MenuElement * element);


void Menu_WriteValueToStr(const MenuElement * element, char * str, uint32_t maxChars)
{
	switch(element->properties.valueType)
	{
		case VALUE_TYPE_WAVE_TYPE:
		{
			WriteWaveType(*(WaveType *)(element->value.data), str, maxChars);
			break;
		}
		case VALUE_TYPE_FREQUENCY:
		{
			WriteFrequency(*(uint32_t *)element->value.data, str, maxChars);
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
		case VALUE_TYPE_FREQUENCY:
		{
			ValueChangeFrequency(increase, element);
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

static void WriteFrequency(uint32_t val, char * str, uint32_t maxChars)
{
	uint8_t i = 0;
	do
	{
		str[i] = '0' + (val % 10);
		val /= 10;
		i++;
	} while(val > 0 && i < maxChars);
	StrInvert(str);

	str[i] = '\0';
	if(maxChars - i >= 3)
		strcat(str, " Hz");
}

static void ValueChangeFrequency(bool increase, MenuElement * element)
{
	uint32_t * val = (uint32_t *)element->value.data;

	if(increase)
		(*val)++;
	else
		(*val)--;
}

static void StrInvert(char * str)
{
	uint8_t len = strlen(str);
	for(uint8_t i = 0; i < len / 2; i++)
	{
		char tmp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = tmp;
	}
}
