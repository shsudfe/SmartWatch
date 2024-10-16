#ifndef __MENU_H
#define __MENU_H
#include "sys/sys.h"

//#define ITEM_NAME_MAXSIZE			30
#define ITEMS_NUM					6		/* �Ӳ˵������� */
#define MAX_LEVEL					3		/* ���˵��ȼ� */

typedef struct menuI* menuItemP;			/* �˵���ָ�� */
typedef void (*funPoint)(void);				/* �˵���ܺ��� */

/* �˵���ṹ�� */
typedef struct menuI {
	char *itemName;							/* �˵������� */
	menuItemP superItem;					/* ���˵���ָ�� */
	menuItemP subItems[ITEMS_NUM];			/* �Ӳ˵������� */
	unsigned char menuItemID;				/* �˵����� */
	funPoint funP;							/* ���ܺ��� */
}menuItem;

/* ��ͷ�ṹ�� */
typedef struct arrow {
	menuItemP currentMItem;					/* ��ͷ��ǰָ��Ĳ˵��� */
	uint8_t currentMenuLevel;				/* ��ǰ�˵���ĵȼ� */
	uint8_t currentLine;					/* ��ͷ���ڵĵ�ǰ�� */
	uint8_t supersItemLine[MAX_LEVEL - 1];	/* ������ʾ�Ӳ˵�ʱ���˵��������У����ڷ���ʱ�ָ���ʾ�������СΪ�˵����ȼ�-1 */
}arrow, *arrowP;


/* (��ͬ״̬��)��ʾ�˵� */
typedef enum showMenuS {
	superMenu,					/* ������ʾ���˵� */
	subMenu,					/* ��ʾ�Ӳ˵� */
	selfMenu,					/* ��ǰ�˵��л� */
}showMenuStatus;

/* ��ת��������ת���� */
typedef enum encoderD {
	noDir,						/* ��ת��������ת */
	forwardDir,					/* ��ת */
	reversalDir,				/* ��ת */
}encoderDirection;

/* �˵���ʾ��־ */
typedef enum showMenuF {
	menuShow,					/* �˵���ʾ */
	menuBlank,					/* �˵�����ʾ����׼ʱ����ʾ */
	menuFun,					/* �˵�����ʾ����ʾ���ܽ��� */
}showMenuFlag;

/* �˵���ܽ�����ʾ */
typedef enum menuF {
	noneShow,					/* ����ʾ���ܽ��� */
	alterTimeShow,				/* �޸�ʱ����ʾ */
	alterAlarmShow,				/* �޸�������ʾ */
	secondWatchShow,			/* �����ʾ */
	timingSleepShow,			/* ��ʱ������ʾ */
	heartShow,					/* ������ʾ */
	spo2Show,					/* Ѫ����ʾ */
	
}menuFunShow;				

/* ��ͷָ�� */
extern arrowP arrowPoint;
/* �˵���ʾ��־ */
extern showMenuFlag menuFlag;
/* ������ʾ��־ */
extern menuFunShow funShowFlag;

/* �˵���ʼ�� */
void Menu_Init(void);
/* ��ʼ��һ���˵������ */
menuItemP createMenuItem(char* name, menuItemP super, unsigned char level, funPoint funP);
/* ��ʾ�˵� */
void showMenu(showMenuStatus status, encoderDirection direction);

/* ���˵���ܺ��� */
/* �ײ˵��� */
void menuTopFun(void);
void quitMenuFun(void);

void backFun(void);
void setFun(void);
void toolsFun(void);
void healthFun(void);

/* �޸�����ʱ��˵���ܺ��� */
void sleepSetFun(void);
void sleepEnterFun(void);
void sleepBackFun(void);

/* ����ʱ������ӹ��ܺ��� */
void timeSetFun(void);
void alarmSetFun(void);

/* ����ܺ��� */
void secondWatchFun(void);
void secondWatchBeginOrSusFun(void);
void secondWatchReset(void);
void secondWatchBackFun(void);

/* �ֵ�Ͳ���ܺ��� */
void flashLightFun(void);

/* ���²������ܺ��� */
void tempFun(void);

/* ���ʲ������ܺ��� */
void heartFun(void);
void heartBackFun(void);
void heartSpo2BeginOrSusFun(void);		/* ���ʺ�Ѫ����ʼ����ͣ��������һ����һ���������� */

/* Ѫ���������ܺ��� */
void bloodOxygenFun(void);
void bloodOxygenBackFun(void);
//void bloodOxygenBeginOrSusFun(void);

/* �պ��� */
void NULLFun(void);

#endif


