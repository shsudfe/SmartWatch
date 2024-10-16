#ifndef __MENU_H
#define __MENU_H
#include "sys/sys.h"

//#define ITEM_NAME_MAXSIZE			30
#define ITEMS_NUM					6		/* 子菜单项数量 */
#define MAX_LEVEL					3		/* 最大菜单等级 */

typedef struct menuI* menuItemP;			/* 菜单项指针 */
typedef void (*funPoint)(void);				/* 菜单项功能函数 */

/* 菜单项结构体 */
typedef struct menuI {
	char *itemName;							/* 菜单项名字 */
	menuItemP superItem;					/* 父菜单项指针 */
	menuItemP subItems[ITEMS_NUM];			/* 子菜单项数组 */
	unsigned char menuItemID;				/* 菜单项编号 */
	funPoint funP;							/* 功能函数 */
}menuItem;

/* 箭头结构体 */
typedef struct arrow {
	menuItemP currentMItem;					/* 箭头当前指向的菜单项 */
	uint8_t currentMenuLevel;				/* 当前菜单项的等级 */
	uint8_t currentLine;					/* 箭头所在的当前行 */
	uint8_t supersItemLine[MAX_LEVEL - 1];	/* 保存显示子菜单时父菜单项所在行，便于返回时恢复显示，数组大小为菜单最大等级-1 */
}arrow, *arrowP;


/* (不同状态下)显示菜单 */
typedef enum showMenuS {
	superMenu,					/* 返回显示父菜单 */
	subMenu,					/* 显示子菜单 */
	selfMenu,					/* 当前菜单切换 */
}showMenuStatus;

/* 旋转编码器旋转方向 */
typedef enum encoderD {
	noDir,						/* 旋转编码器不转 */
	forwardDir,					/* 正转 */
	reversalDir,				/* 反转 */
}encoderDirection;

/* 菜单显示标志 */
typedef enum showMenuF {
	menuShow,					/* 菜单显示 */
	menuBlank,					/* 菜单不显示，标准时间显示 */
	menuFun,					/* 菜单不显示，显示功能界面 */
}showMenuFlag;

/* 菜单项功能界面显示 */
typedef enum menuF {
	noneShow,					/* 不显示功能界面 */
	alterTimeShow,				/* 修改时间显示 */
	alterAlarmShow,				/* 修改闹钟显示 */
	secondWatchShow,			/* 秒表显示 */
	timingSleepShow,			/* 定时休眠显示 */
	heartShow,					/* 心率显示 */
	spo2Show,					/* 血氧显示 */
	
}menuFunShow;				

/* 箭头指针 */
extern arrowP arrowPoint;
/* 菜单显示标志 */
extern showMenuFlag menuFlag;
/* 功能显示标志 */
extern menuFunShow funShowFlag;

/* 菜单初始化 */
void Menu_Init(void);
/* 初始化一个菜单项并返回 */
menuItemP createMenuItem(char* name, menuItemP super, unsigned char level, funPoint funP);
/* 显示菜单 */
void showMenu(showMenuStatus status, encoderDirection direction);

/* 各菜单项功能函数 */
/* 首菜单项 */
void menuTopFun(void);
void quitMenuFun(void);

void backFun(void);
void setFun(void);
void toolsFun(void);
void healthFun(void);

/* 修改休眠时间菜单项功能函数 */
void sleepSetFun(void);
void sleepEnterFun(void);
void sleepBackFun(void);

/* 设置时间和闹钟功能函数 */
void timeSetFun(void);
void alarmSetFun(void);

/* 秒表功能函数 */
void secondWatchFun(void);
void secondWatchBeginOrSusFun(void);
void secondWatchReset(void);
void secondWatchBackFun(void);

/* 手电筒功能函数 */
void flashLightFun(void);

/* 体温测量功能函数 */
void tempFun(void);

/* 心率测量功能函数 */
void heartFun(void);
void heartBackFun(void);
void heartSpo2BeginOrSusFun(void);		/* 心率和血氧开始或暂停测量共用一个这一个函数即可 */

/* 血氧测量功能函数 */
void bloodOxygenFun(void);
void bloodOxygenBackFun(void);
//void bloodOxygenBeginOrSusFun(void);

/* 空函数 */
void NULLFun(void);

#endif


