#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys/sys.h"
#include "menu/menu.h"

#define ENCODER_PORT			GPIOB
#define ENCODER_PINA			GPIO_PIN_11
#define ENCODER_PINB			GPIO_PIN_10

extern int8_t count;
extern uint8_t alterTimeAddOrSub;
extern encoderDirection dir;

void EncoderSensor_Init(void);

uint8_t getAlterTimeAddOrSub(void);
encoderDirection getDir(void);

#endif


