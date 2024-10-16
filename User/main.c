#include "led/led.h"
#include "sys/sys.h"
#include "usart/usart.h"
#include "delay/delay.h"
#include "oled/oled.h"
#include "encoder/encoder.h"
#include "rtc/myrtc.h"
#include "beep/beep.h"
#include "timer/btim.h"
#include "key/key.h"
#include "rtc/MyAlarm.h"
#include "menu/menu.h"
#include "rtc/SecondWatch.h"
#include "sleep/timingSleep.h"
#include "exti/gpioexti.h"
#include "max30102/heart_Spo2.h"
#include "oled/onceDis.h"

uint8_t keyValue;

void keyScanDoFun(void);

int main(void) {
	
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* ����ʱ��,72M  */
    delay_init(72);                             /* ��ʼ����ʱ���� */
	LED_Init();									/* LED��ʼ�� */
	OLED_Init();								/* OLED��ʾ����ʼ�� */
	EncoderSensor_Init();						/* ��ת��������ʼ�� */
	MyRTC_Init();								/* RTC��ʼ�� */
	Beep_Init();								/* ��������ʼ�� */
	Key_Init();									/* ������ʼ�� */
	Key_EXTI_Init();							/* �����ⲿ�жϳ�ʼ�� */
	usart_init(115200);							/* ���ڳ�ʼ�� */
	heart_Spo2_Init();							/* max30102����Ѫ��ģ���ʼ�� */
	
	btim_tim4_int_init(10000 - 1, 7200 - 1);	/* ��ʼ����ʱ��4,�Ƴ�ʱ��Ϊ1s,������ʱ���� */
	btim_tim3_int_init(100 - 1, 7200 - 1);		/* ��ʼ����ʱ��3,���ʱ��Ϊ10ms,��������ʱ */
	btim_tim2_int_init(5000 - 1, 7200 - 1);		/* ��ʼ����ʱ��2,���ʱ��Ϊ500ms,����ʱ����˸��ʾ */
	
	while(1) {
		keyScanDoFun();									/* ����ɨ��ִ��ָ�����ܺ��� */
		
		/* �����棬��ʾ��׼ʱ�� */
		if(menuFlag == menuBlank) {						
       		showTimeAndDate();
		/* ֻ����ʾ�˵���ܽ��棺�޸����Ӻ��޸�ʱ�� */
		}else if(menuFlag == menuFun) {					/* ��ʾ���ܽ��� */
			if(funShowFlag == alterTimeShow){			
				showAlterTime();						/* ��ʾ�޸�ʱ����� */
			}else if(funShowFlag == alterAlarmShow){	
				showAlterAlarm();						/* ��ʾ�޸����ӽ��� */
			}
			
		/* ֻ��ʾ�˵���ͬʱ����ʾ�˵������ܽ��� */
		}else if(menuFlag == menuShow){					
			if(funShowFlag == secondWatchShow) {
				showSecondWatch();						/* ��ʾ����ܽ��� */
			}else if(funShowFlag == timingSleepShow) {
				showTimingSleep();						/* ��ʾ��������ʱ�书�ܽ��� */
			}else if(funShowFlag == heartShow) {
				showHeartOrSpo2(heartSelect);			/* ��ʾ���ʹ��ܽ��� */
			}else if(funShowFlag == spo2Show) {
				showHeartOrSpo2(spo2Select);			/* ��ʾѪ�����ܽ��� */
			}
			else{										
//				if(RTCShowTimeFlag == 1) 				/* ÿ��ֻ��ʾ�˵�ʱ��������RTCShowTimeFlag */
//					RTCShowTimeFlag = 0;
				if(OnceDisFlag == unDis)				/* ÿ��ֻ��ʾ�˵�ʱ��������OnceDisFlag */
					OnceDisFlag = dis;
			}
			
			/* ͬ���˵���ʾ,��ȡ�˵��������� */
			encoderDirection dir = getDir();			
			if(dir != noDir) showMenu(selfMenu, dir);	/* �����˵�����,�������ʾͬ���˵� */
		}
	}
}

/**
  * @brief  ����ɨ��ִ��ָ�����ܺ���
  * @param  ��
  * @retval ��
  */
void keyScanDoFun(void) {
	keyValue = getKeyValue();
	
	/* ����1���£�ֻ���״ΰ��²ų�ʼ���˵���ִ�м�ͷָ��˵���Ĺ��ܺ���*/
//	if(keyValue == KEY1_VAL) {	
	/* ��ȡ����1���ⲿ�жϱ�־�����жϲŽ����֧ */
	if(getKeyITFlag()) {								
		if(menuFlag == menuBlank && arrowPoint == NULL) 
			Menu_Init();								
		arrowPoint->currentMItem->funP();				/* ִ�м�ͷָ��ָ��˵���Ĺ��ܺ��� */
		
	/* ����2���£��Ҵ����޸ı�׼ʱ������ӹ���ʱ�����л��޸�λ */
	}else if(keyValue == KEY2_VAL) {
		if(funShowFlag == alterTimeShow) toggleAlterTimeBit();
		else if(funShowFlag == alterAlarmShow) toggleAlterAlarmBit();
		else if(funShowFlag == timingSleepShow) {
			timingIndex++;
			if(timingIndex > 5) timingIndex = 0;
		}
	
	/* ����3���£��ֶ���������ģʽ */
	}else if(keyValue == KEY3_VAL) {
		goSleepMode();
	}
	
}



