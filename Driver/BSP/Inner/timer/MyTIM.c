#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "OLED.h"
#include "MyRTC.h"
#include "Beep.h"

extern uint8_t flashFlag;
extern uint8_t alarmFlag;
extern uint8_t breathFlag;
extern uint8_t firstQuit;
extern uint8_t secondWatchFlag;
extern uint8_t alarmConfigFlag;
// TIM3定时计数标志，TIM3定时目标计数值
uint16_t countTIM3 = 0, countTIM3Target = 15;
// TIM4中断计数标志
uint8_t countTIM4 = 0;
// TIM4秒表计时，时分秒
uint8_t TIM4Num[3] = {0, 0, 0};

/**
  * @brief  TIM2初始化
  * @param  无
  * @retval 无
  */
void MyTIM2_Init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;		// 重装载值5000，计5000个数，0.5s计满
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 时钟周期100us
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	NVIC_SetPriorityGrouping(2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_SetCounter(TIM2, 5000);
	TIM_Cmd(TIM2, DISABLE);
}

/**
  * @brief  TIM3初始化
  * @param  无
  * @retval 无
  */
void MyTIM3_Init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;		// 重装载值10000, 溢出时间为1s
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 时钟周期100us
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	NVIC_SetPriorityGrouping(2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_SetCounter(TIM3, 10000);
	TIM_Cmd(TIM3, DISABLE);
}

/**
  * @brief  TIM4初始化
  * @param  无
  * @retval 无
  */
void MyTIM4_Init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;		// 重装载值1000, 溢出时间为1ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // 时钟周期1us
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
	NVIC_SetPriorityGrouping(2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_SetCounter(TIM4, 1000);
	TIM_Cmd(TIM4, DISABLE);
}

/**
  * @brief  设置TIM3定时计数目标
  * @param  sleepTimeP 休眠时间挡位数组
  * @param  sleepTimeFlagP 休眠时间档位+单位标志位数组
  * @retval 无
  */
void setCountTIM3Target(uint8_t* sleepTimeP, uint8_t* sleepTimeFlagP) {
	countTIM3 = 0;						// 设置之前，先重置countTIM3计数值，否则可能会出现等待非常久才休眠的bug
	if(sleepTimeFlagP[1] == 0) {		// 秒级
		countTIM3Target = sleepTimeP[sleepTimeFlagP[0]];	
	}else if(sleepTimeFlagP[1] == 1){	// 分钟级
		countTIM3Target = sleepTimeP[sleepTimeFlagP[0]] * 60;
	}
}

/**
  * @brief  重置TIM4秒表时间
  * @param  无
  * @retval 无
  */
void resetTIM4Num(void) {
	TIM4Num[0] = 0;
	TIM4Num[1] = 0;
	TIM4Num[2] = 0;
}

/**
  * @brief  获取TIM4秒表计时时间
  * @param  
  * @retval 
  */
uint8_t *getTIM4Num(void) {
	return TIM4Num;
}

/**
  * @brief  TIM4中断服务函数，秒表计时
  * @param  无
  * @retval 无
  */
void TIM4_IRQHandler(void) {
	countTIM4++;
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		if(countTIM4 == 10) {
			countTIM4 = 0;
			TIM4Num[2]++;
			if(TIM4Num[2] == 100) {
				TIM4Num[2] = 0;
				TIM4Num[1] ++;
				if(TIM4Num[1] == 60) {
					TIM4Num[1] = 0;
					TIM4Num[0] ++;
				}
			}
		}
	}
}


/**
  * @brief  TIM3中断服务函数，定时进入息屏模式
  * @param  无
  * @retval 无
  */
void TIM3_IRQHandler(void) {
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		countTIM3++;
		if(countTIM3 == countTIM3Target) { // 定时达到设置的息屏时间
			countTIM3 = 0;
			breathFlag = 1;
			TIM_Cmd(TIM3, DISABLE);
		}
		
	}
}

/**
  * @brief  TIM2中断服务函数，用于定时闪烁显示和闹钟到时的蜂鸣器发声
  * @param  无
  * @retval 无
  */
void TIM2_IRQHandler(void) {
	static uint8_t count = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		// 闪烁显示，每500ms闪烁一次
		if (alarmConfigFlag != 0 ) {
			if(flashFlag == 0) {
				flashFlag = 1;
			}
			else flashFlag = 0;
			
		}
		
		// 闹钟时间到，则蜂鸣器每500ms响一次，响10次，持续10s后结束
		if(alarmFlag == 1){
			count ++;
			if(count == 20){
				count = 0;
				alarmFlag = 0;
				Beep_OFF();
				firstQuit = 0;	// 闹钟响完，再进入息屏模式
				TIM_Cmd(TIM2, DISABLE);
				
			}
			else Beep_Turn();
		}
		
 	}
}

