/**
 ****************************************************************************************************
 * @file        btim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       ������ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200420
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __BTIM_H
#define __BTIM_H

#include "sys/sys.h"

/******************************************************************************************/
/* ������ʱ�� ���� */

extern TIM_HandleTypeDef g_tim2_handle;        /* ��ʱ��3��� */
extern TIM_HandleTypeDef g_tim3_handle;        /* ��ʱ��3��� */
extern TIM_HandleTypeDef g_tim4_handle;        /* ��ʱ��4��� */

extern uint32_t msCount;					   /* ����������¼���ٸ�10ms */

/* TIMX �ж϶��� 
 * Ĭ�������TIM2~TIM5, TIM12~TIM17.
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM17����һ����ʱ��.
 */
 
#define BTIM_TIM2_INT                       TIM2
#define BTIM_TIM2_INT_IRQn                  TIM2_IRQn
#define BTIM_TIM2_INT_IRQHandler            TIM2_IRQHandler
#define BTIM_TIM2_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)  /* TIM2 ʱ��ʹ�� */ 
#define BTIM_TIM2_ENABLE()					do{ HAL_TIM_Base_Start_IT(&g_tim2_handle);}while(0)
#define BTIM_TIM2_DISABLE()					do{ HAL_TIM_Base_Stop_IT(&g_tim2_handle);}while(0)

#define BTIM_TIM3_INT                       TIM3
#define BTIM_TIM3_INT_IRQn                  TIM3_IRQn
#define BTIM_TIM3_INT_IRQHandler            TIM3_IRQHandler
#define BTIM_TIM3_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 ʱ��ʹ�� */
#define BTIM_TIM3_ENABLE()					do{ HAL_TIM_Base_Start_IT(&g_tim3_handle);}while(0)
#define BTIM_TIM3_DISABLE()					do{ HAL_TIM_Base_Stop_IT(&g_tim3_handle);}while(0)

#define BTIM_TIM4_INT                       TIM4
#define BTIM_TIM4_INT_IRQn                  TIM4_IRQn
#define BTIM_TIM4_INT_IRQHandler            TIM4_IRQHandler
#define BTIM_TIM4_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM4_CLK_ENABLE(); }while(0)  /* TIM4 ʱ��ʹ�� */
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

















