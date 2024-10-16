#include "rtc/MyAlarm.h"
#include "rtc/MyRTC.h"
#include "key/key.h"
#include "oled/oled.h"
#include "timer/btim.h"
#include "encoder/encoder.h"
#include "oled/onceDis.h"

/* 修改闹钟时间标志位 0小时 1分钟 2秒钟 */
uint8_t alterAlarmFlag = 0;
/* 闹钟时间数组 */
uint16_t alarmArr[7] = {2024, 8, 21, 13, 45, 20, 6}; 
extern RTC_HandleTypeDef RTC_HandleStructure;

/**
  * @brief  进入修改闹钟时间模式
  * @param  无
  * @retval 无
  */
void goAlterAlarmMode(void){
	BTIM_TIM2_ENABLE();				/* 使能TIM2 */
	alterAlarmFlag = 0;
	getTimeToArr(alarmArr);
}

/**
  * @brief  退出修改闹钟时间模式
  * @param  无
  * @retval 无
  */
void quitAlterAlarmMode(void){
	BTIM_TIM2_DISABLE();			/* 失能TIM2 */
	RTC_SetAlarm(&RTC_HandleStructure, alarmArr);
}

/**
  * @brief  切换修改闹钟时间位
  * @param  无
  * @retval 无
  */
void toggleAlterAlarmBit(void){
	alterAlarmFlag++;
	if(alterAlarmFlag == 3) alterAlarmFlag = 0;
}

/**
  * @brief  修改闹钟时间
  * @param  无
  * @retval 无
  */
void alterAlarm(void){
	uint8_t alterTimeAddOrSub = getAlterTimeAddOrSub();
	if(alterTimeAddOrSub == 1) {
		if(alterAlarmFlag == 0){
			if(alarmArr[3] < 23) alarmArr[3]++;
			else alarmArr[3] = 0;
		}else if(alterAlarmFlag == 1){
			if(alarmArr[4] < 59) alarmArr[4]++;
			else alarmArr[4] = 0;
		}else if(alterAlarmFlag == 2){
			if(alarmArr[5] < 59) alarmArr[5]++;
			else alarmArr[5] = 0;
		}
	}else if(alterTimeAddOrSub == 2) {
		if(alterAlarmFlag == 0){
			if(alarmArr[3] > 0) alarmArr[3]--;
			else alarmArr[3] = 23;
		}else if(alterAlarmFlag == 1){
			if(alarmArr[4] > 0) alarmArr[4]--;
			else alarmArr[4] = 59;
		}else if(alterAlarmFlag == 2){
			if(alarmArr[5] > 0) alarmArr[5]--;
			else alarmArr[5] = 59;
		}
	}
}

/**
  * @brief  显示修改闹钟时间
  * @param  无
  * @retval 无
  */
void showAlterAlarm(void) {
	uint8_t AlterTimeDisplayFlag = getAlterTimeDisplayFlag();
	
	/* 显示格式字符 */
//	showFormatSymble();
//	RTCShowTimeFlag = 1;
	
	/* 显示星期 */
	showWeek();
	/* 首次显示只执行一次显示格式字符 */
	onceDisAlterTime();

	
	OLED_ShowNum(2, 4, alarmArr[0], 4);
	OLED_ShowNum(2, 9, alarmArr[1], 2);
	OLED_ShowNum(2, 12, alarmArr[2], 2);
	/* 显示日期+时间 */
	if(alterAlarmFlag == 0) {
		if(AlterTimeDisplayFlag == 1) OLED_ShowNum(3, 5, alarmArr[3], 2);
		else OLED_ShowString(3, 5, "  ");
		OLED_ShowNum(3, 8, alarmArr[4], 2);
		OLED_ShowNum(3, 11, alarmArr[5], 2);
	}else if(alterAlarmFlag == 1) {
		if(AlterTimeDisplayFlag == 1) 	OLED_ShowNum(3, 8, alarmArr[4], 2);
		else OLED_ShowString(3, 8, "  ");
		OLED_ShowNum(3, 5, alarmArr[3], 2);
		OLED_ShowNum(3, 11, alarmArr[5], 2);
	}else if(alterAlarmFlag == 2) {
		if(AlterTimeDisplayFlag == 1) OLED_ShowNum(3, 11, alarmArr[5], 2);
		else OLED_ShowString(3, 11, "  ");
		OLED_ShowNum(3, 5, alarmArr[3], 2);
		OLED_ShowNum(3, 8, alarmArr[4], 2);
	}
	alterAlarm();

}

/**
  * @brief  设置RTC闹钟
  * @param  hrtc RTC句柄结构体地址
  * @param  timeArr	时间数组
  * @retval 无
  */
void RTC_SetAlarm(RTC_HandleTypeDef *hrtc, uint16_t *timeArr) {
	/* 先获取最新的当前时间到数组中 */
//	getTimeToArr();
	/* 调试语句 */
//	OLED_ShowNum(1, 4, timeArr[3], 2);
//	OLED_ShowNum(1, 7, timeArr[4], 2);
//	OLED_ShowNum(1, 10, timeArr[5], 2);
	/* RTC闹钟初始化 */
	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_AlarmStructure.Alarm = RTC_ALARM_A;
	RTC_AlarmStructure.AlarmTime.Hours = timeArr[3];
	RTC_AlarmStructure.AlarmTime.Minutes = timeArr[4];
	RTC_AlarmStructure.AlarmTime.Seconds = timeArr[5];
	HAL_RTC_WaitForSynchro(hrtc);
	HAL_RTC_SetAlarm_IT(hrtc, &RTC_AlarmStructure, RTC_FORMAT_BIN);

	/* 设置闹钟后，写备份寄存器DR7，写入闹钟时间总和左移一位（最多占8位）
	加0x01表示已经设置了闹钟，为了在设备被闹钟唤醒后依然能够发声 */
	rtcWriteBKPDataRegister(RTC_BKP_DR7, ((timeArr[3] + timeArr[4] + timeArr[5]) << 1) + 0x01);
//	rtcWriteBKPDataRegister(RTC_BKP_DR7, 0x01);
	
}




