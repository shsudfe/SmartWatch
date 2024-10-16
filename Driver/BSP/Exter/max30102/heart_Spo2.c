#include "max30102/heart_Spo2.h"
#include "oled/oled.h"
#include "max30102/max30102.h"
#include "oled/OnceDis.h"

goOrQuitHS enterHSFlag = quitHS;				/* ������˳�����Ѫ��ģʽ��־λ */
uint32_t aun_ir_buffer[BUFFER_LENGTH] = {0};	/* ������������� */
uint32_t aun_red_buffer[BUFFER_LENGTH] = {0};	/* ����������� */
int32_t n_ir_buffer_length = BUFFER_LENGTH;		/* �������� */
uint8_t dis_hr = 0,dis_spo2 = 0;				/* ������ʾֵ��Ѫ����ʾֵ */

uint8_t firstGetHeartAndSpo2 = 0;		/* �״λ�ȡ����Ѫ����־λ 0Ϊ�״Σ�1���෴*/

void heart_Spo2_Init(void) {
	MAX30102_Init();
}

/**
  * @brief  ������˳�����Ѫ��ģʽ
  * @param  ��
  * @retval ��
  */
void enterOrQuitHeartSpo2Mode(void){
	/* ����ǰ������Ѫ��ģʽ���˳� */
	if(enterHSFlag == enterHS) {
		enterHSFlag = quitHS;		/* �ñ�־λΪ�˳� */
		firstGetHeartAndSpo2 = 0;	/* �����״ν����־ */
		clearHeartAndSpo2();		/* ����������ݺ�����Ѫ����ʾֵ */
		OnceDisFlag = dis;			/* �����״���ʾ��־ */
	/* ��֮����� */
	}else {
		enterHSFlag = enterHS;		/* �ñ�־λΪ���� */
	}
	
}

/**
  * @brief  ��ȡ���ʺ�Ѫ������ʾ���ʻ�Ѫ��
  * @param  select ѡ����ʾλ 
				heartSelect ��ʾ����
				spo2Select	��ʾѪ��
				bothSelect 	���߶���ʾ
  * @retval ��
  */
void showHeartOrSpo2(selectHeart_Spo2 select){
	
	/* ֻ��ʾ�����ʣ��� */
	if(select == heartSelect) {
		onceDis_Heart();
		OLED_ShowNum(2, 9, dis_hr, 3);				/* Ĭ����ʾ0 */
	}
	/* ֻ��ʾ��Ѫ������ */
	else if(select == spo2Select) {
		onceDis_Spo2();
		OLED_ShowNum(2, 9, dis_spo2, 2);			/* Ĭ����ʾ0 */
	}
	/* ����Ѫ������ʾ */
	else if(select == bothSelect) {
		onceDis_HeartSpo2();
		OLED_ShowNum(2, 9, dis_hr, 3);				/* Ĭ����ʾ0 */
		OLED_ShowNum(4, 9, dis_spo2, 2);			/* Ĭ����ʾ0 */
	}
	
	/* ����ǰ�ǽ�������Ѫ��ģʽ�����ȡ����Ѫ��ֵ */
	if(enterHSFlag == enterHS) {
		// ��ȡ����Ѫ��ֵ
		getHeartAndSpo2(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &dis_spo2, &dis_hr, &firstGetHeartAndSpo2);
	}
}

void clearHeartAndSpo2(void) {
	uint16_t i = 0;
	for(; i < BUFFER_LENGTH; i ++) {
		aun_ir_buffer[i] = 0;
		aun_red_buffer[i] = 0;
	}
	dis_hr = 0;
	dis_spo2 = 0;
}

void setOnceDis(void) {
	OnceDisFlag = dis;
}

