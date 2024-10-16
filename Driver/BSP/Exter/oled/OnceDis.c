#include "oled/oncedis.h"
#include "oled/oled.h"
#include "rtc/myrtc.h"

/*
	只针对“ 首次执行且只需执行一次的文本(不需要更新执行) ”
	
*/


onceDisplay OnceDisFlag = dis;		/* 首次显示标志位 */

/**
  * @brief  显示标准时间格式字符
  * @param  无
  * @retval 无
  */
void disTimeFormatSymble(void) {
	OLED_ShowString(2, 4, "    -  -  ");
	OLED_ShowString(3, 5, "  :  :  ");
}

/**
  * @brief  首次显示标准时间
  * @param  无
  * @retval 无
  */
void onceDisTime(void) {
	if(OnceDisFlag == dis){
		disTimeFormatSymble();		/* 显示时间格式字符 */
		timingGoSleep();			/* 进入定时休眠模式 */
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  首次显示修改标准时间
  * @param  无
  * @retval 无
  */
void onceDisAlterTime(void) {
	if(OnceDisFlag == dis){
		disTimeFormatSymble();		/* 显示时间格式字符 */
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  首次显示修改标准时间
  * @param  无
  * @retval 无
  */
void onceDisSecondWatch(void) {
	if(OnceDisFlag == dis){
		OLED_ShowString(2, 9, "  :  :  ");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  首次显示设置休眠时间
  * @param  无
  * @retval 无
  */
void onceDisTimingSleep(void) {
	if(OnceDisFlag == dis){
		OLED_ShowChinese(2, 5, "后休眠");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  首次显示心率
  * @param  无
  * @retval 无
  */
void onceDis_Heart(void){
	if(OnceDisFlag == dis) {
		OLED_ShowChinese(1, 5, "心率");
		OLED_ShowString(2, 9, "   BMP");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  首次显示血氧
  * @param  无
  * @retval 无
  */
void onceDis_Spo2(void){
	if(OnceDisFlag == dis) {
		OLED_ShowChinese(1, 5, "血氧");
		OLED_ShowString(2, 9, "  %");
		OnceDisFlag = unDis;
	}
}

/**
  * @brief  首次显示心率和血氧
  * @param  无
  * @retval 无
  */
void onceDis_HeartSpo2(void){
	if(OnceDisFlag == dis) {
		OLED_ShowChinese(1, 5, "心率");
		OLED_ShowString(2, 9, "   BMP");
		OLED_ShowChinese(3, 5, "血氧");
		OLED_ShowString(4, 9, "  %");
		OnceDisFlag = unDis;
	}
}
