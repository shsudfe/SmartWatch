#ifndef __MYRTC_H
#define __MYRTC_H

#include "sys/sys.h"

//extern uint8_t RTCShowTimeFlag;

/* new function */
/* RTC��ʼ�� */
void MyRTC_Init(void);

/* ���ж�ʹ/ʧ�� */
void SecondItEnable(void);
void SecondItDisable(void);

/* ��ʾ��ǰ����ʱ�� */
void showTimeAndDate(void);
void showWeek(void);

/* ��������ʱ�䡢��ȡ����ʱ�� */  
void setTimeToTypeDef(void);
void setDateToTypeDef(void);
void getTimeToArr(uint16_t *Arr);

/* ��ǰʱ������ģʽ */
void goAlterTimeMode(void);
void quitAlterTimeMode(void);
void toggleAlterTimeBit(void);
void alterTime(void);
void showAlterTime(void);


/* ��д�������ݼĴ��� */
uint32_t rtcReadBKPDataRegister(uint32_t BackupRegister);
void rtcWriteBKPDataRegister(uint32_t BackupRegister, uint32_t Data);
void beforeResetSaveDate(uint8_t select);
void getSaveDateFromBKP(void);

/* ����ʱ������/�˳�����ģʽ */
void goSleepMode(void);
void quitSleepMode(void);
void timingGoSleep(void);
void timingQuitSleep(void);

#endif


