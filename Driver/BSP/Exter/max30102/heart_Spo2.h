#ifndef __HEART_SPON2_H
#define __HEART_SPON2_H

#include "sys/sys.h"

#define BUFFER_LENGTH			500		/* 样本数据大小，设置为500个样本，首次测量需等待5s，后每1s更新 */

/* 心率血氧选择显示标志 */
typedef enum heart_spo2 {
	heartSelect,
	spo2Select,
	bothSelect,
	
}selectHeart_Spo2;

/* 进入或退出心率血氧模式标志 */
typedef enum goOrQuit {
	enterHS,
	quitHS,
	
}goOrQuitHS;

extern goOrQuitHS enterHSFlag;

extern uint32_t aun_ir_buffer[BUFFER_LENGTH]; 			/* IR LED   红外光数据，用于计算血氧 */
extern int32_t n_ir_buffer_length;    					/* 数据长度 */
extern uint32_t aun_red_buffer[BUFFER_LENGTH]; 			/* Red LED	红光数据，用于计算心率曲线以及计算心率 */
extern uint8_t dis_hr, dis_spo2;						/* 实际要显示的心率和血氧值 */

void heart_Spo2_Init(void);
void enterOrQuitHeartSpo2Mode(void);
void showHeartOrSpo2(selectHeart_Spo2 select);
void clearHeartAndSpo2(void);
void setOnceDis(void);


#endif


