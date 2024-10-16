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
// TIM3��ʱ������־��TIM3��ʱĿ�����ֵ
uint16_t countTIM3 = 0, countTIM3Target = 15;
// TIM4�жϼ�����־
uint8_t countTIM4 = 0;
// TIM4����ʱ��ʱ����
uint8_t TIM4Num[3] = {0, 0, 0};

/**
  * @brief  TIM2��ʼ��
  * @param  ��
  * @retval ��
  */
void MyTIM2_Init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;		// ��װ��ֵ5000����5000������0.5s����
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // ʱ������100us
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
  * @brief  TIM3��ʼ��
  * @param  ��
  * @retval ��
  */
void MyTIM3_Init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;		// ��װ��ֵ10000, ���ʱ��Ϊ1s
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // ʱ������100us
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
  * @brief  TIM4��ʼ��
  * @param  ��
  * @retval ��
  */
void MyTIM4_Init(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;		// ��װ��ֵ1000, ���ʱ��Ϊ1ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // ʱ������1us
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
  * @brief  ����TIM3��ʱ����Ŀ��
  * @param  sleepTimeP ����ʱ�䵲λ����
  * @param  sleepTimeFlagP ����ʱ�䵵λ+��λ��־λ����
  * @retval ��
  */
void setCountTIM3Target(uint8_t* sleepTimeP, uint8_t* sleepTimeFlagP) {
	countTIM3 = 0;						// ����֮ǰ��������countTIM3����ֵ��������ܻ���ֵȴ��ǳ��ò����ߵ�bug
	if(sleepTimeFlagP[1] == 0) {		// �뼶
		countTIM3Target = sleepTimeP[sleepTimeFlagP[0]];	
	}else if(sleepTimeFlagP[1] == 1){	// ���Ӽ�
		countTIM3Target = sleepTimeP[sleepTimeFlagP[0]] * 60;
	}
}

/**
  * @brief  ����TIM4���ʱ��
  * @param  ��
  * @retval ��
  */
void resetTIM4Num(void) {
	TIM4Num[0] = 0;
	TIM4Num[1] = 0;
	TIM4Num[2] = 0;
}

/**
  * @brief  ��ȡTIM4����ʱʱ��
  * @param  
  * @retval 
  */
uint8_t *getTIM4Num(void) {
	return TIM4Num;
}

/**
  * @brief  TIM4�жϷ�����������ʱ
  * @param  ��
  * @retval ��
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
  * @brief  TIM3�жϷ���������ʱ����Ϣ��ģʽ
  * @param  ��
  * @retval ��
  */
void TIM3_IRQHandler(void) {
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		countTIM3++;
		if(countTIM3 == countTIM3Target) { // ��ʱ�ﵽ���õ�Ϣ��ʱ��
			countTIM3 = 0;
			breathFlag = 1;
			TIM_Cmd(TIM3, DISABLE);
		}
		
	}
}

/**
  * @brief  TIM2�жϷ����������ڶ�ʱ��˸��ʾ�����ӵ�ʱ�ķ���������
  * @param  ��
  * @retval ��
  */
void TIM2_IRQHandler(void) {
	static uint8_t count = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		// ��˸��ʾ��ÿ500ms��˸һ��
		if (alarmConfigFlag != 0 ) {
			if(flashFlag == 0) {
				flashFlag = 1;
			}
			else flashFlag = 0;
			
		}
		
		// ����ʱ�䵽���������ÿ500ms��һ�Σ���10�Σ�����10s�����
		if(alarmFlag == 1){
			count ++;
			if(count == 20){
				count = 0;
				alarmFlag = 0;
				Beep_OFF();
				firstQuit = 0;	// �������꣬�ٽ���Ϣ��ģʽ
				TIM_Cmd(TIM2, DISABLE);
				
			}
			else Beep_Turn();
		}
		
 	}
}

