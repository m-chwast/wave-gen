#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>


#define LOG_DEBUG_STATS true


bool Setup_Logging_CreateTask();
void ISR_LoggingUARTTx();

void Logging_LogBlocking(const char * message);
void Logging_Log(const char * message);
void Logging_Info(const char * message);
void Logging_Warn(const char * message);
void Logging_Error(const char * message);


#endif
