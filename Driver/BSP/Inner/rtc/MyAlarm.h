#ifndef __MYALARM_H
#define __MYALARM_H

#include "sys/sys.h"

/* RTC�������� */
void RTC_SetAlarm(RTC_HandleTypeDef *hrtc, uint16_t *timeArr);

/* �����޸�����ʱ��ģʽ */
void goAlterAlarmMode(void);
void quitAlterAlarmMode(void);
void toggleAlterAlarmBit(void);
void alterAlarm(void);
void showAlterAlarm(void);


#endif


