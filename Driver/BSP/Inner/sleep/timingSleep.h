#ifndef __TIMINGSLEEP_H
#define __TIMINGSLEEP_H

#include "sys/sys.h"

/* 定时休眠等级 */
#define TIMING_LEVEL			3			

extern uint8_t timingArr[2][TIMING_LEVEL];
extern uint8_t timingIndex;

void showTimingSleep(void);
void setTiming(void);

#endif


