#ifndef __BEEP_H__
#define __BEEP_H__

#define BEEP_PORT			GPIOB
#define BEEP_PIN			GPIO_PIN_12

void Beep_Init(void);
void Beep_ON(void);
void Beep_OFF(void);
void Beep_Turn(void);

#endif

