#include "sys/sys.h"
#include "rtc/MyRTC.h"
#include "oled/OLED.h"
#include "beep/beep.h"
#include "timer/btim.h"
#include "encoder/encoder.h"
#include "oled/OnceDis.h"

//uint8_t RTCShowTimeFlag = 0;

uint16_t timeArr[7] = {2022, 12, 30, 23, 59, 59, 6}; // 标准时间数组
/* 修改时间标志位，1年 2月 3日 4小时 5分钟 6秒钟 */
uint8_t alterTimeFlag = 0;


/* new变量  */
RTC_TimeTypeDef RTC_TimeStructure;			/* 时间结构体 */
RTC_DateTypeDef RTC_DateStructure;			/* 日期结构体 */
//int16_t getYear;							/* 获取最新的年份 */
RTC_HandleTypeDef RTC_HandleStructure;		/* RTC句柄结构体 */

uint8_t alarmRingFlag = 0;					/* 闹钟响标志位 置1表示闹钟定时到，置0清除标志位 */

/***********************************************************************************************************************/
/***************************************----------MyRTC初始化----------*************************************************/

/**
  * @brief  RTC外设初始化，包括WKUP引脚使能、当前时间设置和闹钟中断配置
  * @param  无
  * @retval 无
  */
void MyRTC_Init(void) {
	uint16_t timeSum = 0;
	
	/* 保存RTC状态 */
//	HAL_StatusTypeDef RTC_Status;
	
	/* 开启后备区域和电源时钟，使能对后备区域和RTC的访问 */
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	
	/* 使能LSE时钟源和RTC时钟源配置 */
	__HAL_RCC_RTC_ENABLE();
	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
	__HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
	
	/* 使能唤醒引脚 */
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	/* RTC句柄初始化 */
	RTC_HandleStructure.Init.AsynchPrediv = 32767;				/* 计数器溢出周期 1s溢出1次 */
	RTC_HandleStructure.Init.OutPut = RTC_OUTPUTSOURCE_NONE;	/* 输出源为无 */
	RTC_HandleStructure.Instance = RTC;							/* 外设寄存器基地址 */
	HAL_RTC_Init(&RTC_HandleStructure);
	
	/* RTC中断优先级设置 */
	HAL_NVIC_SetPriority(RTC_IRQn, 2, 0);
	/* RTC中断通道使能 */
	HAL_NVIC_EnableIRQ(RTC_IRQn);
	
	/* RTC秒中断使能 */
	SecondItEnable();

	/* 当需要修改标准时间并且首次下载程序时放开该行代码，写备份寄存器 */
//	rtcWriteBKPDataRegister(RTC_BKP_DR1, 0x00);
	
	/* 读备份寄存器 */ 
	if(rtcReadBKPDataRegister(RTC_BKP_DR1) == 0) {		/* 为0表示首次初始化 */
		/* 设置时分秒 */
		setTimeToTypeDef();
		/* 设置年月日星期 */
		setDateToTypeDef();
		/* 每次上电后先保存当前日期到备份寄存器 */
		beforeResetSaveDate(2);
		/* 写备份寄存器，上电后设置时间只执行一次 */
		rtcWriteBKPDataRegister(RTC_BKP_DR1, 0x01);
	}else if(rtcReadBKPDataRegister(RTC_BKP_DR1) == 1){	/* 为1表示复位进入该函数，需读取备份的日期 */
		/* 从备份寄存器获取保存的日期设置到RTC句柄中 */
		getSaveDateFromBKP();
	}
	
	/* 获取PWR唤醒标志位，判断mcu是否被唤醒 */
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU)) {
		/* 若被唤醒，则退出休眠模式，清除唤醒标志位和待机标志位 */
		quitSleepMode();

		/* 计算当前时间时分秒的总和 */ 
		timeSum = timeArr[3] + timeArr[4] + timeArr[5];
		/* 由闹钟唤醒，则置位闹钟响标志位，并清除唤醒标志位和备份寄存器  */
		if((rtcReadBKPDataRegister(RTC_BKP_DR7) & 0x0001) == 0x01 && (rtcReadBKPDataRegister(RTC_BKP_DR7) >> 1) == timeSum) {
			rtcWriteBKPDataRegister(RTC_BKP_DR7, 0x00);
			alarmRingFlag = 1;
		}
	}
	
	// 获取最新时间到数组中
	getTimeToArr(timeArr);

	/* 设置RTC闹钟及中断 */
//	RTC_SetAlarm();
}

/***********************************************************************************************************************/
/*************************************----------秒中断使/失能----------*************************************************/

/**
  * @brief  秒中断使能
  * @param  无
  * @retval 无
  */
void SecondItEnable(void){
	__HAL_RTC_SECOND_ENABLE_IT(&RTC_HandleStructure, RTC_IT_SEC);
}

/**
  * @brief  秒中断失能
  * @param  无
  * @retval 无
  */
void SecondItDisable(void){
	__HAL_RTC_SECOND_DISABLE_IT(&RTC_HandleStructure, RTC_IT_SEC);
}

/***********************************************************************************************************************/
/***************************************----------显示当前时间日期----------********************************************/

/**
  * @brief  显示时间和日期，日期和时间刷新显示，格式符号只在第一次显示时调用函数显示
  * @param  无
  * @retval 无
  */
void showTimeAndDate(void) {
//	/* 格式符号只在第一次调用该函数时执行显示一次 */
//	showFormatSymble();
//	
//	/* 每次首次显示标准时间前，进入定时休眠模式 */
//	if(RTCShowTimeFlag == 0) {
//		timingGoSleep();
//	} 
	
	/* 显示星期 */
	showWeek();
	
	/* 只在首次显示时间执行一次 */
	onceDisTime();
	
	/* 获取定时休眠标志位，定时时间到则进入休眠模式 */
	if(getGoSleepModeFlag()) {
		goSleepMode();
	}	
	/* 显示日期+时间 */
	OLED_ShowNum(2, 4, timeArr[0], 4);
	OLED_ShowNum(2, 9, timeArr[1], 2);
	OLED_ShowNum(2, 12, timeArr[2], 2);
	OLED_ShowNum(3, 5, timeArr[3], 2);
	OLED_ShowNum(3, 8, timeArr[4], 2);
	OLED_ShowNum(3, 11, timeArr[5], 2);

//	RTCShowTimeFlag = 1;
	
	/* 每次显示后，检查日期是否发生改变并保存日期 */
	beforeResetSaveDate(1);
}

/**
  * @brief  显示星期几，只有星期改变时才刷新显示一次，不变时不刷新显示
  * @param  无
  * @retval 无
  */
void showWeek(void) {
	uint8_t weekDay = timeArr[6];				/* 当前星期几 */
	static int8_t lastWeekDay = -1;				/* 用来保存上次获取的星期，赋初值-1使得能够首次显示 */
	
	/* 若当前星期与上次的星期不同或重新显示时间时，则刷新显示星期 */
	if(lastWeekDay != weekDay || OnceDisFlag == 0){					
		OLED_ShowString(4, 4, "          ");		/* 清除先前的显示 */
		switch(weekDay){
			case 0:
				OLED_ShowString(4, 6, "SUNDAY");	/* 0 星期日 */
				break;
			case 1:
				OLED_ShowString(4, 6, "MONDAY");	/* 1 星期一 */
				break;
			case 2:
				OLED_ShowString(4, 6, "TUESDAY");	/* 2 星期二 */
				break;
			case 3:
				OLED_ShowString(4, 5, "WEDNESDAY");	/* 3 星期三 */
				break;
			case 4:
				OLED_ShowString(4, 5, "THURSDAY");	/* 4 星期四 */
				break;
			case 5:
				OLED_ShowString(4, 6, "FRIDAY");	/* 5 星期五 */
				break;
			case 6:
				OLED_ShowString(4, 5, "SATURDAY");	/* 6 星期六 */
				break;
			default:
				OLED_ShowString(4, 5, "ERROR");		/* 默认显示ERROR */
				break;
		}
	}
	lastWeekDay = weekDay;							/* 函数执行到末尾保存这次执行函数时的星期 */
}

/***********************************************************************************************************************/
/***************************************----------设置/获取日期时间----------*******************************************/

/**
  * @brief  设置时间：时 分 秒
  * @param  无
  * @retval 无
  */
void setTimeToTypeDef(void) {
	RTC_TimeStructure.Hours = timeArr[3];
	RTC_TimeStructure.Minutes = timeArr[4];
	RTC_TimeStructure.Seconds = timeArr[5];
	
	/* 等待时钟同步 */
	HAL_RTC_WaitForSynchro(&RTC_HandleStructure);
	/* 更新时间，直接更新计数寄存器 */
	HAL_RTC_SetTime(&RTC_HandleStructure, &RTC_TimeStructure, RTC_FORMAT_BIN);
}

/**
  * @brief  设置日期：年 月 日 周
  * @param  无
  * @retval 无
  */
void setDateToTypeDef(void) {
	/* 结构体变量成员赋值 */
	RTC_DateStructure.Year = (uint8_t)(timeArr[0] - 2000);
	RTC_DateStructure.Month = timeArr[1];
	RTC_DateStructure.Date = timeArr[2];
	RTC_DateStructure.WeekDay = timeArr[6];
	
	/* 等待时钟同步 */
	HAL_RTC_WaitForSynchro(&RTC_HandleStructure);
	/* 设置日期，更新RTC句柄日期属性值 */
	HAL_RTC_SetDate(&RTC_HandleStructure, &RTC_DateStructure, RTC_FORMAT_BIN);
}

/**
  * @brief  获取标准时间到时间数组中
  * @param  无
  * @retval 无
  */
void getTimeToArr(uint16_t *Arr) {
	/* 获取当前时间 */
	HAL_RTC_GetTime(&RTC_HandleStructure, &RTC_TimeStructure, RTC_FORMAT_BIN);
	/* 获取当前日期 */
	HAL_RTC_GetDate(&RTC_HandleStructure, &RTC_DateStructure, RTC_FORMAT_BIN);
	/* 当前时间和日期赋值给数组和变量 */
	Arr[0] = RTC_DateStructure.Year + 2000;
	Arr[1] = RTC_DateStructure.Month;
	Arr[2] = RTC_DateStructure.Date;
	Arr[3] = RTC_TimeStructure.Hours;
	Arr[4] = RTC_TimeStructure.Minutes;
	Arr[5] = RTC_TimeStructure.Seconds;
	Arr[6] = RTC_DateStructure.WeekDay;
}

/***********************************************************************************************************************/
/***************************************----------标准时间配置模式----------********************************************/

/**
  * @brief  进入标准时间配置模式，执行初始化标志位等操作
  * @param  无
  * @retval 无
  */
void goAlterTimeMode(void) {
	BTIM_TIM2_ENABLE();			/* 使能TIM2 */
	alterTimeFlag = 0;
	SecondItDisable();			/* 失能秒中断 */
}

/**
  * @brief  退出标准时间配置模式
  * @param  无
  * @retval 无
  */
void quitAlterTimeMode(void) {
	BTIM_TIM2_DISABLE();		/* 失能TIM2 */
	setTimeToTypeDef();
	setDateToTypeDef();
	SecondItEnable();			/* 使能秒中断 */
}

/**
  * @brief  切换修改时间的位（时、分、秒等），更新标志位
  * @param  无
  * @retval 无
  */
void toggleAlterTimeBit(void) {
	alterTimeFlag++;
	if(alterTimeFlag == 6) alterTimeFlag = 0;
}

/**
  * @brief  修改标准时间
  * @param  无
  * @retval 无
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
  * @brief  显示修改时间
  * @param  无
  * @retval 无
  */
void showAlterTime(void) {
	uint8_t AlterTimeDisplayFlag = getAlterTimeDisplayFlag();
	
//	showFormatSymble();
//	RTCShowTimeFlag = 1;
	
	/* 显示星期 */
	showWeek();
	onceDisAlterTime();
	
	
	/* 显示日期+时间 */
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
/***************************************---------操作备份寄存器----------***********************************************/

/**
  * @brief  读备份寄存器
  * @param  BackupRegister 备份寄存器 RTC_BKP_DR1 - RTC_BKP_DR10
  * @retval 32位数据
  */
uint32_t rtcReadBKPDataRegister(uint32_t BackupRegister) {
	uint32_t temp;
	temp = HAL_RTCEx_BKUPRead(&RTC_HandleStructure, BackupRegister);
	return temp;
}

/**
  * @brief  写备份寄存器
  * @param  BackupRegister 备份寄存器 RTC_BKP_DR1 - RTC_BKP_DR10
  * @param  Data 写入备份寄存器的数据
  * @retval 无
  */
void rtcWriteBKPDataRegister(uint32_t BackupRegister, uint32_t Data){
	HAL_PWR_EnableBkUpAccess();
	HAL_RTCEx_BKUPWrite(&RTC_HandleStructure, BackupRegister, Data);
}

/**
  * @brief  在系统复位前保存日期到备份寄存器：年月日星期，当其改变时才保存
  * @param  select 1选择其改变时才保存，2选择无条件保存
  * @retval 无
  */
void beforeResetSaveDate(uint8_t select) {
	/* 保存上次进入该函数时的日期值，用于判断日期是否改变 */
	static int8_t lastYear = -1, lastMonth = -1, lastDate = -1, lastWeek = -1;
	/* 保存当前日期值 */
	int16_t year = timeArr[0] - 2000;
	uint8_t month = timeArr[1], date = timeArr[2], week = timeArr[6];
	/* 日期值改变时，进行备份保存 */
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
	/* 无论日期值是否发生改变，都备份保存 */
	}else if(select == 2) {
		rtcWriteBKPDataRegister(RTC_BKP_DR2, year);
		rtcWriteBKPDataRegister(RTC_BKP_DR3, month);
		rtcWriteBKPDataRegister(RTC_BKP_DR4, date);
		rtcWriteBKPDataRegister(RTC_BKP_DR5, week);
	}
	
	/* 保存进入函数时的日期 */
	lastYear = year;
	lastMonth = month;
	lastDate = date;
	lastWeek = week;
}

/**
  * @brief  从备份寄存器中获取保存的日期，并设置到RTC句柄结构体中
  * @param  无
  * @retval 无
  */
void getSaveDateFromBKP(void) {
	/* 读备份寄存器获取日期值，更新标准日期 */
	timeArr[0] = rtcReadBKPDataRegister(RTC_BKP_DR2) + 2000;
	timeArr[1] = rtcReadBKPDataRegister(RTC_BKP_DR3);
	timeArr[2] = rtcReadBKPDataRegister(RTC_BKP_DR4);
	timeArr[6] = rtcReadBKPDataRegister(RTC_BKP_DR5);
	setDateToTypeDef();
}


/**
  * @brief  进入休眠模式，清屏
  * @param  无
  * @retval 无
  */
void goSleepMode(void) {
	OLED_Clear();
	HAL_PWR_EnterSTANDBYMode();
}

/**
  * @brief  退出休眠模式，清除待机标志位，清除唤醒标志位
  * @param  无
  * @retval 无
  */
void quitSleepMode(void) {
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	HAL_Delay(1);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
}

/**
  * @brief  进入定时休眠模式，清零秒计数值，使能TIM4
  * @param  无
  * @retval 无
  */
void timingGoSleep(void) {
	setTIM4SCount(0);
	BTIM_TIM4_ENABLE();
}

/**
  * @brief  退出定时休眠模式，清除定时休眠标志位，清零秒计数值，失能TIM4
  * @param  无
  * @retval 无
  */
void timingQuitSleep(void) {
	getGoSleepModeFlag();
	setTIM4SCount(0);
	BTIM_TIM4_DISABLE();
}

/***********************************************************************************************************************/
/***************************************----------RTC中断服务函数----------*********************************************/

/**
  * @brief  RTC中断服务函数
  * @param  无
  * @retval 无
  */
void RTC_IRQHandler(void) {
	static uint8_t alarmRingCount = 0;	/* 蜂鸣器翻转次数计次 */
	/* 触发RTC闹钟中断 */
	if(__HAL_RTC_ALARM_GET_FLAG(&RTC_HandleStructure, RTC_IT_ALRA) != RESET){
		/* 处理RTC闹钟中断请求函数，内部调用回调函数 HAL_RTC_AlarmAEventCallback */
		HAL_RTC_AlarmIRQHandler(&RTC_HandleStructure);
		/* 因产生闹钟标志后也会置位唤醒标志位，所以在这要清除闹钟标志位和唤醒标志位 */
		__HAL_RTC_ALARM_CLEAR_FLAG(&RTC_HandleStructure, RTC_FLAG_ALRAF);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	/* 触发RTC秒中断 */
	}else if(__HAL_RTC_SECOND_GET_FLAG(&RTC_HandleStructure, RTC_FLAG_SEC) != RESET) {
		/* 判断闹钟响标志位是否置1 */
		if(alarmRingFlag == 1) {
			Beep_Turn();				/* 蜂鸣器以1s时间间隔翻转 */
			if(alarmRingCount == 10) {	/* 闹钟定时到，蜂鸣器响5下 */
				/* 清除标志位和计次值，关闭蜂鸣器 */
				alarmRingCount = 0;		
				alarmRingFlag = 0;
				Beep_OFF();
			}else alarmRingCount ++;
		}

		/* 获取最新时间，更新当前时间 */
		getTimeToArr(timeArr);
		/* 清除秒中断标志 */
		__HAL_RTC_SECOND_CLEAR_FLAG(&RTC_HandleStructure, RTC_FLAG_SEC);
	}
	/* 清除RTC溢出中断标志 */
	__HAL_RTC_OVERFLOW_CLEAR_FLAG(&RTC_HandleStructure, RTC_FLAG_OW);
	
}

/**
  * @brief  闹钟回调函数，被闹钟中断请求函数调用
  * @param  hrtc RTC句柄指针
  * @retval 无
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
	/* 闹钟定时到，闹钟响标志位置1 */
	alarmRingFlag = 1;
	/* 当执行了该回调函数，说明mcu不在待机模式下，因此清除备份寄存器中存储的闹钟设置标志 */
	rtcWriteBKPDataRegister(RTC_BKP_DR7, 0x00);
}




