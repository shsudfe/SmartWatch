#include "sleep/timingSleep.h"
#include "oled/oled.h"
#include "timer/btim.h"
#include "oled/onceDis.h"

/* 	��ʱ���߶�ʱʱ��
	timingArr[0] ����Ϊ��λ��ʱ 15s, 30s, 45s
	timingArr[1] �Է���Ϊ��λ��ʱ 5m, 15m, 30m

*/
uint8_t timingArr[2][TIMING_LEVEL] = {
		15, 30, 45,
		5, 15, 30,
};
uint8_t timingIndex = 0;


/**
  * @brief  ��ʾ��ʱ���߽���
  * @param  ��
  * @retval ��
  */
void showTimingSleep(void) {
	OLED_ShowNum(1, 11, timingArr[timingIndex / TIMING_LEVEL][timingIndex % TIMING_LEVEL], 2);
	if(timingIndex / TIMING_LEVEL == 0){
		OLED_ShowChar(1, 13, 's');
	}else OLED_ShowChar(1, 13, 'm');
//	OLED_ShowChinese(2, 5, "������");
	onceDisTimingSleep();
}


void setTiming(void) {
	uint8_t num = timingArr[timingIndex / TIMING_LEVEL][timingIndex % TIMING_LEVEL];
	if(timingIndex / TIMING_LEVEL == 0){
		setTimingTaret(num);
	}else setTimingTaret(num * 60);
}









