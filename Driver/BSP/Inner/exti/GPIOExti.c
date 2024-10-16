#include "exti/GPIOExti.h"
#include "encoder/encoder.h"
#include "key/key.h"
#include "led/led.h"
#include "delay/delay.h"

uint8_t key1_IT_Flag = 0;		/* ����1�жϱ�־ */

/**
  * @brief  ��ȡ�����жϱ�־
  * @param  ��
  * @retval ���ذ����жϱ�־ 1Ϊ�����жϣ�0��δ�����ж�
  */
uint8_t getKeyITFlag(void) {
	uint8_t temp = 0;
	if(key1_IT_Flag == 1) {
		temp = key1_IT_Flag;
		key1_IT_Flag = 0;
	}
	return temp;
}

/**
  * @brief  �ⲿ�ж�5-9�жϷ�����
  * @param  ��
  * @retval ��
  */
void EXTI9_5_IRQHandler(void) {
	
	HAL_GPIO_EXTI_IRQHandler(KEY1_PIN);		/* ����GPIO�ⲿ�ж����������ж��������л���ûص����� */	
	__HAL_GPIO_EXTI_CLEAR_IT(KEY1_PIN);		/* ����ⲿ�жϱ�־λ */

}

/**
  * @brief  �ⲿ�ж�10-15�жϷ�����
  * @param  ��
  * @retval ��
  */
void EXTI15_10_IRQHandler(void) {
	
	HAL_GPIO_EXTI_IRQHandler(ENCODER_PINA);
	__HAL_GPIO_EXTI_CLEAR_IT(ENCODER_PINA);
	HAL_GPIO_EXTI_IRQHandler(ENCODER_PINB);
	__HAL_GPIO_EXTI_CLEAR_IT(ENCODER_PINB);
}

/**
  * @brief  �ⲿ�жϻص�����
  * @param  GPIO_Pin �ⲿ�ж���
  * @retval ��
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* encoder�������ⲿ�жϴ��� */
	if(GPIO_Pin == ENCODER_PINA) {
		if(HAL_GPIO_ReadPin(ENCODER_PORT, ENCODER_PINB) == GPIO_PIN_SET) {	/* ��ת */
			count ++;
			/* ����ʾ����Ϊ�޸ı�׼ʱ�������ʱ���ñ�־λΪ1�����ӷ� */
			if(funShowFlag == alterTimeShow || funShowFlag == alterAlarmShow) 
				alterTimeAddOrSub = 1;
			
			/* ����ʾ�˵�ʱ�����²˵���������Ϊ�ϻ� */
			if(menuFlag == menuShow) {
				dir = forwardDir;
			}
		}
	}else if(GPIO_Pin == ENCODER_PINB) {
		if(HAL_GPIO_ReadPin(ENCODER_PORT, ENCODER_PINA) == GPIO_PIN_SET) {	/* ��ת */
			count --;
			/* ����ʾ����Ϊ�޸ı�׼ʱ�������ʱ���ñ�־λΪ2�������� */
			if(funShowFlag == alterTimeShow || funShowFlag == alterAlarmShow) 
				alterTimeAddOrSub = 2;
			
			/* ����ʾ�˵�ʱ�����²˵���������Ϊ�»� */
			if(menuFlag == menuShow) {
				dir = reversalDir;
			} 
		}
	/* KEY1�ⲿ�жϴ��� */
	}else if(GPIO_Pin == KEY1_PIN) {
		/* ���������ⲿ�жϣ���ʱ10ms���� */
		delay_ms(20);					
		key1_IT_Flag = 1;
		
//		LED1_TOGGLE();

	}
}







