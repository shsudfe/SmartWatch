#include "stm32f1xx.h"
#include "led/led.h"

void LED_Init(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin = LED0_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(LED0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LED1_PIN;
	HAL_GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LED2_PIN;
	HAL_GPIO_Init(LED1_PORT, &GPIO_InitStructure);

	LED0_OFF();
	LED1_OFF();
	LED2_OFF();
}

void LED0_ON(void) {
	HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_RESET);
}

void LED1_ON(void) {
	HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
}

void LED2_ON(void) {
	HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
}

void LED0_OFF(void) {
	HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_SET);
}

void LED1_OFF(void) {
	HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
}

void LED2_OFF(void) {
	HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
}

void LED0_TOGGLE(void) {
	if(HAL_GPIO_ReadPin(LED0_PORT, LED0_PIN) == GPIO_PIN_RESET) {
		LED0_OFF();
	}else {
		LED0_ON();
	}
}

void LED1_TOGGLE(void) {
	if(HAL_GPIO_ReadPin(LED1_PORT, LED1_PIN) == GPIO_PIN_RESET) {
		LED1_OFF();
	}else {
		LED1_ON();
	}
}

void LED2_TOGGLE(void) {
	if(HAL_GPIO_ReadPin(LED2_PORT, LED2_PIN) == GPIO_PIN_RESET) {
		LED2_OFF();
	}else {
		LED2_ON();
	}
}




