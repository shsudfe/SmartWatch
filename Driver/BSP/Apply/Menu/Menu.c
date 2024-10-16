#include <stdlib.h>
#include <string.h>
#include "menu/menu.h"
#include "oled/oled.h"
#include "rtc/MyRTC.h"
#include "rtc/MyAlarm.h"
#include "rtc/secondWatch.h"
#include "sleep/timingSleep.h"
#include "max30102/heart_spo2.h"


arrowP arrowPoint = NULL;				/* 全局变量箭头指针 */
showMenuFlag menuFlag = menuBlank;		/* 菜单显示标志，默认不显示菜单 */
menuFunShow funShowFlag = noneShow;		/* 功能显示标志，默认不显示功能 */


/**
  * @brief  菜单初始化，包括首菜单项、各个菜单项、箭头指针的初始化
  * @param  无
  * @retval 无
  */
void Menu_Init(void){
	uint8_t i;
	menuItemP menuTop = NULL;
	
	/* 首菜单项 */
	menuTop = createMenuItem("首选项", NULL, 0, menuTopFun);

	/* 一级菜单项 */
	menuTop->subItems[0] = createMenuItem("返回", menuTop, 0, quitMenuFun);
	menuTop->subItems[1] = createMenuItem("设置", menuTop, 1, setFun);
	menuTop->subItems[2] = createMenuItem("实用工具", menuTop, 2, toolsFun);
	menuTop->subItems[3] = createMenuItem("运动健康", menuTop, 3, healthFun);
	menuTop->subItems[4] = createMenuItem("其他菜单", menuTop, 4, NULLFun);

	
	/* 二级菜单项-->设置 */
	menuTop->subItems[1]->subItems[0] = createMenuItem("返回", menuTop->subItems[1], 0, backFun);
	menuTop->subItems[1]->subItems[1] = createMenuItem("息屏设置", menuTop->subItems[1], 1, sleepSetFun);
	menuTop->subItems[1]->subItems[2] = createMenuItem("时间设置", menuTop->subItems[1], 2, timeSetFun);
	menuTop->subItems[1]->subItems[3] = createMenuItem("闹钟设置", menuTop->subItems[1], 3, alarmSetFun);

	/* 二级菜单项-->实用工具 */
	menuTop->subItems[2]->subItems[0] = createMenuItem("返回", menuTop->subItems[2], 0, backFun);
	menuTop->subItems[2]->subItems[1] = createMenuItem("秒表", menuTop->subItems[2], 1, secondWatchFun);
	menuTop->subItems[2]->subItems[2] = createMenuItem("手电筒", menuTop->subItems[2], 2, flashLightFun);
	
	/* 二级菜单项-->运动健康 */
	menuTop->subItems[3]->subItems[0] = createMenuItem("返回", menuTop->subItems[3], 0, backFun);
	menuTop->subItems[3]->subItems[1] = createMenuItem("体温测量", menuTop->subItems[3], 1, tempFun);
	menuTop->subItems[3]->subItems[2] = createMenuItem("心率测量", menuTop->subItems[3], 2, heartFun);
	menuTop->subItems[3]->subItems[3] = createMenuItem("血氧测量", menuTop->subItems[3], 3, bloodOxygenFun);

	
	/* 三级菜单项-->息屏设置 */
	menuTop->subItems[1]->subItems[1]->subItems[0] = createMenuItem("返回", menuTop->subItems[1]->subItems[1], 0, sleepBackFun);
	menuTop->subItems[1]->subItems[1]->subItems[1] = createMenuItem("确定", menuTop->subItems[1]->subItems[1], 1, sleepEnterFun);

	/* 三级菜单项-->秒表 */
	menuTop->subItems[2]->subItems[1]->subItems[0] = createMenuItem("返回", menuTop->subItems[2]->subItems[1], 0, secondWatchBackFun);
	menuTop->subItems[2]->subItems[1]->subItems[1] = createMenuItem("开始", menuTop->subItems[2]->subItems[1], 1, secondWatchBeginOrSusFun);
	menuTop->subItems[2]->subItems[1]->subItems[2] = createMenuItem("重置", menuTop->subItems[2]->subItems[1], 2, secondWatchReset);

	/* 三级菜单项-->体温测量*/
	menuTop->subItems[3]->subItems[1]->subItems[0] = createMenuItem("返回", menuTop->subItems[3]->subItems[1], 0, backFun);
	menuTop->subItems[3]->subItems[1]->subItems[1] = createMenuItem("确定", menuTop->subItems[3]->subItems[1], 1, NULLFun);

	/* 三级菜单项-->心率测量*/
	menuTop->subItems[3]->subItems[2]->subItems[0] = createMenuItem("返回", menuTop->subItems[3]->subItems[2], 0, heartBackFun);
	menuTop->subItems[3]->subItems[2]->subItems[1] = createMenuItem("开始", menuTop->subItems[3]->subItems[2], 1, heartSpo2BeginOrSusFun);
	
	/* 三级菜单项-->血氧测量*/
	menuTop->subItems[3]->subItems[3]->subItems[0] = createMenuItem("返回", menuTop->subItems[3]->subItems[3], 0, bloodOxygenBackFun);
	menuTop->subItems[3]->subItems[3]->subItems[1] = createMenuItem("开始", menuTop->subItems[3]->subItems[3], 1, heartSpo2BeginOrSusFun);

	/* 分配内存给箭头指针 */
	arrowPoint = malloc(sizeof(arrow));
	
	/* 箭头指针初始化 */
	arrowPoint->currentMItem = menuTop;			/* 初始化箭头指针指向首菜单项 */
	arrowPoint->currentMenuLevel = 0;			/* 初始化菜单等级为0 */
	arrowPoint->currentLine = 1;				/* 初始化箭头指针所在行为1 */
	for (i = 0; i < MAX_LEVEL - 1; i++){
		arrowPoint->supersItemLine[i] = 0;		/* 初始化箭头指针指向菜单项的父菜单项所在行为0 */
	}  
}

/**
  * @brief  动态分配内存创建一个菜单项
  * @param  name 菜单项名字
  * @param  super 父菜单项
  * @param  level 菜单等级
  * @param  funP 菜单项的功能函数指针
  * @retval menuItemP 返回菜单项指针
  */
menuItemP createMenuItem(char* name, menuItemP super, unsigned char ID, funPoint funP){
	uint8_t i = 0;
	menuItemP menuI = (menuItemP)malloc(sizeof(menuItem));
	if(menuI == NULL) OLED_ShowString(1, 1, "ERROR");		/* 内存分配失败，则显示ERROR */
	
	menuI->itemName = name;
	menuI->superItem = super;
	menuI->menuItemID = ID;
	menuI->funP = funP;
	for(; i < ITEMS_NUM; i ++) menuI->subItems[i] = NULL;	/* 子菜单项初始化为NULL */
	
	return menuI;
}

/**
  * @brief  显示菜单
  * @param  status 显示菜单时的状态
  * @retval 无
  */
void showMenu(showMenuStatus status, encoderDirection direction) {
	menuItemP next = NULL, super = NULL;		/* next为箭头指针要更新的菜单项指针，super为父菜单项 */
	menuItemP grandSuper;						/* grandSuper祖父菜单项 */
	uint8_t superId, superLine, currentLevel;	/* superId父菜单项ID，superLine父菜单项所在行，currentLevel当前项菜单等级 */
	uint8_t i = 0, j;						
	menuItemP temp;

	/* 显示子菜单 */
	if(status == subMenu) {
		temp = arrowPoint->currentMItem->subItems[0];
		currentLevel = arrowPoint->currentMenuLevel;
		OLED_Clear();
		if(currentLevel != 0) {		/* 若当前菜单项的菜单等级不为0，则保存其所在行到父项所在行中*/
			arrowPoint->supersItemLine[currentLevel - 1] = arrowPoint->currentLine;	
		}
		arrowPoint->currentLine = 1;							/* 更新箭头指针的所在行 */
		arrowPoint->currentMenuLevel ++;
		/* 显示箭arrowPoint头和菜单项 */
//		OLED_ShowString(arrowPoint->currentLine, strlen(temp->itemName) + 1, "<--");
		while(i <= 3 && arrowPoint->currentMItem->subItems[i] != NULL){
			OLED_ShowChinese(i + 1, 1, arrowPoint->currentMItem->subItems[i]->itemName);
			i++;
		}
		arrowPoint->currentMItem = temp;		/* 更新箭头指针的当前指向菜单项的指针 */
		
	/* 显示同级菜单 */
	}else if(status == selfMenu) {
		super = arrowPoint->currentMItem->superItem;
		/* 正转上滑 */
		if(direction == forwardDir) {
			/* 箭头指向的菜单项的编号+1为子菜单项数组最大长度时，表示没有下一个菜单项了 */
			if(arrowPoint->currentMItem->menuItemID+1 >= ITEMS_NUM)	
				next = NULL;
			else 
				next = super->subItems[(arrowPoint->currentMItem->menuItemID)+1];
			/* 存在下一个菜单项 */
			if(next != NULL){	
				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "   ");
				arrowPoint->currentMItem = next;			/* 更新箭头当前指向 */
				if(arrowPoint->currentLine < 4) {			/* 当箭头没有指向最后一行时 */
					arrowPoint->currentLine ++;
				}else if(arrowPoint->currentLine == 4) {	/* 当箭头指向最后一行时 */
					OLED_Clear();
					/* 显示菜单项 */
					j = arrowPoint->currentMItem->menuItemID - 3;
					while(i <= 3){
						OLED_ShowChinese(i + 1, 1, super->subItems[j]->itemName);
						i++;
						j++;
					}
				}
//				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");
			/* 不存在下一个菜单项 */
			}else {
				
			}
		/* 反转下滑 */
		}else if(direction == reversalDir) {
			if(arrowPoint->currentMItem->menuItemID > 0)
				next = super->subItems[(arrowPoint->currentMItem->menuItemID)-1];
			else 
				next = NULL;
			/* 存在下一个菜单项 */
			if(next != NULL){
				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "   ");
				arrowPoint->currentMItem = next;			/* 更新箭头当前指向 */
				if(arrowPoint->currentLine > 1) {			/* 当箭头没有指向最后一行时 */
					arrowPoint->currentLine --;
				}else if(arrowPoint->currentLine == 1) {	/* 当箭头指向最后一行时 */
					OLED_Clear();
					/* 显示菜单项 */
					j = arrowPoint->currentMItem->menuItemID;
					while(i <= 3){
						OLED_ShowChinese(i + 1, 1, super->subItems[j]->itemName);
						i++;
						j++;
					}
				}
//				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");
			/* 不存在下一个菜单项 */
			}else {
				
			}
		}else if(direction == noDir) {
			
			j = arrowPoint->currentMItem->menuItemID - (arrowPoint->currentLine - 1);
			while(i <= 3 && super->subItems[j] != NULL) {
				OLED_ShowChinese(i + 1, 1, super->subItems[j]->itemName);
				i++;
				j++;
			}
		}
	/* 返回显示父菜单 */
	}else if(status == superMenu) {
		OLED_Clear();
		super = arrowPoint->currentMItem->superItem;				/* 获取父菜单项 */
		grandSuper = super->superItem;								/* 获取父菜单项的父菜单项 */
		superId = super->menuItemID;								/* 获取父菜单项的ID */
		currentLevel = arrowPoint->currentMenuLevel;
		superLine = arrowPoint->supersItemLine[currentLevel - 2];	/* 获取先前进入显示子菜单时父菜单项的所在行 */
		
		arrowPoint->currentMItem = super;					/* 更新箭头指针 */
		arrowPoint->currentLine = superLine;
		arrowPoint->currentMenuLevel--;
		arrowPoint->supersItemLine[currentLevel - 2] = 0;
		
		/* 显示菜单项 */
		j = superId - (superLine - 1);
		for(; i <= 3 && grandSuper->subItems[j] != NULL; i++) {
			OLED_ShowChinese(i + 1, 1, grandSuper->subItems[j++]->itemName);
		}
//		OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");
	}
	
	/* 显示箭头 */
	OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");

	
	
}

/***********************************************************************************************************************/
/***************************************----------菜单项功能函数----------*************************************************/

/**
  * @brief  首菜单项功能函数
  * @param  无
  * @retval 无
  */
void menuTopFun(void) {
	menuFlag = menuShow;					/* 标志位置显示菜单 */
	showMenu(subMenu, noDir);				/* 显示菜单 */
	timingQuitSleep();						/* 退出定时休眠模式 */
}

/**
  * @brief  退出菜单项功能函数，释放初始化时动态分配的内存空间(菜单项和箭头)
  * @param  无
  * @retval 无
  */
void quitMenuFun(void){
	
/*
	释放动态申请的菜单项内存
		因C语言的内存管理算法原因，在申请内存、释放内存后，可能会产生内存碎片，
		导致可用的堆内存越来越少。因此不再每次进入菜单时动态分配内存给菜单项，
		退出菜单时释放内存，而采用只在首次进入菜单时分配内存空间，退出菜单不再释放其内存。
*/
//	uint8_t i = 0, j = 0;
//	menuItemP top = NULL, level1 = NULL;		/* top为首菜单项指针，level1为一级菜单项指针 */

//	/* 遍历父菜单项，得到首菜单项 */
//	while(arrowPoint->currentMItem->superItem != NULL) {
//		arrowPoint->currentMItem = arrowPoint->currentMItem->superItem;
//	}
//	top = arrowPoint->currentMItem;
//	/* 遍历一级菜单项 */
//	while(top->subItems[i] != NULL && i < ITEMS_NUM) {
//		arrowPoint->currentMItem = top->subItems[i];
//		level1 = arrowPoint->currentMItem;
//		/* 遍历二级菜单项 */
//		while(level1->subItems[j] != NULL && j < ITEMS_NUM) {
//			arrowPoint->currentMItem = level1->subItems[j];
//			free(arrowPoint->currentMItem);		/* 释放二级菜单项 */
//			j++;
//		}
//		free(level1);		/* 释放一级菜单项 */
//		i++;
//	}
//	free(top);				/* 释放首菜单项 */
//	free(arrowPoint);		/* 释放箭头 */
	
	
	OLED_Clear();
	/* 菜单标志置显示标准时间 */
	menuFlag = menuBlank; 	
	/* 箭头指针指向首菜单项，便于下次进入菜单显示菜单 */
	arrowPoint->currentMItem = arrowPoint->currentMItem->superItem;
	
}

/* 一级菜单项功能函数实现
***********************************************************************************************/

/**
  * @brief  返回菜单项功能函数，返回上一级菜单显示父菜单
  * @param  无
  * @retval 无
  */
void backFun(void) {
	showMenu(superMenu, noDir);
}

/**
  * @brief  设置菜单项功能函数，显示下一级菜单
  * @param  无
  * @retval 无
  */
void setFun(void) {
	showMenu(subMenu, noDir);
}

/**
  * @brief  实用工具菜单项功能函数，显示下一级菜单
  * @param  无
  * @retval 无
  */
void toolsFun(void) {
	showMenu(subMenu, noDir);
}

/**
  * @brief  运动健康功能函数，显示下一级菜单
  * @param  无
  * @retval 无
  */
void healthFun(void) {
	showMenu(subMenu, noDir);
}

/* 定时休眠设置功能函数实现
***********************************************************************************************/

/**
  * @brief  进入定时休眠设置界面功能函数
  * @param  无
  * @retval 无
  */
void sleepSetFun(void) {
	menuFlag = menuShow;
	funShowFlag = timingSleepShow;
	showMenu(subMenu, noDir);
	
}

/**
  * @brief  定时休眠确定设置功能函数
  * @param  无
  * @retval 无
  */
void sleepEnterFun(void) {
	setTiming();
}

/**
  * @brief  退出定时休眠界面功能函数
  * @param  无
  * @retval 无
  */
void sleepBackFun(void) {
	funShowFlag = noneShow;
	backFun();
}

/* 修改时间、闹钟功能函数实现
***********************************************************************************************/

/**
  * @brief  设置标准时间功能函数
  * @param  无
  * @retval 无
  */
void timeSetFun(void) {
	OLED_Clear();
	if(funShowFlag != alterTimeShow) {
		funShowFlag = alterTimeShow;
		menuFlag = menuFun;
		goAlterTimeMode();
	}else {
		funShowFlag = noneShow;
		menuFlag = menuShow;
		quitAlterTimeMode();
		showMenu(selfMenu, noDir);
	}
}

/**
  * @brief  设置闹钟功能函数
  * @param  无
  * @retval 无
  */
void alarmSetFun(void) {
	OLED_Clear();
	if(funShowFlag != alterAlarmShow) {
		funShowFlag = alterAlarmShow;
		menuFlag = menuFun;
		goAlterAlarmMode();
	}else {
		funShowFlag = noneShow;
		menuFlag = menuShow;
		quitAlterAlarmMode();
		showMenu(selfMenu, noDir);
	}
}

/* 秒表功能函数实现
***********************************************************************************************/

/**
  * @brief  秒表功能函数
  * @param  无
  * @retval 无
  */
void secondWatchFun(void) {
	showMenu(subMenu, noDir);
	menuFlag = menuShow;
	funShowFlag = secondWatchShow;
	
}

/**
  * @brief  秒表开始/暂停计数
  * @param  无
  * @retval 无
  */
void secondWatchBeginOrSusFun(void) {
	if(strcmp(arrowPoint->currentMItem->itemName, "开始") == 0) {
		arrowPoint->currentMItem->itemName = "暂停";
		goSecondWatchMode();
	}else {
		arrowPoint->currentMItem->itemName = "开始";
		suspendSecondWatchMode();
	}
	
	showMenu(selfMenu, noDir);

}

/**
  * @brief  重置秒表计数值
  * @param  无
  * @retval 无
  */
void secondWatchReset(void) {
	resetSecondWatchMode();
}

/**
  * @brief  退出秒表功能显示界面
  * @param  无
  * @retval 无
  */
void secondWatchBackFun(void) {
	funShowFlag = noneShow;
	backFun();
}

/* 手电筒功能函数实现
***********************************************************************************************/

/**
  * @brief  手电筒功能函数
  * @param  无
  * @retval 无
  */
void flashLightFun(void) {
	
}

/* 测体温功能函数实现
***********************************************************************************************/

/**
  * @brief  测体温功能函数
  * @param  无
  * @retval 无
  */
void tempFun(void) {
	showMenu(subMenu, noDir);

}

/* 测心率功能函数实现
***********************************************************************************************/

/**
  * @brief  测心率功能函数
  * @param  无
  * @retval 无
  */
void heartFun(void) {
	showMenu(subMenu, noDir);		/* 显示子菜单 */
	setOnceDis();					/* 进入测心率功能前设置首次显示标志 */
	menuFlag = menuShow;			/* 置菜单显示标志 */
	funShowFlag = heartShow;		/* 置心率显示标志 */
	
}

/**
  * @brief  退出测心率功能函数
  * @param  无
  * @retval 无
  */
void heartBackFun(void) {
	funShowFlag = noneShow;			/* 置功能显示空标志 */
	backFun();                      /* 返回上级菜单 */
}

/**
  * @brief  开始/暂停测心率/血氧功能函数
  * @param  无
  * @retval 无
  */
void heartSpo2BeginOrSusFun(void) {
	/* 进入或退出测心率/血氧模式 */
	enterOrQuitHeartSpo2Mode();		
	/* 翻转显示菜单项名字（开始/暂停） */
	if(strcmp(arrowPoint->currentMItem->itemName, "开始") == 0) {
		arrowPoint->currentMItem->itemName = "暂停";
	}else {
		arrowPoint->currentMItem->itemName = "开始";
	}
	/* 刷新当前菜单显示 */
	showMenu(selfMenu, noDir);
}

/* 测血氧功能函数实现
***********************************************************************************************/

/**
  * @brief  测血氧功能函数
  * @param  无
  * @retval 无
  */
void bloodOxygenFun(void) {
	showMenu(subMenu, noDir);		 /* 显示子菜单 */
	setOnceDis();                    /* 进入测血氧功能前设置首次显示标志 */
	menuFlag = menuShow;             /* 置菜单显示标志 */
	funShowFlag = spo2Show;          /* 置血氧显示标志 */
}

/**
  * @brief  退出测血氧功能函数
  * @param  
  * @retval 
  */
void bloodOxygenBackFun(void){
	funShowFlag = noneShow;			/* 置功能显示空标志 */
	backFun();						/* 返回上级菜单 */
}

/**
  * @brief  开始/暂停测血氧功能函数
  * @param  无
  * @retval 无
  */
//void bloodOxygenBeginOrSusFun(void){
//	enterOrQuitHeartSpo2Mode();
//	if(strcmp(arrowPoint->currentMItem->itemName, "开始") == 0) {
//		arrowPoint->currentMItem->itemName = "暂停";
//	}else {
//		arrowPoint->currentMItem->itemName = "开始";
//	}
//	showMenu(selfMenu, noDir);
//}


/* 空函数
***********************************************************************************************/

/**
  * @brief  空函数，什么也不执行
  * @param  无
  * @retval 无
  */
void NULLFun(void) {

}
