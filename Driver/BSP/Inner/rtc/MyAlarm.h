#ifndef __MYALARM_H
#define __MYALARM_H

#include "sys/sys.h"

/* RTC闹钟设置 */
void RTC_SetAlarm(RTC_HandleTypeDef *hrtc, uint16_t *timeArr);

/* 进入修改闹钟时间模式 */
void goAlterAlarmMode(void);
void quitAlterAlarmMode(void);
void toggleAlterAlarmBit(void);
void alterAlarm(void);
void showAlterAlarm(void);


#endif


