/**
 ****************************************************************************************************
 * @file        btim.c
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

#include "TIMER/btim.h"
#include "USART/usart.h"
#include "rtc/SecondWatch.h"
#include "rtc/myrtc.h"
#include "led/led.h"

TIM_HandleTypeDef g_tim2_handle;        /* ��ʱ��3��� */
TIM_HandleTypeDef g_tim3_handle;        /* ��ʱ��3��� */
TIM_HandleTypeDef g_tim4_handle;        /* ��ʱ��4��� */

/* ʱ����˸��ʾ��־λ */
uint8_t alterTimeDisplayFlag = 1;
/* ��������ģʽ��־λ */
uint8_t goSleepModeFlag = 0;
/* ����������¼�ж��ٸ�10ms�����ɼ�4,294,967,2.95s */
uint32_t msCount = 0;
/* TIM4SCount ��������ģʽǰ�ȴ�����, timingTaret Ŀ�궨ʱ���� Ĭ��15s */
uint16_t TIM4SCount = 0, timingTaret = 15;

/**
 * @brief       ������ʱ��TIM2��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim_tim2_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM2_INT_CLK_ENABLE();                                      /* ʹ��TIM2ʱ�� */
    
    g_tim2_handle.Instance = BTIM_TIM2_INT;                          /* ͨ�ö�ʱ��2 */
    g_tim2_handle.Init.Prescaler = psc;                              /* ��Ƶ */
    g_tim2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* ���ϼ����� */
    g_tim2_handle.Init.Period = arr;                                 /* �Զ�װ��ֵ */
    g_tim2_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_Base_Init(&g_tim2_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM2_INT_IRQn, 4, 0);  /* �����ж����ȼ�����ռ���ȼ�4�������ȼ�0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM2_INT_IRQn);          /* ����ITM2�ж� */
    
//    HAL_TIM_Base_Start_IT(&g_tim2_handle);           /* ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����ж� */
	BTIM_TIM2_DISABLE();
}

/**
 * @brief       ������ʱ��TIM3��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim_tim3_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM3_INT_CLK_ENABLE();                                      /* ʹ��TIM3ʱ�� */
    
    g_tim3_handle.Instance = BTIM_TIM3_INT;                          /* ͨ�ö�ʱ��3 */
    g_tim3_handle.Init.Prescaler = psc;                              /* ��Ƶ */
    g_tim3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* ���ϼ����� */
    g_tim3_handle.Init.Period = arr;                                 /* �Զ�װ��ֵ */
    g_tim3_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_Base_Init(&g_tim3_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM3_INT_IRQn, 4, 0);  /* �����ж����ȼ�����ռ���ȼ�4�������ȼ�0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM3_INT_IRQn);          /* ����ITM3�ж� */
    
	BTIM_TIM3_DISABLE();
//    HAL_TIM_Base_Start_IT(&g_tim3_handle);           /* ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����ж� */
	
}

/**
 * @brief       ������ʱ��TIM4��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *		
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim_tim4_int_init(uint16_t arr, uint16_t psc)
{
    BTIM_TIM4_INT_CLK_ENABLE();                                      /* ʹ��TIM4ʱ�� */
    
    g_tim4_handle.Instance = BTIM_TIM4_INT;                          /* ͨ�ö�ʱ��4 */
    g_tim4_handle.Init.Prescaler = psc;                              /* ��Ƶ */
    g_tim4_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* ���ϼ����� */
    g_tim4_handle.Init.Period = arr;                                 /* �Զ�װ��ֵ */
    g_tim4_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* ʱ�ӷ�Ƶ���� */
    HAL_TIM_Base_Init(&g_tim4_handle);
    
    HAL_NVIC_SetPriority(BTIM_TIM4_INT_IRQn, 6, 0); 		/* �����ж����ȼ�����ռ���ȼ�6�������ȼ�0 */
    HAL_NVIC_EnableIRQ(BTIM_TIM4_INT_IRQn);         		/* ����ITM4�ж� */
    
	if(rtcReadBKPDataRegister(RTC_BKP_DR6) == 0x00) {
		rtcWriteBKPDataRegister(RTC_BKP_DR6, timingTaret);
	}
	/* ÿ�θ�λ���Ѻ�ӱ��ݼĴ�����ȡ��ʱ����ʱ�䣬ȷ�����ѻ�λ��ʱ����ʱ��ı��� */
	timingTaret = rtcReadBKPDataRegister(RTC_BKP_DR6);		
		
	BTIM_TIM4_ENABLE();
//    HAL_TIM_Base_Start_IT(&g_tim4_handle);           		/* ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�����ж� */
}

/**
  * @brief  ��ȡ�޸�ʱ����ʾ��־λ
  * @param  ��
  * @retval �����޸�ʱ����ʾ��־λ
  */
uint8_t getAlterTimeDisplayFlag(void) {
	return alterTimeDisplayFlag;
}

/**
  * @brief  ��ȡ��������ģʽ��־λ
  * @param  ��
  * @retval ��������ģʽ��־λ��1��������ģʽ��0����������ģʽ
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
  * @brief  ���ö�ʱ���ߵ������ֵ
  * @param  s �����ֵ
  * @retval ��
  */
void setTIM4SCount(uint16_t s) {
	TIM4SCount = s;
}

/**
  * @brief  ����
  * @param  
  * @retval 
  */
void setTimingTaret(uint16_t s) {
	timingTaret = s;
	rtcWriteBKPDataRegister(RTC_BKP_DR6, s);
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIM2_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim2_handle);
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIM3_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim3_handle);
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIM4_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_tim4_handle);
}

/**
 * @brief       ��ʱ�������жϻص�����
* @param        htim:��ʱ�����ָ��
 * @retval      ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* tim2�жϣ�����ʱ����˸��ʾ */
	if (htim == (&g_tim2_handle))
    {
//		LED0_TOGGLE();
		if(alterTimeDisplayFlag == 1) alterTimeDisplayFlag = 2;		/* ��ת�޸�ʱ����ʾ��־λ */
		else alterTimeDisplayFlag = 1;
    }
	/* tim3�жϣ����������� */
	else if (htim == (&g_tim3_handle))
    {
		msCount++;					/* ����ֵ+1 */
    }
	/* tim4�жϣ����ڶ�ʱ���� */
    else if (htim == (&g_tim4_handle))
    {
		TIM4SCount++;				/* �����ֵ+1 */
		if(TIM4SCount == timingTaret) {		/* ����Ŀ�궨ʱʱ�䣬����λ��־λ */
			goSleepModeFlag = 1;
			BTIM_TIM4_DISABLE();	/* ʧ�ܶ�ʱ��4 */
		}
    }
}




