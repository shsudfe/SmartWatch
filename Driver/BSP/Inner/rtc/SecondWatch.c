#include "rtc/SecondWatch.h"
#include "rtc/MyRTC.h"
#include "oled/oled.h"
#include "timer/btim.h"
#include "oled/onceDis.h"

/**
  * @brief  ���������ʼ��ʱ
  * @param  ��
  * @retval ��
  */
void goSecondWatchMode(void){
	BTIM_TIM3_ENABLE();
}

/**
  * @brief  ֹͣ�����ͣ��ʱ
  * @param  ��
  * @retval ��
  */
void suspendSecondWatchMode(void){
	BTIM_TIM3_DISABLE();
}

/**
  * @brief  ����������ֵ
  * @param  ��
  * @retval ��
  */
void resetSecondWatchMode(void) {
	msCount = 0;
}

/**
  * @brief  ��ʾ������ֵ
  * @param  ��
  * @retval ��
  */
void showSecondWatch(void){
	
//	if(RTCShowTimeFlag == 0) {
//		OLED_ShowString(2, 9, "  :  :  ");
//		RTCShowTimeFlag = 1;
//	}
	
	/* �״�ִ��ֻ��ʾһ�θ�ʽ�ַ� */
	onceDisSecondWatch();
	
	OLED_ShowNum(2, 9, msCount / 100 / 60, 2);		/* ���� */
	OLED_ShowNum(2, 12, msCount / 100, 2);			/* ���� */
	OLED_ShowNum(2, 15, msCount, 2);				/* ���� */
}




