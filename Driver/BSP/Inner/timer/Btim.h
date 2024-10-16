/**
 ****************************************************************************************************
 * @file        btim.h
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

#ifndef __BTIM_H
#define __BTIM_H

#include "sys/sys.h"

/******************************************************************************************/
/* 基本定时器 定义 */

extern TIM_HandleTypeDef g_tim2_handle;        /* 定时器3句柄 */
extern TIM_HandleTypeDef g_tim3_handle;        /* 定时器3句柄 */
extern TIM_HandleTypeDef g_tim4_handle;        /* 定时器4句柄 */

extern uint32_t msCount;					   /* 秒表计数，记录多少个10ms */

/* TIMX 中断定义 
 * 默认是针对TIM2~TIM5, TIM12~TIM17.
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM17任意一个定时器.
 */
 
#define BTIM_TIM2_INT                       TIM2
#define BTIM_TIM2_INT_IRQn                  TIM2_IRQn
#define BTIM_TIM2_INT_IRQHandler            TIM2_IRQHandler
#define BTIM_TIM2_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)  /* TIM2 时钟使能 */ 
#define BTIM_TIM2_ENABLE()					do{ HAL_TIM_Base_Start_IT(&g_tim2_handle);}while(0)
#define BTIM_TIM2_DISABLE()					do{ HAL_TIM_Base_Stop_IT(&g_tim2_handle);}while(0)

#define BTIM_TIM3_INT                       TIM3
#define BTIM_TIM3_INT_IRQn                  TIM3_IRQn
#define BTIM_TIM3_INT_IRQHandler            TIM3_IRQHandler
#define BTIM_TIM3_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */
#define BTIM_TIM3_ENABLE()					do{ HAL_TIM_Base_Start_IT(&g_tim3_handle);}while(0)
#define BTIM_TIM3_DISABLE()					do{ HAL_TIM_Base_Stop_IT(&g_tim3_handle);}while(0)

#define BTIM_TIM4_INT                       TIM4
#define BTIM_TIM4_INT_IRQn                  TIM4_IRQn
#define BTIM_TIM4_INT_IRQHandler            TIM4_IRQHandler
#define BTIM_TIM4_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM4_CLK_ENABLE(); }while(0)  /* TIM4 时钟使能 */
#define BTIM_TIM4_ENABLE()					do{ HAL_TIM_Base_Start_IT(&g_tim4_handle);}while(0)
#define BTIM_TIM4_DISABLE()					do{ HAL_TIM_Base_Stop_IT(&g_tim4_handle);}while(0)

/******************************************************************************************/

void btim_tim2_int_init(uint16_t arr, uint16_t psc);
void btim_tim3_int_init(uint16_t arr, uint16_t psc);
void btim_tim4_int_init(uint16_t arr, uint16_t psc);

uint8_t getAlterTimeDisplayFlag(void);
uint16_t getGoSleepModeFlag(void);
void setTIM4SCount(uint16_t s);
void setTimingTaret(uint16_t s);

#endif

















