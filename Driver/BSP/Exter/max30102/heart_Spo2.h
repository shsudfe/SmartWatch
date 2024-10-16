#ifndef __HEART_SPON2_H
#define __HEART_SPON2_H

#include "sys/sys.h"

#define BUFFER_LENGTH			500		/* �������ݴ�С������Ϊ500���������״β�����ȴ�5s����ÿ1s���� */

/* ����Ѫ��ѡ����ʾ��־ */
typedef enum heart_spo2 {
	heartSelect,
	spo2Select,
	bothSelect,
	
}selectHeart_Spo2;

/* ������˳�����Ѫ��ģʽ��־ */
typedef enum goOrQuit {
	enterHS,
	quitHS,
	
}goOrQuitHS;

extern goOrQuitHS enterHSFlag;

extern uint32_t aun_ir_buffer[BUFFER_LENGTH]; 			/* IR LED   ��������ݣ����ڼ���Ѫ�� */
extern int32_t n_ir_buffer_length;    					/* ���ݳ��� */
extern uint32_t aun_red_buffer[BUFFER_LENGTH]; 			/* Red LED	������ݣ����ڼ������������Լ��������� */
extern uint8_t dis_hr, dis_spo2;						/* ʵ��Ҫ��ʾ�����ʺ�Ѫ��ֵ */

void heart_Spo2_Init(void);
void enterOrQuitHeartSpo2Mode(void);
void showHeartOrSpo2(selectHeart_Spo2 select);
void clearHeartAndSpo2(void);
void setOnceDis(void);


#endif


