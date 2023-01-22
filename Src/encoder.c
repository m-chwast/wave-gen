#include "encoder.h"
#include "logging.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "tim.h"


#define TASK_STACK_SIZE 256
#define TASK_NAME "Encoder Task"
#define TASK_PRIORITY 1
#define TASK_DELAY_TIME pdMS_TO_TICKS(150)

#define ENCODER_TIMER htim3	//should be 16-bit timer
#define ENCODER_TIMER_HALF_RANGE (0xFFFF / 2)

static Encoder_Movement encoderMovement;


static void EncoderTask();
static void UpdateEncoderState();
static void LogEncoderParams();


bool Setup_Encoder_CreateTask()
{
	BaseType_t res =  xTaskCreate(EncoderTask, TASK_NAME, TASK_STACK_SIZE,
			NULL, TASK_PRIORITY, NULL);
	if(res != pdPASS)
	{
		Logging_Error(TASK_NAME " could not be created!\r\n");
		return false;
	}
	Logging_Info(TASK_NAME " created\r\n");
	return true;
}

Encoder_Movement Encoder_GetMovement()
{
	return encoderMovement;
}

bool Encoder_IsButtonPressed()
{
	GPIO_PinState encButtonState;
	encButtonState = HAL_GPIO_ReadPin(ENC_SW_GPIO_Port, ENC_SW_Pin);
	if(encButtonState != GPIO_PIN_RESET)
		return false;
	return true;
}


static void EncoderTask()
{
	encoderMovement.dir = ENCODER_DIR_STOPPED;
	encoderMovement.speed = ENCODER_SPEED_STOPPED;
	encoderMovement.exactStepsDelta = 0;

	while(true)
	{
		UpdateEncoderState();
		LogEncoderParams();
		vTaskDelay(TASK_DELAY_TIME);
	}
}

static void UpdateEncoderState()
{
	static uint16_t oldCnt;
	uint16_t steps = 0, newCnt = ENCODER_TIMER.Instance->CNT;
	Encoder_Movement currentMovement;

	if(newCnt > oldCnt)
	{
		steps = newCnt - oldCnt;
		if(steps < ENCODER_TIMER_HALF_RANGE)	//no overflow
		{
			currentMovement.dir = ENCODER_DIR_RIGHT;
		}
		else	//overflow (difference too big for normal operation)
		{
			currentMovement.dir = ENCODER_DIR_LEFT;
			steps = oldCnt - newCnt;
		}
	}
	else if(newCnt < oldCnt)
	{
		steps = oldCnt - newCnt;
		if(steps < ENCODER_TIMER_HALF_RANGE)	//no overflow
		{
			currentMovement.dir = ENCODER_DIR_LEFT;
		}
		else	//overflow
		{
			currentMovement.dir = ENCODER_DIR_RIGHT;
			steps = newCnt - oldCnt;
		}
	}

	steps /= 4;	//convert from ticks to steps
	if(steps == 0)
	{
		currentMovement.dir = ENCODER_DIR_STOPPED;
		currentMovement.speed = ENCODER_SPEED_STOPPED;
	}
	else
	{
		oldCnt = newCnt;	//update only if encoder is at stable position
	}

	currentMovement.exactStepsDelta = steps;

	if(steps == 1)
		currentMovement.speed = ENCODER_SPEED_SLOW;
	else if(steps <= 3)
		currentMovement.speed = ENCODER_SPEED_MEDIUM;
	else if(steps > 3)
		currentMovement.speed = ENCODER_SPEED_HIGH;

	taskENTER_CRITICAL();
	encoderMovement = currentMovement;
	taskEXIT_CRITICAL();
}

static void LogEncoderParams()
{
	if(encoderMovement.dir == ENCODER_DIR_STOPPED)
		return;

	const char * dirMsg;
	if(encoderMovement.dir == ENCODER_DIR_LEFT)
		dirMsg = "left";
	else if(encoderMovement.dir == ENCODER_DIR_RIGHT)
		dirMsg = "right";

	Logging_Info("Encoder movement: ");
	Logging_Log(dirMsg);
	Logging_LogWithNum(", steps: ", encoderMovement.exactStepsDelta);
}
