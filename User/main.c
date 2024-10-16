#include "led/led.h"
#include "sys/sys.h"
#include "usart/usart.h"
#include "delay/delay.h"
#include "oled/oled.h"
#include "encoder/encoder.h"
#include "rtc/myrtc.h"
#include "beep/beep.h"
#include "timer/btim.h"
#include "key/key.h"
#include "rtc/MyAlarm.h"
#include "menu/menu.h"
#include "rtc/SecondWatch.h"
#include "sleep/timingSleep.h"
#include "exti/gpioexti.h"
#include "max30102/heart_Spo2.h"
#include "oled/onceDis.h"

uint8_t keyValue;

void keyScanDoFun(void);

int main(void) {
	
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M  */
    delay_init(72);                             /* 初始化延时函数 */
	LED_Init();									/* LED初始化 */
	OLED_Init();								/* OLED显示屏初始化 */
	EncoderSensor_Init();						/* 旋转编码器初始化 */
	MyRTC_Init();								/* RTC初始化 */
	Beep_Init();								/* 蜂鸣器初始化 */
	Key_Init();									/* 按键初始化 */
	Key_EXTI_Init();							/* 按键外部中断初始化 */
	usart_init(115200);							/* 串口初始化 */
	heart_Spo2_Init();							/* max30102心率血氧模块初始化 */
	
	btim_tim4_int_init(10000 - 1, 7200 - 1);	/* 初始化定时器4,移除时间为1s,用做定时休眠 */
	btim_tim3_int_init(100 - 1, 7200 - 1);		/* 初始化定时器3,溢出时间为10ms,用作秒表计时 */
	btim_tim2_int_init(5000 - 1, 7200 - 1);		/* 初始化定时器2,溢出时间为500ms,用作时间闪烁显示 */
	
	while(1) {
		keyScanDoFun();									/* 按键扫描执行指定功能函数 */
		
		/* 主界面，显示标准时间 */
		if(menuFlag == menuBlank) {						
       		showTimeAndDate();
		/* 只需显示菜单项功能界面：修改闹钟和修改时间 */
		}else if(menuFlag == menuFun) {					/* 显示功能界面 */
			if(funShowFlag == alterTimeShow){			
				showAlterTime();						/* 显示修改时间界面 */
			}else if(funShowFlag == alterAlarmShow){	
				showAlterAlarm();						/* 显示修改闹钟界面 */
			}
			
		/* 只显示菜单或同时需显示菜单及功能界面 */
		}else if(menuFlag == menuShow){					
			if(funShowFlag == secondWatchShow) {
				showSecondWatch();						/* 显示秒表功能界面 */
			}else if(funShowFlag == timingSleepShow) {
				showTimingSleep();						/* 显示设置休眠时间功能界面 */
			}else if(funShowFlag == heartShow) {
				showHeartOrSpo2(heartSelect);			/* 显示心率功能界面 */
			}else if(funShowFlag == spo2Show) {
				showHeartOrSpo2(spo2Select);			/* 显示血氧功能界面 */
			}
			else{										
//				if(RTCShowTimeFlag == 1) 				/* 每次只显示菜单时，先重置RTCShowTimeFlag */
//					RTCShowTimeFlag = 0;
				if(OnceDisFlag == unDis)				/* 每次只显示菜单时，先重置OnceDisFlag */
					OnceDisFlag = dis;
			}
			
			/* 同级菜单显示,获取菜单滑动方向 */
			encoderDirection dir = getDir();			
			if(dir != noDir) showMenu(selfMenu, dir);	/* 发生菜单滑动,则更新显示同级菜单 */
		}
	}
}

/**
  * @brief  按键扫描执行指定功能函数
  * @param  无
  * @retval 无
  */
void keyScanDoFun(void) {
	keyValue = getKeyValue();
	
	/* 按键1按下，只有首次按下才初始化菜单，执行箭头指向菜单项的功能函数*/
//	if(keyValue == KEY1_VAL) {	
	/* 获取按键1的外部中断标志，被中断才进入分支 */
	if(getKeyITFlag()) {								
		if(menuFlag == menuBlank && arrowPoint == NULL) 
			Menu_Init();								
		arrowPoint->currentMItem->funP();				/* 执行箭头指针指向菜单项的功能函数 */
		
	/* 按键2按下，且处于修改标准时间或闹钟功能时，才切换修改位 */
	}else if(keyValue == KEY2_VAL) {
		if(funShowFlag == alterTimeShow) toggleAlterTimeBit();
		else if(funShowFlag == alterAlarmShow) toggleAlterAlarmBit();
		else if(funShowFlag == timingSleepShow) {
			timingIndex++;
			if(timingIndex > 5) timingIndex = 0;
		}
	
	/* 按键3按下，手动进入休眠模式 */
	}else if(keyValue == KEY3_VAL) {
		goSleepMode();
	}
	
}



