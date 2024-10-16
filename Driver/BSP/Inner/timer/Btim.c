/**
 ****************************************************************************************************
 * @file        btim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       基本定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200420
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "TIMER/btim.h"
#include "USART/usart.h"
#include "rtc/SecondWatch.h"
#include "rtc/myrtc.h"
#include "led/led.h"

TIM_HandleTypeDef g_tim2_handle;        /* 定时器3句柄 */
TIM_HandleTypeDef g_tim3_handle;        /* 定时器3句柄 */
TIM_HandleTypeDef g_tim4_handle;        /* 定时器4句柄 */

/* 时间闪烁显示标志位 */
uint8_t alterTimeDisplayFlag = 1;
/* 进入休眠模式标志位 */
uint8_t goSleepModeFlag = 0;
/* 秒表计数，记录有多少个10ms，最大可计4,294,967,2.95s */
uint32_t msCount = 0;
/* TIM4SCount 进入休眠模式前等待秒数, timingTaret 目标定时秒数 默认15s */
uint16_t TIM4SCount = 0, timingTaret = 15;

/**
 * @brief       基本定时器TIM2定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void btim_tim2_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM2_INT_CLK_ENABLE();                                      /* 使能TIM2时钟 */
    
    g_tim2_handle.Instance = BTIM_TIM2_INT;                          /* 通用定时器2 */
    g_tim2_handle.Init.Prescaler = psc;                              /* 分频 */
    g_tim2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* 向上计数器 */
    g_tim2_handle.Init.Period = arr;                                 /* 自动装载值 */
    g_tim2_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* 时钟分频因子 */
    HAL_TIM_Base_Init(&g_tim2_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM2_INT_IRQn, 4, 0);  /* 设置中断优先级，抢占优先级4，子优先级0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM2_INT_IRQn);          /* 开启ITM2中断 */
    
//    HAL_TIM_Base_Start_IT(&g_tim2_handle);           /* 使能定时器2和定时器2更新中断 */
	BTIM_TIM2_DISABLE();
}

/**
 * @brief       基本定时器TIM3定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void btim_tim3_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM3_INT_CLK_ENABLE();                                      /* 使能TIM3时钟 */
    
    g_tim3_handle.Instance = BTIM_TIM3_INT;                          /* 通用定时器3 */
    g_tim3_handle.Init.Prescaler = psc;                              /* 分频 */
    g_tim3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* 向上计数器 */
    g_tim3_handle.Init.Period = arr;                                 /* 自动装载值 */
    g_tim3_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* 时钟分频因子 */
    HAL_TIM_Base_Init(&g_tim3_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM3_INT_IRQn, 4, 0);  /* 设置中断优先级，抢占优先级4，子优先级0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM3_INT_IRQn);          /* 开启ITM3中断 */
    
	BTIM_TIM3_DISABLE();
//    HAL_TIM_Base_Start_IT(&g_tim3_handle);           /* 使能定时器3和定时器3更新中断 */
	
}

/**
 * @brief       基本定时器TIM4定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *		
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void btim_tim4_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM4_INT_CLK_ENABLE();                                      /* 使能TIM4时钟 */
    
    g_tim4_handle.Instance = BTIM_TIM4_INT;                          /* 通用定时器4 */
    g_tim4_handle.Init.Prescaler = psc;                              /* 分频 */
    g_tim4_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* 向上计数器 */
    g_tim4_handle.Init.Period = arr;                                 /* 自动装载值 */
    g_tim4_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* 时钟分频因子 */
    HAL_TIM_Base_Init(&g_tim4_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM4_INT_IRQn, 6, 0); 		/* 设置中断优先级，抢占优先级6，子优先级0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM4_INT_IRQn);         		/* 开启ITM4中断 */
    
	if(rtcReadBKPDataRegister(RTC_BKP_DR6) == 0x00) {
		rtcWriteBKPDataRegister(RTC_BKP_DR6, timingTaret);
	}
	/* 每次复位或唤醒后从备份寄存器读取定时休眠时间，确保唤醒或复位后定时休眠时间的保持 */
	timingTaret = rtcReadBKPDataRegister(RTC_BKP_DR6);		
		
	BTIM_TIM4_ENABLE();
//    HAL_TIM_Base_Start_IT(&g_tim4_handle);           		/* 使能定时器4和定时器4更新中断 */
}

/**
  * @brief  获取修改时间显示标志位
  * @param  无
  * @retval 返回修改时间显示标志位
  */
uint8_t getAlterTimeDisplayFlag(void) {
	return alterTimeDisplayFlag;
}

/**
  * @brief  获取进入休眠模式标志位
  * @param  无
  * @retval 返回休眠模式标志位，1进入休眠模式，0不进入休眠模式
  */
uint16_t getGoSleepModeFlag(void) {
	uint8_t temp = 0;
	if(goSleepModeFlag != 0) {
		temp = goSleepModeFlag;
		goSleepModeFlag = 0;
	}
	return temp;
}

/**
  * @brief  设置定时休眠的秒计数值
  * @param  s 秒计数值
  * @retval 无
  */
void setTIM4SCount(uint16_t s) {
	TIM4SCount = s;
}

/**
  * @brief  设置
  * @param  
  * @retval 
  */
void setTimingTaret(uint16_t s) {
	timingTaret = s;
	rtcWriteBKPDataRegister(RTC_BKP_DR6, s);
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIM2_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim2_handle);
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIM3_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim3_handle);
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIM4_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim4_handle);
}

/**
 * @brief       定时器更新中断回调函数
* @param        htim:定时器句柄指针
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* tim2中断，用于时间闪烁显示 */
	if (htim == (&g_tim2_handle))
    {
//		LED0_TOGGLE();
		if(alterTimeDisplayFlag == 1) alterTimeDisplayFlag = 2;		/* 翻转修改时间显示标志位 */
		else alterTimeDisplayFlag = 1;
    }
	/* tim3中断，用于秒表计数 */
	else if (htim == (&g_tim3_handle))
    {
		msCount++;					/* 毫秒值+1 */
    }
	/* tim4中断，用于定时休眠 */
    else if (htim == (&g_tim4_handle))
    {
		TIM4SCount++;				/* 秒计数值+1 */
		if(TIM4SCount == timingTaret) {		/* 到达目标定时时间，就置位标志位 */
			goSleepModeFlag = 1;
			BTIM_TIM4_DISABLE();	/* 失能定时器4 */
		}
    }
}




