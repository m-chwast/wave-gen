#ifndef _SERIAL_RECEIVER_H_
#define _SERIAL_RECEIVER_H_


#include <stdbool.h>


bool Setup_SerialReceiver_CreateTask();
void ISR_SerialReceiver_RxCplt();

#endif
