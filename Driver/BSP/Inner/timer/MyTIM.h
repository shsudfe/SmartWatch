#ifndef __MYTIM_H
#define __MYTIM_H

void MyTIM2_Init(void);
void MyTIM3_Init(void);
void MyTIM4_Init(void);
void setCountTIM3Target(uint8_t* sleepTimeP, uint8_t* sleepTimeFlagP);
void resetTIM4Num(void);
uint8_t *getTIM4Num(void);

#endif


