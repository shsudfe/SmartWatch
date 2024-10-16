#include "max30102/heart_Spo2.h"
#include "oled/oled.h"
#include "max30102/max30102.h"
#include "oled/OnceDis.h"

goOrQuitHS enterHSFlag = quitHS;				/* 进入或退出心率血氧模式标志位 */
uint32_t aun_ir_buffer[BUFFER_LENGTH] = {0};	/* 红外光样本数据 */
uint32_t aun_red_buffer[BUFFER_LENGTH] = {0};	/* 红光样本数据 */
int32_t n_ir_buffer_length = BUFFER_LENGTH;		/* 样本数量 */
uint8_t dis_hr = 0,dis_spo2 = 0;				/* 心率显示值，血氧显示值 */

uint8_t firstGetHeartAndSpo2 = 0;		/* 首次获取心率血氧标志位 0为首次，1则相反*/

void heart_Spo2_Init(void) {
	MAX30102_Init();
}

/**
  * @brief  进入或退出心率血氧模式
  * @param  无
  * @retval 无
  */
void enterOrQuitHeartSpo2Mode(void){
	/* 若当前在心率血氧模式则退出 */
	if(enterHSFlag == enterHS) {
		enterHSFlag = quitHS;		/* 置标志位为退出 */
		firstGetHeartAndSpo2 = 0;	/* 重置首次进入标志 */
		clearHeartAndSpo2();		/* 清除样本数据和心率血氧显示值 */
		OnceDisFlag = dis;			/* 重置首次显示标志 */
	/* 反之则进入 */
	}else {
		enterHSFlag = enterHS;		/* 置标志位为进入 */
	}
	
}

/**
  * @brief  获取心率和血氧并显示心率或血氧
  * @param  select 选择显示位 
				heartSelect 显示心跳
				spo2Select	显示血氧
				bothSelect 	二者都显示
  * @retval 无
  */
void showHeartOrSpo2(selectHeart_Spo2 select){
	
	/* 只显示“心率：” */
	if(select == heartSelect) {
		onceDis_Heart();
		OLED_ShowNum(2, 9, dis_hr, 3);				/* 默认显示0 */
	}
	/* 只显示“血氧：” */
	else if(select == spo2Select) {
		onceDis_Spo2();
		OLED_ShowNum(2, 9, dis_spo2, 2);			/* 默认显示0 */
	}
	/* 心率血氧都显示 */
	else if(select == bothSelect) {
		onceDis_HeartSpo2();
		OLED_ShowNum(2, 9, dis_hr, 3);				/* 默认显示0 */
		OLED_ShowNum(4, 9, dis_spo2, 2);			/* 默认显示0 */
	}
	
	/* 若当前是进入心率血氧模式，则获取心率血氧值 */
	if(enterHSFlag == enterHS) {
		// 获取心率血氧值
		getHeartAndSpo2(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &dis_spo2, &dis_hr, &firstGetHeartAndSpo2);
	}
}

void clearHeartAndSpo2(void) {
	uint16_t i = 0;
	for(; i < BUFFER_LENGTH; i ++) {
		aun_ir_buffer[i] = 0;
		aun_red_buffer[i] = 0;
	}
	dis_hr = 0;
	dis_spo2 = 0;
}

void setOnceDis(void) {
	OnceDisFlag = dis;
}

