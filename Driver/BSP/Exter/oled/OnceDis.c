#include "oled/oncedis.h"
#include "oled/oled.h"
#include "rtc/myrtc.h"

/*
	ֻ��ԡ� �״�ִ����ֻ��ִ��һ�ε��ı�(����Ҫ����ִ��) ��
	
*/


onceDisplay OnceDisFlag = dis;		/* �״���ʾ��־λ */

/**
  * @brief  ��ʾ��׼ʱ���ʽ�ַ�
  * @param  ��
  * @retval ��
  */
void disTimeFormatSymble(void) {
	OLED_ShowString(2, 4, "    -  -  ");
	OLED_ShowString(3, 5, "  :  :  ");
}

/**
  * @brief  �״���ʾ��׼ʱ��
  * @param  ��
  * @retval ��
  */
void onceDisTime(void) {
	if(OnceDisFlag == dis){
		disTimeFormatSymble();		/* ��ʾʱ���ʽ�ַ� */
		timingGoSleep();			/* ���붨ʱ����ģʽ */
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  �״���ʾ�޸ı�׼ʱ��
  * @param  ��
  * @retval ��
  */
void onceDisAlterTime(void) {
	if(OnceDisFlag == dis){
		disTimeFormatSymble();		/* ��ʾʱ���ʽ�ַ� */
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  �״���ʾ�޸ı�׼ʱ��
  * @param  ��
  * @retval ��
  */
void onceDisSecondWatch(void) {
	if(OnceDisFlag == dis){
		OLED_ShowString(2, 9, "  :  :  ");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  �״���ʾ��������ʱ��
  * @param  ��
  * @retval ��
  */
void onceDisTimingSleep(void) {
	if(OnceDisFlag == dis){
		OLED_ShowChinese(2, 5, "������");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  �״���ʾ����
  * @param  ��
  * @retval ��
  */
void onceDis_Heart(void){
	if(OnceDisFlag == dis) {
		OLED_ShowChinese(1, 5, "����");
		OLED_ShowString(2, 9, "   BMP");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  �״���ʾѪ��
  * @param  ��
  * @retval ��
  */
void onceDis_Spo2(void){
	if(OnceDisFlag == dis) {
		OLED_ShowChinese(1, 5, "Ѫ��");
		OLED_ShowString(2, 9, "  %");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  �״���ʾ���ʺ�Ѫ��
  * @param  ��
  * @retval ��
  */
void onceDis_HeartSpo2(void){
	if(OnceDisFlag == dis) {
		OLED_ShowChinese(1, 5, "����");
		OLED_ShowString(2, 9, "   BMP");
		OLED_ShowChinese(3, 5, "Ѫ��");
		OLED_ShowString(4, 9, "  %");
		OnceDisFlag = unDis;
	}
}
