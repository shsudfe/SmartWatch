#include "encoder/encoder.h"
#include "led/led.h"
#include "oled/oled.h"


int8_t count = 0;
/* �޸�ʱ������ӼӼ���־λ��1Ϊ�ӣ�2Ϊ�� */
uint8_t alterTimeAddOrSub = 0;
/* ��ʾ�˵�ʱ��ͬ���˵����л�����forwardDir�ϻ���reversalDir�»� */
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
  * @brief  ��ȡʱ��/����ֵ�ӻ����־λalterTimeAddOrSub
  * @param  ��
  * @retval ����alterTimeAddOrSub
  */
uint8_t getAlterTimeAddOrSub(void) {
	uint8_t temp = alterTimeAddOrSub;
	alterTimeAddOrSub = 0;
	if(temp != 0) return temp;
	return 0;
}

/**
  * @brief  ��ȡ�˵����������־λdir
  * @param  ��
  * @retval ����dir
  */
encoderDirection getDir(void) {
	encoderDirection temp = noDir;
	if(dir != noDir){
		temp = dir;
		dir = noDir;
	}
	return temp;
}



