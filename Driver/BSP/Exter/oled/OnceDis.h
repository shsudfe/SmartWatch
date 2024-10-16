#ifndef __ONCEDIS_H
#define __ONCEDIS_H

#include "sys/sys.h"

/* �״���ʾ��־���� */
typedef enum Once {
	dis,		/* ��ʾ */
	unDis,		/* ����ʾ */
}onceDisplay;

extern onceDisplay OnceDisFlag;

/* ��ʾʱ���ʽ�ַ� */
void disTimeFormatSymble(void);

/* �״�ִ�л���ʾʱ��/���ӣ�ִֻ��һ�εĴ��� */
void onceDisTime(void);
void onceDisAlterTime(void);
void onceDisSecondWatch(void);

/* �״���ʾ����/Ѫ����ִֻ��һ�εĴ��� */
void onceDis_Heart(void);
void onceDis_Spo2(void);
void onceDis_HeartSpo2(void);

/* �״���ʾ�޸�����ʱ�䣬ִֻ��һ�εĴ��� */
void onceDisTimingSleep(void);

#endif


