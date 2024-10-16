#ifndef __LED_H
#define __LED_H 

#define LED0_PORT 		GPIOA
#define LED0_PIN 		GPIO_PIN_1
#define LED1_PORT 		GPIOA
#define LED1_PIN 		GPIO_PIN_2
#define LED2_PORT 		GPIOA
#define LED2_PIN 		GPIO_PIN_3

void LED_Init(void);

void LED0_ON(void);
void LED1_ON(void);
void LED2_ON(void);

void LED0_OFF(void);
void LED1_OFF(void);
void LED2_OFF(void);

void LED0_TOGGLE(void);
void LED1_TOGGLE(void);
void LED2_TOGGLE(void);

#endif

