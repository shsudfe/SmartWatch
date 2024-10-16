#include "stm32f1xx.h"
#include "delay/delay.h"
#include "key/key.h"

/**
  * @brief  ������ʼ���������ѯ��ʽɨ�谴����
  * @param  ��
  * @retval ��
  */
void Key_Init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = KEY3_PIN;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(KEY3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = KEY1_PIN;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = KEY0_PIN;
	HAL_GPIO_Init(KEY0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = KEY2_PIN;
	HAL_GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = WKUP_KEY_PIN;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(WKUP_KEY_PORT, &GPIO_InitStructure);	
}

/**
  * @brief  �����ⲿ�жϳ�ʼ�����жϷ�ʽɨ�谴����
  * @param  ��
  * @retval ��
  */
void Key_EXTI_Init(void) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;		/* ����IOģʽΪ�ж������ش��� */
	GPIO_InitStructure.Pin = KEY1_PIN;					
	GPIO_InitStructure.Pull = GPIO_PULLUP;				/* ���� */
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStructure);		
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 4, 0);			// �����ж����ȼ�
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);					// ʹ���ж�
	
}

/**
  * @brief  ɨ�谴���������ѯ��ʽ��
  * @param  ��
  * @retval ���ذ��°����ļ�ֵ
  */
uint8_t getKeyValue(void) {
	uint8_t keyValue = 0;
	if(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == RESET) {
		delay_ms(20);
		while(HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == RESET);
		delay_ms(20);
		keyValue = 1;
	}else if(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == RESET) {
		delay_ms(20);
		while(HAL_GPIO_ReadPin(KEY0_PORT, KEY0_PIN) == RESET);
		delay_ms(20);
		keyValue = 2;
	}
	else if(HAL_GPIO_ReadPin(WKUP_KEY_PORT, WKUP_KEY_PIN) == SET) {
		delay_ms(20);
		while(HAL_GPIO_ReadPin(WKUP_KEY_PORT, WKUP_KEY_PIN) == SET);
		delay_ms(20);
		keyValue = 3;
	}
	else if(HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == RESET) {
		delay_ms(20);	
		while(HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == RESET);
		delay_ms(20);
		keyValue = 4;
	}else if(HAL_GPIO_ReadPin(KEY3_PORT, KEY3_PIN) == RESET) {
		delay_ms(20);	
		while(HAL_GPIO_ReadPin(KEY3_PORT, KEY3_PIN) == RESET);
		delay_ms(20);
		keyValue = 5;
	}
	return keyValue;
}

