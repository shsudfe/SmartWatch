#include "exti/GPIOExti.h"
#include "encoder/encoder.h"
#include "key/key.h"
#include "led/led.h"
#include "delay/delay.h"

uint8_t key1_IT_Flag = 0;		/* 按键1中断标志 */

/**
  * @brief  获取按键中断标志
  * @param  无
  * @retval 返回按键中断标志 1为触发中断，0则未触发中断
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
  * @brief  外部中断5-9中断服务函数
  * @param  无
  * @retval 无
  */
void EXTI9_5_IRQHandler(void) {
	
	HAL_GPIO_EXTI_IRQHandler(KEY1_PIN);		/* 调用GPIO外部中断请求函数，中断请求函数中会调用回调函数 */	
	__HAL_GPIO_EXTI_CLEAR_IT(KEY1_PIN);		/* 清除外部中断标志位 */

}

/**
  * @brief  外部中断10-15中断服务函数
  * @param  无
  * @retval 无
  */
void EXTI15_10_IRQHandler(void) {
	
	HAL_GPIO_EXTI_IRQHandler(ENCODER_PINA);
	__HAL_GPIO_EXTI_CLEAR_IT(ENCODER_PINA);
	HAL_GPIO_EXTI_IRQHandler(ENCODER_PINB);
	__HAL_GPIO_EXTI_CLEAR_IT(ENCODER_PINB);
}

/**
  * @brief  外部中断回调函数
  * @param  GPIO_Pin 外部中断线
  * @retval 无
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* encoder编码器外部中断处理 */
	if(GPIO_Pin == ENCODER_PINA) {
		if(HAL_GPIO_ReadPin(ENCODER_PORT, ENCODER_PINB) == GPIO_PIN_SET) {	/* 正转 */
			count ++;
			/* 当显示功能为修改标准时间或闹钟时，置标志位为1，做加法 */
			if(funShowFlag == alterTimeShow || funShowFlag == alterAlarmShow) 
				alterTimeAddOrSub = 1;
			
			/* 当显示菜单时，更新菜单滑动方向为上滑 */
			if(menuFlag == menuShow) {
				dir = forwardDir;
			}
		}
	}else if(GPIO_Pin == ENCODER_PINB) {
		if(HAL_GPIO_ReadPin(ENCODER_PORT, ENCODER_PINA) == GPIO_PIN_SET) {	/* 反转 */
			count --;
			/* 当显示功能为修改标准时间或闹钟时，置标志位为2，做减法 */
			if(funShowFlag == alterTimeShow || funShowFlag == alterAlarmShow) 
				alterTimeAddOrSub = 2;
			
			/* 当显示菜单时，更新菜单滑动方向为下滑 */
			if(menuFlag == menuShow) {
				dir = reversalDir;
			} 
		}
	/* KEY1外部中断处理 */
	}else if(GPIO_Pin == KEY1_PIN) {
		/* 按键触发外部中断，延时10ms消抖 */
		delay_ms(20);					
		key1_IT_Flag = 1;
		
//		LED1_TOGGLE();

	}
}







