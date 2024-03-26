#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdbool.h>
#include <stdint.h>


typedef enum
{
	ENCODER_DIR_STOPPED,
	ENCODER_DIR_LEFT,
	ENCODER_DIR_RIGHT,
} Encoder_Direction;

typedef enum
{
	ENCODER_SPEED_STOPPED,
	ENCODER_SPEED_SLOW,
	ENCODER_SPEED_MEDIUM,
	ENCODER_SPEED_HIGH,
} Encoder_Speed;

typedef struct
{
	Encoder_Direction dir;
	Encoder_Speed speed;
	uint16_t exactStepsDelta;	//always positive, dir determines direction
} Encoder_Movement;


bool Setup_Encoder_CreateTask();
Encoder_Movement Encoder_GetMovement();
bool Encoder_IsButtonPressed();


#endif
