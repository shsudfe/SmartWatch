#include "stm32f1xx.h"
#include "delay/delay.h"
#include "key/key.h"

/**
  * @brief  按键初始化（程序查询方式扫描按键）
  * @param  无
  * @retval 无
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
  * @brief  按键外部中断初始化（中断方式扫描按键）
  * @param  无
  * @retval 无
  */
void Key_EXTI_Init(void) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;		/* 设置IO模式为中断上升沿触发 */
	GPIO_InitStructure.Pin = KEY1_PIN;					
	GPIO_InitStructure.Pull = GPIO_PULLUP;				/* 上拉 */
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStructure);		
	
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 4, 0);			// 设置中断优先级
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);					// 使能中断
	
}

/**
  * @brief  扫描按键（程序查询方式）
  * @param  无
  * @retval 返回按下按键的键值
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

