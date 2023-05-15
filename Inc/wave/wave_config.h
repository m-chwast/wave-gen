#pragma once
#include <stdint.h>

typedef enum
{
	WAVE_TYPE_SINE,
	WAVE_TYPE_SQUARE,
	WAVE_TYPE_TRIANGLE,
	WAVE_TYPE_SAWTOOTH,

	WAVE_TYPE_CNT
} WaveType;

typedef struct
{
	WaveType type;
	uint32_t freq;
} WaveConfig;

extern WaveConfig * waveConfig;
