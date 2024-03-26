#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdint.h>


#define LOG_DEBUG_STATS true


bool Setup_Logging_CreateTask();
void ISR_LoggingUARTTx();

void Logging_LogBlocking(const char * message);
void Logging_Log(const char * message);
void Logging_LogWithNum(const char * message, int32_t num);
void Logging_Info(const char * message);
void Logging_InfoWithNum(const char * message, int32_t num);
void Logging_Warn(const char * message);
void Logging_Error(const char * message);


#endif
