#include "rtc/SecondWatch.h"
#include "rtc/MyRTC.h"
#include "oled/oled.h"
#include "timer/btim.h"
#include "oled/onceDis.h"

/**
  * @brief  开启秒表，开始计时
  * @param  无
  * @retval 无
  */
void goSecondWatchMode(void){
	BTIM_TIM3_ENABLE();
}

/**
  * @brief  停止秒表，暂停计时
  * @param  无
  * @retval 无
  */
void suspendSecondWatchMode(void){
	BTIM_TIM3_DISABLE();
}

/**
  * @brief  重置秒表计数值
  * @param  无
  * @retval 无
  */
void resetSecondWatchMode(void) {
	msCount = 0;
}

/**
  * @brief  显示秒表计数值
  * @param  无
  * @retval 无
  */
void showSecondWatch(void){
	
//	if(RTCShowTimeFlag == 0) {
//		OLED_ShowString(2, 9, "  :  :  ");
//		RTCShowTimeFlag = 1;
//	}
	
	/* 首次执行只显示一次格式字符 */
	onceDisSecondWatch();
	
	OLED_ShowNum(2, 9, msCount / 100 / 60, 2);		/* 分钟 */
	OLED_ShowNum(2, 12, msCount / 100, 2);			/* 秒钟 */
	OLED_ShowNum(2, 15, msCount, 2);				/* 毫秒 */
}




