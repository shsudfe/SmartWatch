#ifndef __ONCEDIS_H
#define __ONCEDIS_H

#include "sys/sys.h"

/* 首次显示标志类型 */
typedef enum Once {
	dis,		/* 显示 */
	unDis,		/* 不显示 */
}onceDisplay;

extern onceDisplay OnceDisFlag;

/* 显示时间格式字符 */
void disTimeFormatSymble(void);

/* 首次执行或显示时间/闹钟，只执行一次的代码 */
void onceDisTime(void);
void onceDisAlterTime(void);
void onceDisSecondWatch(void);

/* 首次显示心率/血氧，只执行一次的代码 */
void onceDis_Heart(void);
void onceDis_Spo2(void);
void onceDis_HeartSpo2(void);

/* 首次显示修改休眠时间，只执行一次的代码 */
void onceDisTimingSleep(void);

#endif


