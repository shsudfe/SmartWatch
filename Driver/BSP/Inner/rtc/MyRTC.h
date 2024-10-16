#ifndef __MYRTC_H
#define __MYRTC_H

#include "sys/sys.h"

//extern uint8_t RTCShowTimeFlag;

/* new function */
/* RTC初始化 */
void MyRTC_Init(void);

/* 秒中断使/失能 */
void SecondItEnable(void);
void SecondItDisable(void);

/* 显示当前日期时间 */
void showTimeAndDate(void);
void showWeek(void);

/* 设置日期时间、获取日期时间 */  
void setTimeToTypeDef(void);
void setDateToTypeDef(void);
void getTimeToArr(uint16_t *Arr);

/* 当前时间配置模式 */
void goAlterTimeMode(void);
void quitAlterTimeMode(void);
void toggleAlterTimeBit(void);
void alterTime(void);
void showAlterTime(void);


/* 读写操作备份寄存器 */
uint32_t rtcReadBKPDataRegister(uint32_t BackupRegister);
void rtcWriteBKPDataRegister(uint32_t BackupRegister, uint32_t Data);
void beforeResetSaveDate(uint8_t select);
void getSaveDateFromBKP(void);

/* （定时）进入/退出休眠模式 */
void goSleepMode(void);
void quitSleepMode(void);
void timingGoSleep(void);
void timingQuitSleep(void);

#endif


