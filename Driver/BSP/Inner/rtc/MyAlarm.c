#include "rtc/MyAlarm.h"
#include "rtc/MyRTC.h"
#include "key/key.h"
#include "oled/oled.h"
#include "timer/btim.h"
#include "encoder/encoder.h"
#include "oled/onceDis.h"

/* �޸�����ʱ���־λ 0Сʱ 1���� 2���� */
uint8_t alterAlarmFlag = 0;
/* ����ʱ������ */
uint16_t alarmArr[7] = {2024, 8, 21, 13, 45, 20, 6}; 
extern RTC_HandleTypeDef RTC_HandleStructure;

/**
  * @brief  �����޸�����ʱ��ģʽ
  * @param  ��
  * @retval ��
  */
void goAlterAlarmMode(void){
	BTIM_TIM2_ENABLE();				/* ʹ��TIM2 */
	alterAlarmFlag = 0;
	getTimeToArr(alarmArr);
}

/**
  * @brief  �˳��޸�����ʱ��ģʽ
  * @param  ��
  * @retval ��
  */
void quitAlterAlarmMode(void){
	BTIM_TIM2_DISABLE();			/* ʧ��TIM2 */
	RTC_SetAlarm(&RTC_HandleStructure, alarmArr);
}

/**
  * @brief  �л��޸�����ʱ��λ
  * @param  ��
  * @retval ��
  */
void toggleAlterAlarmBit(void){
	alterAlarmFlag++;
	if(alterAlarmFlag == 3) alterAlarmFlag = 0;
}

/**
  * @brief  �޸�����ʱ��
  * @param  ��
  * @retval ��
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
  * @brief  ��ʾ�޸�����ʱ��
  * @param  ��
  * @retval ��
  */
void showAlterAlarm(void) {
	uint8_t AlterTimeDisplayFlag = getAlterTimeDisplayFlag();
	
	/* ��ʾ��ʽ�ַ� */
//	showFormatSymble();
//	RTCShowTimeFlag = 1;
	
	/* ��ʾ���� */
	showWeek();
	/* �״���ʾִֻ��һ����ʾ��ʽ�ַ� */
	onceDisAlterTime();

	
	OLED_ShowNum(2, 4, alarmArr[0], 4);
	OLED_ShowNum(2, 9, alarmArr[1], 2);
	OLED_ShowNum(2, 12, alarmArr[2], 2);
	/* ��ʾ����+ʱ�� */
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
  * @brief  ����RTC����
  * @param  hrtc RTC����ṹ���ַ
  * @param  timeArr	ʱ������
  * @retval ��
  */
void RTC_SetAlarm(RTC_HandleTypeDef *hrtc, uint16_t *timeArr) {
	/* �Ȼ�ȡ���µĵ�ǰʱ�䵽������ */
//	getTimeToArr();
	/* ������� */
//	OLED_ShowNum(1, 4, timeArr[3], 2);
//	OLED_ShowNum(1, 7, timeArr[4], 2);
//	OLED_ShowNum(1, 10, timeArr[5], 2);
	/* RTC���ӳ�ʼ�� */
	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_AlarmStructure.Alarm = RTC_ALARM_A;
	RTC_AlarmStructure.AlarmTime.Hours = timeArr[3];
	RTC_AlarmStructure.AlarmTime.Minutes = timeArr[4];
	RTC_AlarmStructure.AlarmTime.Seconds = timeArr[5];
	HAL_RTC_WaitForSynchro(hrtc);
	HAL_RTC_SetAlarm_IT(hrtc, &RTC_AlarmStructure, RTC_FORMAT_BIN);

	/* �������Ӻ�д���ݼĴ���DR7��д������ʱ���ܺ�����һλ�����ռ8λ��
	��0x01��ʾ�Ѿ����������ӣ�Ϊ�����豸�����ӻ��Ѻ���Ȼ�ܹ����� */
	rtcWriteBKPDataRegister(RTC_BKP_DR7, ((timeArr[3] + timeArr[4] + timeArr[5]) << 1) + 0x01);
//	rtcWriteBKPDataRegister(RTC_BKP_DR7, 0x01);
	
}




