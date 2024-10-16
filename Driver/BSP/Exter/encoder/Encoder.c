#include "encoder/encoder.h"
#include "led/led.h"
#include "oled/oled.h"


int8_t count = 0;
/* 修改时间或闹钟加减标志位，1为加，2为减 */
uint8_t alterTimeAddOrSub = 0;
/* 显示菜单时，同级菜单项切换方向，forwardDir上滑，reversalDir下滑 */
encoderDirection dir = noDir;

void EncoderSensor_Init(void) {
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pin = ENCODER_PINA | ENCODER_PINB;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);
	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
}

/**
  * @brief  获取时间/闹钟值加或减标志位alterTimeAddOrSub
  * @param  无
  * @retval 返回alterTimeAddOrSub
  */
uint8_t getAlterTimeAddOrSub(void) {
	uint8_t temp = alterTimeAddOrSub;
	alterTimeAddOrSub = 0;
	if(temp != 0) return temp;
	return 0;
}

/**
  * @brief  获取菜单滑动方向标志位dir
  * @param  无
  * @retval 返回dir
  */
encoderDirection getDir(void) {
	encoderDirection temp = noDir;
	if(dir != noDir){
		temp = dir;
		dir = noDir;
	}
	return temp;
}



