#ifndef __TIMINGSLEEP_H
#define __TIMINGSLEEP_H

#include "sys/sys.h"

/* ��ʱ���ߵȼ� */
#define TIMING_LEVEL			3			

extern uint8_t timingArr[2][TIMING_LEVEL];
extern uint8_t timingIndex;

void showTimingSleep(void);
void setTiming(void);

#endif


