#include "sys/sys.h"
#include "rtc/MyRTC.h"
#include "oled/OLED.h"
#include "beep/beep.h"
#include "timer/btim.h"
#include "encoder/encoder.h"
#include "oled/OnceDis.h"

//uint8_t RTCShowTimeFlag = 0;

uint16_t timeArr[7] = {2022, 12, 30, 23, 59, 59, 6}; // ��׼ʱ������
/* �޸�ʱ���־λ��1�� 2�� 3�� 4Сʱ 5���� 6���� */
uint8_t alterTimeFlag = 0;


/* new����  */
RTC_TimeTypeDef RTC_TimeStructure;			/* ʱ��ṹ�� */
RTC_DateTypeDef RTC_DateStructure;			/* ���ڽṹ�� */
//int16_t getYear;							/* ��ȡ���µ���� */
RTC_HandleTypeDef RTC_HandleStructure;		/* RTC����ṹ�� */

uint8_t alarmRingFlag = 0;					/* �������־λ ��1��ʾ���Ӷ�ʱ������0�����־λ */

/***********************************************************************************************************************/
/***************************************----------MyRTC��ʼ��----------*************************************************/

/**
  * @brief  RTC�����ʼ��������WKUP����ʹ�ܡ���ǰʱ�����ú������ж�����
  * @param  ��
  * @retval ��
  */
void MyRTC_Init(void) {
	uint16_t timeSum = 0;
	
	/* ����RTC״̬ */
//	HAL_StatusTypeDef RTC_Status;
	
	/* ����������͵�Դʱ�ӣ�ʹ�ܶԺ������RTC�ķ��� */
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	/* ʹ��LSEʱ��Դ��RTCʱ��Դ���� */
	__HAL_RCC_RTC_ENABLE();
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
	
	/* ʹ�ܻ������� */
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	/* RTC�����ʼ�� */
	RTC_HandleStructure.Init.AsynchPrediv = 32767;				/* ������������� 1s���1�� */
	RTC_HandleStructure.Init.OutPut = RTC_OUTPUTSOURCE_NONE;	/* ���ԴΪ�� */
	RTC_HandleStructure.Instance = RTC;							/* ����Ĵ�������ַ */
	HAL_RTC_Init(&RTC_HandleStructure);
	
	/* RTC�ж����ȼ����� */
	HAL_NVIC_SetPriority(RTC_IRQn, 2, 0);
	/* RTC�ж�ͨ��ʹ�� */
	HAL_NVIC_EnableIRQ(RTC_IRQn);
	
	/* RTC���ж�ʹ�� */
	SecondItEnable();

	/* ����Ҫ�޸ı�׼ʱ�䲢���״����س���ʱ�ſ����д��룬д���ݼĴ��� */
//	rtcWriteBKPDataRegister(RTC_BKP_DR1, 0x00);
	
	/* �����ݼĴ��� */ 
	if(rtcReadBKPDataRegister(RTC_BKP_DR1) == 0) {		/* Ϊ0��ʾ�״γ�ʼ�� */
		/* ����ʱ���� */
		setTimeToTypeDef();
		/* �������������� */
		setDateToTypeDef();
		/* ÿ���ϵ���ȱ��浱ǰ���ڵ����ݼĴ��� */
		beforeResetSaveDate(2);
		/* д���ݼĴ������ϵ������ʱ��ִֻ��һ�� */
		rtcWriteBKPDataRegister(RTC_BKP_DR1, 0x01);
	}else if(rtcReadBKPDataRegister(RTC_BKP_DR1) == 1){	/* Ϊ1��ʾ��λ����ú��������ȡ���ݵ����� */
		/* �ӱ��ݼĴ�����ȡ������������õ�RTC����� */
		getSaveDateFromBKP();
	}
	
	/* ��ȡPWR���ѱ�־λ���ж�mcu�Ƿ񱻻��� */
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU)) {
		/* �������ѣ����˳�����ģʽ��������ѱ�־λ�ʹ�����־λ */
		quitSleepMode();

		/* ���㵱ǰʱ��ʱ������ܺ� */ 
		timeSum = timeArr[3] + timeArr[4] + timeArr[5];
		/* �����ӻ��ѣ�����λ�������־λ����������ѱ�־λ�ͱ��ݼĴ���  */
		if((rtcReadBKPDataRegister(RTC_BKP_DR7) & 0x0001) == 0x01 && (rtcReadBKPDataRegister(RTC_BKP_DR7) >> 1) == timeSum) {
			rtcWriteBKPDataRegister(RTC_BKP_DR7, 0x00);
			alarmRingFlag = 1;
		}
	}
	
	// ��ȡ����ʱ�䵽������
	getTimeToArr(timeArr);

	/* ����RTC���Ӽ��ж� */
//	RTC_SetAlarm();
}

/***********************************************************************************************************************/
/*************************************----------���ж�ʹ/ʧ��----------*************************************************/

/**
  * @brief  ���ж�ʹ��
  * @param  ��
  * @retval ��
  */
void SecondItEnable(void){
	__HAL_RTC_SECOND_ENABLE_IT(&RTC_HandleStructure, RTC_IT_SEC);
}

/**
  * @brief  ���ж�ʧ��
  * @param  ��
  * @retval ��
  */
void SecondItDisable(void){
	__HAL_RTC_SECOND_DISABLE_IT(&RTC_HandleStructure, RTC_IT_SEC);
}

/***********************************************************************************************************************/
/***************************************----------��ʾ��ǰʱ������----------********************************************/

/**
  * @brief  ��ʾʱ������ڣ����ں�ʱ��ˢ����ʾ����ʽ����ֻ�ڵ�һ����ʾʱ���ú�����ʾ
  * @param  ��
  * @retval ��
  */
void showTimeAndDate(void) {
//	/* ��ʽ����ֻ�ڵ�һ�ε��øú���ʱִ����ʾһ�� */
//	showFormatSymble();
//	
//	/* ÿ���״���ʾ��׼ʱ��ǰ�����붨ʱ����ģʽ */
//	if(RTCShowTimeFlag == 0) {
//		timingGoSleep();
//	} 
	
	/* ��ʾ���� */
	showWeek();
	
	/* ֻ���״���ʾʱ��ִ��һ�� */
	onceDisTime();
	
	/* ��ȡ��ʱ���߱�־λ����ʱʱ�䵽���������ģʽ */
	if(getGoSleepModeFlag()) {
		goSleepMode();
	}	
	/* ��ʾ����+ʱ�� */
	OLED_ShowNum(2, 4, timeArr[0], 4);
	OLED_ShowNum(2, 9, timeArr[1], 2);
	OLED_ShowNum(2, 12, timeArr[2], 2);
	OLED_ShowNum(3, 5, timeArr[3], 2);
	OLED_ShowNum(3, 8, timeArr[4], 2);
	OLED_ShowNum(3, 11, timeArr[5], 2);

//	RTCShowTimeFlag = 1;
	
	/* ÿ����ʾ�󣬼�������Ƿ����ı䲢�������� */
	beforeResetSaveDate(1);
}

/**
  * @brief  ��ʾ���ڼ���ֻ�����ڸı�ʱ��ˢ����ʾһ�Σ�����ʱ��ˢ����ʾ
  * @param  ��
  * @retval ��
  */
void showWeek(void) {
	uint8_t weekDay = timeArr[6];				/* ��ǰ���ڼ� */
	static int8_t lastWeekDay = -1;				/* ���������ϴλ�ȡ�����ڣ�����ֵ-1ʹ���ܹ��״���ʾ */
	
	/* ����ǰ�������ϴε����ڲ�ͬ��������ʾʱ��ʱ����ˢ����ʾ���� */
	if(lastWeekDay != weekDay || OnceDisFlag == 0){					
		OLED_ShowString(4, 4, "          ");		/* �����ǰ����ʾ */
		switch(weekDay){
			case 0:
				OLED_ShowString(4, 6, "SUNDAY");	/* 0 ������ */
				break;
			case 1:
				OLED_ShowString(4, 6, "MONDAY");	/* 1 ����һ */
				break;
			case 2:
				OLED_ShowString(4, 6, "TUESDAY");	/* 2 ���ڶ� */
				break;
			case 3:
				OLED_ShowString(4, 5, "WEDNESDAY");	/* 3 ������ */
				break;
			case 4:
				OLED_ShowString(4, 5, "THURSDAY");	/* 4 ������ */
				break;
			case 5:
				OLED_ShowString(4, 6, "FRIDAY");	/* 5 ������ */
				break;
			case 6:
				OLED_ShowString(4, 5, "SATURDAY");	/* 6 ������ */
				break;
			default:
				OLED_ShowString(4, 5, "ERROR");		/* Ĭ����ʾERROR */
				break;
		}
	}
	lastWeekDay = weekDay;							/* ����ִ�е�ĩβ�������ִ�к���ʱ������ */
}

/***********************************************************************************************************************/
/***************************************----------����/��ȡ����ʱ��----------*******************************************/

/**
  * @brief  ����ʱ�䣺ʱ �� ��
  * @param  ��
  * @retval ��
  */
void setTimeToTypeDef(void) {
	RTC_TimeStructure.Hours = timeArr[3];
	RTC_TimeStructure.Minutes = timeArr[4];
	RTC_TimeStructure.Seconds = timeArr[5];
	
	/* �ȴ�ʱ��ͬ�� */
	HAL_RTC_WaitForSynchro(&RTC_HandleStructure);
	/* ����ʱ�䣬ֱ�Ӹ��¼����Ĵ��� */
	HAL_RTC_SetTime(&RTC_HandleStructure, &RTC_TimeStructure, RTC_FORMAT_BIN);
}

/**
  * @brief  �������ڣ��� �� �� ��
  * @param  ��
  * @retval ��
  */
void setDateToTypeDef(void) {
	/* �ṹ�������Ա��ֵ */
	RTC_DateStructure.Year = (uint8_t)(timeArr[0] - 2000);
	RTC_DateStructure.Month = timeArr[1];
	RTC_DateStructure.Date = timeArr[2];
	RTC_DateStructure.WeekDay = timeArr[6];
	
	/* �ȴ�ʱ��ͬ�� */
	HAL_RTC_WaitForSynchro(&RTC_HandleStructure);
	/* �������ڣ�����RTC�����������ֵ */
	HAL_RTC_SetDate(&RTC_HandleStructure, &RTC_DateStructure, RTC_FORMAT_BIN);
}

/**
  * @brief  ��ȡ��׼ʱ�䵽ʱ��������
  * @param  ��
  * @retval ��
  */
void getTimeToArr(uint16_t *Arr) {
	/* ��ȡ��ǰʱ�� */
	HAL_RTC_GetTime(&RTC_HandleStructure, &RTC_TimeStructure, RTC_FORMAT_BIN);
	/* ��ȡ��ǰ���� */
	HAL_RTC_GetDate(&RTC_HandleStructure, &RTC_DateStructure, RTC_FORMAT_BIN);
	/* ��ǰʱ������ڸ�ֵ������ͱ��� */
	Arr[0] = RTC_DateStructure.Year + 2000;
	Arr[1] = RTC_DateStructure.Month;
	Arr[2] = RTC_DateStructure.Date;
	Arr[3] = RTC_TimeStructure.Hours;
	Arr[4] = RTC_TimeStructure.Minutes;
	Arr[5] = RTC_TimeStructure.Seconds;
	Arr[6] = RTC_DateStructure.WeekDay;
}

/***********************************************************************************************************************/
/***************************************----------��׼ʱ������ģʽ----------********************************************/

/**
  * @brief  �����׼ʱ������ģʽ��ִ�г�ʼ����־λ�Ȳ���
  * @param  ��
  * @retval ��
  */
void goAlterTimeMode(void) {
	BTIM_TIM2_ENABLE();			/* ʹ��TIM2 */
	alterTimeFlag = 0;
	SecondItDisable();			/* ʧ�����ж� */
}

/**
  * @brief  �˳���׼ʱ������ģʽ
  * @param  ��
  * @retval ��
  */
void quitAlterTimeMode(void) {
	BTIM_TIM2_DISABLE();		/* ʧ��TIM2 */
	setTimeToTypeDef();
	setDateToTypeDef();
	SecondItEnable();			/* ʹ�����ж� */
}

/**
  * @brief  �л��޸�ʱ���λ��ʱ���֡���ȣ������±�־λ
  * @param  ��
  * @retval ��
  */
void toggleAlterTimeBit(void) {
	alterTimeFlag++;
	if(alterTimeFlag == 6) alterTimeFlag = 0;
}

/**
  * @brief  �޸ı�׼ʱ��
  * @param  ��
  * @retval ��
  */
void alterTime(void) {
	uint8_t alterTimeAddOrSub = getAlterTimeAddOrSub();
	if(alterTimeAddOrSub == 1) {
		if(alterTimeFlag == 0){
			if(timeArr[0] < 3000) timeArr[0]++;
			else timeArr[0] = 2024;
		}else if(alterTimeFlag == 1){
			if(timeArr[1] < 12) timeArr[1]++;
			else timeArr[1] = 1;
		}else if(alterTimeFlag == 2){
			if(timeArr[2] < 31) timeArr[2]++;
			else timeArr[2] = 1;
		}else if(alterTimeFlag == 3){
			if(timeArr[3] < 23) timeArr[3]++;
			else timeArr[3] = 0;
		}else if(alterTimeFlag == 4){
			if(timeArr[4] < 59) timeArr[4]++;
			else timeArr[4] = 0;
		}else if(alterTimeFlag == 5){
			if(timeArr[5] < 59) timeArr[5]++;
			else timeArr[5] = 0;
		}
	}else if(alterTimeAddOrSub == 2) {
		if(alterTimeFlag == 0){
			if(timeArr[0] > 1000) timeArr[0]--;
			else timeArr[0] = 2024;
		}else if(alterTimeFlag == 1){
			if(timeArr[1] > 1) timeArr[1]--;
			else timeArr[1] = 12;
		}else if(alterTimeFlag == 2){
			if(timeArr[2] > 1) timeArr[2]--;
			else timeArr[2] = 31;
		}else if(alterTimeFlag == 3){
			if(timeArr[3] > 0) timeArr[3]--;
			else timeArr[3] = 23;
		}else if(alterTimeFlag == 4){
			if(timeArr[4] > 0) timeArr[4]--;
			else timeArr[4] = 59;
		}else if(alterTimeFlag == 5){
			if(timeArr[5] > 0) timeArr[5]--;
			else timeArr[5] = 59;
		}
	}
}

/**
  * @brief  ��ʾ�޸�ʱ��
  * @param  ��
  * @retval ��
  */
void showAlterTime(void) {
	uint8_t AlterTimeDisplayFlag = getAlterTimeDisplayFlag();
	
//	showFormatSymble();
//	RTCShowTimeFlag = 1;
	
	/* ��ʾ���� */
	showWeek();
	onceDisAlterTime();
	
	
	/* ��ʾ����+ʱ�� */
	if(alterTimeFlag == 0) {
		if(AlterTimeDisplayFlag == 1) OLED_ShowNum(2, 4, timeArr[0], 4);
		else OLED_ShowString(2, 4, "    ");
		OLED_ShowNum(2, 9, timeArr[1], 2);
		OLED_ShowNum(2, 12, timeArr[2], 2);
		OLED_ShowNum(3, 5, timeArr[3], 2);
		OLED_ShowNum(3, 8, timeArr[4], 2);
		OLED_ShowNum(3, 11, timeArr[5], 2);
	}else if(alterTimeFlag == 1) {
		if(AlterTimeDisplayFlag == 1) 	OLED_ShowNum(2, 9, timeArr[1], 2);
		else OLED_ShowString(2, 9, "  ");
		OLED_ShowNum(2, 4, timeArr[0], 4);
		OLED_ShowNum(2, 12, timeArr[2], 2);
		OLED_ShowNum(3, 5, timeArr[3], 2);
		OLED_ShowNum(3, 8, timeArr[4], 2);
		OLED_ShowNum(3, 11, timeArr[5], 2);
	}else if(alterTimeFlag == 2) {
		if(AlterTimeDisplayFlag == 1) OLED_ShowNum(2, 12, timeArr[2], 2);
		else OLED_ShowString(2, 12, "  ");
		OLED_ShowNum(2, 4, timeArr[0], 4);
		OLED_ShowNum(2, 9, timeArr[1], 2);
		OLED_ShowNum(3, 5, timeArr[3], 2);
		OLED_ShowNum(3, 8, timeArr[4], 2);
		OLED_ShowNum(3, 11, timeArr[5], 2);
	}else if(alterTimeFlag == 3) {
		if(AlterTimeDisplayFlag == 1) OLED_ShowNum(3, 5, timeArr[3], 2);
		else OLED_ShowString(3, 5, "  ");
		OLED_ShowNum(2, 4, timeArr[0], 4);
		OLED_ShowNum(2, 9, timeArr[1], 2);
		OLED_ShowNum(2, 12, timeArr[2], 2);
		OLED_ShowNum(3, 8, timeArr[4], 2);
		OLED_ShowNum(3, 11, timeArr[5], 2);
	}else if(alterTimeFlag == 4) {
		if(AlterTimeDisplayFlag == 1) OLED_ShowNum(3, 8, timeArr[4], 2);
		else OLED_ShowString(3, 8, "  ");
		OLED_ShowNum(2, 4, timeArr[0], 4);
		OLED_ShowNum(2, 9, timeArr[1], 2);
		OLED_ShowNum(2, 12, timeArr[2], 2);
		OLED_ShowNum(3, 5, timeArr[3], 2);
		OLED_ShowNum(3, 11, timeArr[5], 2);
	}else if(alterTimeFlag == 5) {
		if(AlterTimeDisplayFlag == 1) OLED_ShowNum(3, 11, timeArr[5], 2);
		else OLED_ShowString(3, 11, "  ");
		OLED_ShowNum(2, 4, timeArr[0], 4);
		OLED_ShowNum(2, 9, timeArr[1], 2);
		OLED_ShowNum(2, 12, timeArr[2], 2);
		OLED_ShowNum(3, 5, timeArr[3], 2);
		OLED_ShowNum(3, 8, timeArr[4], 2);
	}
	alterTime();
	

}

/***********************************************************************************************************************/
/***************************************---------�������ݼĴ���----------***********************************************/

/**
  * @brief  �����ݼĴ���
  * @param  BackupRegister ���ݼĴ��� RTC_BKP_DR1 - RTC_BKP_DR10
  * @retval 32λ����
  */
uint32_t rtcReadBKPDataRegister(uint32_t BackupRegister) {
	uint32_t temp;
	temp = HAL_RTCEx_BKUPRead(&RTC_HandleStructure, BackupRegister);
	return temp;
}

/**
  * @brief  д���ݼĴ���
  * @param  BackupRegister ���ݼĴ��� RTC_BKP_DR1 - RTC_BKP_DR10
  * @param  Data д�뱸�ݼĴ���������
  * @retval ��
  */
void rtcWriteBKPDataRegister(uint32_t BackupRegister, uint32_t Data){
	HAL_PWR_EnableBkUpAccess();
	HAL_RTCEx_BKUPWrite(&RTC_HandleStructure, BackupRegister, Data);
}

/**
  * @brief  ��ϵͳ��λǰ�������ڵ����ݼĴ��������������ڣ�����ı�ʱ�ű���
  * @param  select 1ѡ����ı�ʱ�ű��棬2ѡ������������
  * @retval ��
  */
void beforeResetSaveDate(uint8_t select) {
	/* �����ϴν���ú���ʱ������ֵ�������ж������Ƿ�ı� */
	static int8_t lastYear = -1, lastMonth = -1, lastDate = -1, lastWeek = -1;
	/* ���浱ǰ����ֵ */
	int16_t year = timeArr[0] - 2000;
	uint8_t month = timeArr[1], date = timeArr[2], week = timeArr[6];
	/* ����ֵ�ı�ʱ�����б��ݱ��� */
	if(select == 1) {
		if(year != lastYear){
			rtcWriteBKPDataRegister(RTC_BKP_DR2, year);
		}                           
		if(month != lastMonth){     
			rtcWriteBKPDataRegister(RTC_BKP_DR3, month);
		}                           
		if(date != lastDate){       
			rtcWriteBKPDataRegister(RTC_BKP_DR4, date);
		}                           
		if(week != lastWeek){       
			rtcWriteBKPDataRegister(RTC_BKP_DR5, week);
		}
	/* ��������ֵ�Ƿ����ı䣬�����ݱ��� */
	}else if(select == 2) {
		rtcWriteBKPDataRegister(RTC_BKP_DR2, year);
		rtcWriteBKPDataRegister(RTC_BKP_DR3, month);
		rtcWriteBKPDataRegister(RTC_BKP_DR4, date);
		rtcWriteBKPDataRegister(RTC_BKP_DR5, week);
	}
	
	/* ������뺯��ʱ������ */
	lastYear = year;
	lastMonth = month;
	lastDate = date;
	lastWeek = week;
}

/**
  * @brief  �ӱ��ݼĴ����л�ȡ��������ڣ������õ�RTC����ṹ����
  * @param  ��
  * @retval ��
  */
void getSaveDateFromBKP(void) {
	/* �����ݼĴ�����ȡ����ֵ�����±�׼���� */
	timeArr[0] = rtcReadBKPDataRegister(RTC_BKP_DR2) + 2000;
	timeArr[1] = rtcReadBKPDataRegister(RTC_BKP_DR3);
	timeArr[2] = rtcReadBKPDataRegister(RTC_BKP_DR4);
	timeArr[6] = rtcReadBKPDataRegister(RTC_BKP_DR5);
	setDateToTypeDef();
}


/**
  * @brief  ��������ģʽ������
  * @param  ��
  * @retval ��
  */
void goSleepMode(void) {
	OLED_Clear();
	HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  �˳�����ģʽ�����������־λ��������ѱ�־λ
  * @param  ��
  * @retval ��
  */
void quitSleepMode(void) {
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	HAL_Delay(1);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
}

/**
  * @brief  ���붨ʱ����ģʽ�����������ֵ��ʹ��TIM4
  * @param  ��
  * @retval ��
  */
void timingGoSleep(void) {
	setTIM4SCount(0);
	BTIM_TIM4_ENABLE();
}

/**
  * @brief  �˳���ʱ����ģʽ�������ʱ���߱�־λ�����������ֵ��ʧ��TIM4
  * @param  ��
  * @retval ��
  */
void timingQuitSleep(void) {
	getGoSleepModeFlag();
	setTIM4SCount(0);
	BTIM_TIM4_DISABLE();
}

/***********************************************************************************************************************/
/***************************************----------RTC�жϷ�����----------*********************************************/

/**
  * @brief  RTC�жϷ�����
  * @param  ��
  * @retval ��
  */
void RTC_IRQHandler(void) {
	static uint8_t alarmRingCount = 0;	/* ��������ת�����ƴ� */
	/* ����RTC�����ж� */
	if(__HAL_RTC_ALARM_GET_FLAG(&RTC_HandleStructure, RTC_IT_ALRA) != RESET){
		/* ����RTC�����ж����������ڲ����ûص����� HAL_RTC_AlarmAEventCallback */
		HAL_RTC_AlarmIRQHandler(&RTC_HandleStructure);
		/* ��������ӱ�־��Ҳ����λ���ѱ�־λ����������Ҫ������ӱ�־λ�ͻ��ѱ�־λ */
		__HAL_RTC_ALARM_CLEAR_FLAG(&RTC_HandleStructure, RTC_FLAG_ALRAF);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	/* ����RTC���ж� */
	}else if(__HAL_RTC_SECOND_GET_FLAG(&RTC_HandleStructure, RTC_FLAG_SEC) != RESET) {
		/* �ж��������־λ�Ƿ���1 */
		if(alarmRingFlag == 1) {
			Beep_Turn();				/* ��������1sʱ������ת */
			if(alarmRingCount == 10) {	/* ���Ӷ�ʱ������������5�� */
				/* �����־λ�ͼƴ�ֵ���رշ����� */
				alarmRingCount = 0;		
				alarmRingFlag = 0;
				Beep_OFF();
			}else alarmRingCount ++;
		}

		/* ��ȡ����ʱ�䣬���µ�ǰʱ�� */
		getTimeToArr(timeArr);
		/* ������жϱ�־ */
		__HAL_RTC_SECOND_CLEAR_FLAG(&RTC_HandleStructure, RTC_FLAG_SEC);
	}
	/* ���RTC����жϱ�־ */
	__HAL_RTC_OVERFLOW_CLEAR_FLAG(&RTC_HandleStructure, RTC_FLAG_OW);
	
}

/**
  * @brief  ���ӻص��������������ж�����������
  * @param  hrtc RTC���ָ��
  * @retval ��
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
	/* ���Ӷ�ʱ�����������־λ��1 */
	alarmRingFlag = 1;
	/* ��ִ���˸ûص�������˵��mcu���ڴ���ģʽ�£����������ݼĴ����д洢���������ñ�־ */
	rtcWriteBKPDataRegister(RTC_BKP_DR7, 0x00);
}




