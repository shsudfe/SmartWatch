#ifndef __KEY_H
#define __KEY_H

#include "sys/sys.h"

#define KEY0_PORT			GPIOB
#define KEY0_PIN			GPIO_PIN_0
#define KEY1_PORT			GPIOA
#define KEY1_PIN			GPIO_PIN_7
#define KEY2_PORT			GPIOB
#define KEY2_PIN			GPIO_PIN_1
#define KEY3_PORT			GPIOA
#define KEY3_PIN			GPIO_PIN_5
#define WKUP_KEY_PORT		GPIOA
#define WKUP_KEY_PIN		GPIO_PIN_0

#define KEY0_VAL			2
#define KEY1_VAL			1
#define KEY2_VAL			4
#define KEY3_VAL			5
#define WKUP_KEY_VAL		3


void Key_Init(void);
void Key_EXTI_Init(void);
uint8_t getKeyValue(void);


#endif


