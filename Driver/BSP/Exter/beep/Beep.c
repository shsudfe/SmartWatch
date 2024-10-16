#include "beep/Beep.h"
#include "sys/sys.h"

void Beep_ON(void) {
//	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
	HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_RESET);
}

void Beep_OFF(void) {
//	GPIO_SetBits(BEEP_PORT, BEEP_PIN);
	HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_SET);
}

void Beep_Turn(void) {
//	if(GPIO_ReadOutputDataBit(BEEP_PORT, BEEP_PIN) == SET) {
//		Beep_ON();
//	}else if(GPIO_ReadOutputDataBit(BEEP_PORT, BEEP_PIN) == RESET) {
//		Beep_OFF();
//	}
	
	if(HAL_GPIO_ReadPin(BEEP_PORT, BEEP_PIN) == GPIO_PIN_SET) {
		Beep_ON();
	}else if(HAL_GPIO_ReadPin(BEEP_PORT, BEEP_PIN) == GPIO_PIN_RESET) {
		Beep_OFF();
	}
}

void Beep_Init(void) {
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin = BEEP_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	
	Beep_OFF();

}


