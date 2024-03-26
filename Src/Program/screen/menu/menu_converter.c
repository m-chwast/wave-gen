#include "screen/menu/menu_converter.h"
#include "wave/wave_config.h"
#include <string.h>


static void StrInvert(char * str);
static void WriteWaveType(WaveType val, char * str, uint32_t maxChars);
static void ValueChangeWaveType(bool increase, Encoder_Speed speed, MenuElement * element);
static void WriteFrequency(uint32_t val, char * str, uint32_t maxChars);
static void ValueChangeFrequency(bool increase, Encoder_Speed speed, MenuElement * element);


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

void Menu_ChangeValue(bool increase, Encoder_Speed speed, MenuElement * element)
{
	switch(element->properties.valueType)
	{
		case VALUE_TYPE_WAVE_TYPE:
		{
			ValueChangeWaveType(increase, speed, element);
			break;
		}
		case VALUE_TYPE_FREQUENCY:
		{
			ValueChangeFrequency(increase, speed, element);
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


static void ValueChangeWaveType(bool increase, Encoder_Speed speed, MenuElement * element)
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
	uint32_t tmp = val;
	uint8_t i = 0;
	do
	{
		str[i] = '0' + (tmp % 10);
		tmp /= 10;
		i++;
	} while(tmp > 0 && i < maxChars);
	StrInvert(str);
	str[i] = '\0';

	if(val < 10000)
	{
		if(maxChars - i >= 3)
			strcat(str, " ");
		if(maxChars - i >= 2)
			strcat(str, "Hz");
	}
	else
	{
		int8_t len = strlen(str);
		int8_t dot = len - 3;
		if(dot < 0 || len < 4)
			return;

		uint8_t pos = dot;
		if(dot < 3)
		{
			char tmp = str[dot];
			str[dot] = '.';
			str[dot + 1] = tmp;
			pos = dot + 2;
		}
		str[pos] = '\0';

		if(pos + 4 < len)
			return;
		strcat(str, " kHz");
	}
}

static void ValueChangeFrequency(bool increase, Encoder_Speed speed, MenuElement * element)
{
	int32_t newVal = *(uint32_t *)element->value.data;

	int32_t step = increase ? 1 : -1;

	if(newVal > 100000)
	{
		if(newVal % 1000)
			newVal = (newVal / 1000) * 1000;
		step *= 1000;
	}
	else if(newVal > 10000)
	{
		if(newVal % 100)
			newVal = (newVal / 100) * 100;
		step *= 100;
	}
	else if(newVal > 1000)
	{
		if(newVal % 10)
			newVal = (newVal / 10) * 10;
		step *= 10;
	}

	switch(speed)
	{
		case ENCODER_SPEED_MEDIUM:
			step *= 10;
			break;
		case ENCODER_SPEED_HIGH:
			step *= 100;
			break;
		default:
			break;
	}


	newVal += step;

	if(newVal > 1000000)
		newVal = 1000000;
	if(newVal < 0)
		newVal = 0;

	*(uint32_t *)element->value.data = newVal;
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
