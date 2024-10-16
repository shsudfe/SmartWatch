#include <stdlib.h>
#include <string.h>
#include "menu/menu.h"
#include "oled/oled.h"
#include "rtc/MyRTC.h"
#include "rtc/MyAlarm.h"
#include "rtc/secondWatch.h"
#include "sleep/timingSleep.h"
#include "max30102/heart_spo2.h"


arrowP arrowPoint = NULL;				/* ȫ�ֱ�����ͷָ�� */
showMenuFlag menuFlag = menuBlank;		/* �˵���ʾ��־��Ĭ�ϲ���ʾ�˵� */
menuFunShow funShowFlag = noneShow;		/* ������ʾ��־��Ĭ�ϲ���ʾ���� */


/**
  * @brief  �˵���ʼ���������ײ˵�������˵����ͷָ��ĳ�ʼ��
  * @param  ��
  * @retval ��
  */
void Menu_Init(void){
	uint8_t i;
	menuItemP menuTop = NULL;
	
	/* �ײ˵��� */
	menuTop = createMenuItem("��ѡ��", NULL, 0, menuTopFun);

	/* һ���˵��� */
	menuTop->subItems[0] = createMenuItem("����", menuTop, 0, quitMenuFun);
	menuTop->subItems[1] = createMenuItem("����", menuTop, 1, setFun);
	menuTop->subItems[2] = createMenuItem("ʵ�ù���", menuTop, 2, toolsFun);
	menuTop->subItems[3] = createMenuItem("�˶�����", menuTop, 3, healthFun);
	menuTop->subItems[4] = createMenuItem("�����˵�", menuTop, 4, NULLFun);

	
	/* �����˵���-->���� */
	menuTop->subItems[1]->subItems[0] = createMenuItem("����", menuTop->subItems[1], 0, backFun);
	menuTop->subItems[1]->subItems[1] = createMenuItem("Ϣ������", menuTop->subItems[1], 1, sleepSetFun);
	menuTop->subItems[1]->subItems[2] = createMenuItem("ʱ������", menuTop->subItems[1], 2, timeSetFun);
	menuTop->subItems[1]->subItems[3] = createMenuItem("��������", menuTop->subItems[1], 3, alarmSetFun);

	/* �����˵���-->ʵ�ù��� */
	menuTop->subItems[2]->subItems[0] = createMenuItem("����", menuTop->subItems[2], 0, backFun);
	menuTop->subItems[2]->subItems[1] = createMenuItem("���", menuTop->subItems[2], 1, secondWatchFun);
	menuTop->subItems[2]->subItems[2] = createMenuItem("�ֵ�Ͳ", menuTop->subItems[2], 2, flashLightFun);
	
	/* �����˵���-->�˶����� */
	menuTop->subItems[3]->subItems[0] = createMenuItem("����", menuTop->subItems[3], 0, backFun);
	menuTop->subItems[3]->subItems[1] = createMenuItem("���²���", menuTop->subItems[3], 1, tempFun);
	menuTop->subItems[3]->subItems[2] = createMenuItem("���ʲ���", menuTop->subItems[3], 2, heartFun);
	menuTop->subItems[3]->subItems[3] = createMenuItem("Ѫ������", menuTop->subItems[3], 3, bloodOxygenFun);

	
	/* �����˵���-->Ϣ������ */
	menuTop->subItems[1]->subItems[1]->subItems[0] = createMenuItem("����", menuTop->subItems[1]->subItems[1], 0, sleepBackFun);
	menuTop->subItems[1]->subItems[1]->subItems[1] = createMenuItem("ȷ��", menuTop->subItems[1]->subItems[1], 1, sleepEnterFun);

	/* �����˵���-->��� */
	menuTop->subItems[2]->subItems[1]->subItems[0] = createMenuItem("����", menuTop->subItems[2]->subItems[1], 0, secondWatchBackFun);
	menuTop->subItems[2]->subItems[1]->subItems[1] = createMenuItem("��ʼ", menuTop->subItems[2]->subItems[1], 1, secondWatchBeginOrSusFun);
	menuTop->subItems[2]->subItems[1]->subItems[2] = createMenuItem("����", menuTop->subItems[2]->subItems[1], 2, secondWatchReset);

	/* �����˵���-->���²���*/
	menuTop->subItems[3]->subItems[1]->subItems[0] = createMenuItem("����", menuTop->subItems[3]->subItems[1], 0, backFun);
	menuTop->subItems[3]->subItems[1]->subItems[1] = createMenuItem("ȷ��", menuTop->subItems[3]->subItems[1], 1, NULLFun);

	/* �����˵���-->���ʲ���*/
	menuTop->subItems[3]->subItems[2]->subItems[0] = createMenuItem("����", menuTop->subItems[3]->subItems[2], 0, heartBackFun);
	menuTop->subItems[3]->subItems[2]->subItems[1] = createMenuItem("��ʼ", menuTop->subItems[3]->subItems[2], 1, heartSpo2BeginOrSusFun);
	
	/* �����˵���-->Ѫ������*/
	menuTop->subItems[3]->subItems[3]->subItems[0] = createMenuItem("����", menuTop->subItems[3]->subItems[3], 0, bloodOxygenBackFun);
	menuTop->subItems[3]->subItems[3]->subItems[1] = createMenuItem("��ʼ", menuTop->subItems[3]->subItems[3], 1, heartSpo2BeginOrSusFun);

	/* �����ڴ����ͷָ�� */
	arrowPoint = malloc(sizeof(arrow));
	
	/* ��ͷָ���ʼ�� */
	arrowPoint->currentMItem = menuTop;			/* ��ʼ����ͷָ��ָ���ײ˵��� */
	arrowPoint->currentMenuLevel = 0;			/* ��ʼ���˵��ȼ�Ϊ0 */
	arrowPoint->currentLine = 1;				/* ��ʼ����ͷָ��������Ϊ1 */
	for (i = 0; i < MAX_LEVEL - 1; i++){
		arrowPoint->supersItemLine[i] = 0;		/* ��ʼ����ͷָ��ָ��˵���ĸ��˵���������Ϊ0 */
	}  
}

/**
  * @brief  ��̬�����ڴ洴��һ���˵���
  * @param  name �˵�������
  * @param  super ���˵���
  * @param  level �˵��ȼ�
  * @param  funP �˵���Ĺ��ܺ���ָ��
  * @retval menuItemP ���ز˵���ָ��
  */
menuItemP createMenuItem(char* name, menuItemP super, unsigned char ID, funPoint funP){
	uint8_t i = 0;
	menuItemP menuI = (menuItemP)malloc(sizeof(menuItem));
	if(menuI == NULL) OLED_ShowString(1, 1, "ERROR");		/* �ڴ����ʧ�ܣ�����ʾERROR */
	
	menuI->itemName = name;
	menuI->superItem = super;
	menuI->menuItemID = ID;
	menuI->funP = funP;
	for(; i < ITEMS_NUM; i ++) menuI->subItems[i] = NULL;	/* �Ӳ˵����ʼ��ΪNULL */
	
	return menuI;
}

/**
  * @brief  ��ʾ�˵�
  * @param  status ��ʾ�˵�ʱ��״̬
  * @retval ��
  */
void showMenu(showMenuStatus status, encoderDirection direction) {
	menuItemP next = NULL, super = NULL;		/* nextΪ��ͷָ��Ҫ���µĲ˵���ָ�룬superΪ���˵��� */
	menuItemP grandSuper;						/* grandSuper�游�˵��� */
	uint8_t superId, superLine, currentLevel;	/* superId���˵���ID��superLine���˵��������У�currentLevel��ǰ��˵��ȼ� */
	uint8_t i = 0, j;						
	menuItemP temp;

	/* ��ʾ�Ӳ˵� */
	if(status == subMenu) {
		temp = arrowPoint->currentMItem->subItems[0];
		currentLevel = arrowPoint->currentMenuLevel;
		OLED_Clear();
		if(currentLevel != 0) {		/* ����ǰ�˵���Ĳ˵��ȼ���Ϊ0���򱣴��������е�������������*/
			arrowPoint->supersItemLine[currentLevel - 1] = arrowPoint->currentLine;	
		}
		arrowPoint->currentLine = 1;							/* ���¼�ͷָ��������� */
		arrowPoint->currentMenuLevel ++;
		/* ��ʾ��arrowPointͷ�Ͳ˵��� */
//		OLED_ShowString(arrowPoint->currentLine, strlen(temp->itemName) + 1, "<--");
		while(i <= 3 && arrowPoint->currentMItem->subItems[i] != NULL){
			OLED_ShowChinese(i + 1, 1, arrowPoint->currentMItem->subItems[i]->itemName);
			i++;
		}
		arrowPoint->currentMItem = temp;		/* ���¼�ͷָ��ĵ�ǰָ��˵����ָ�� */
		
	/* ��ʾͬ���˵� */
	}else if(status == selfMenu) {
		super = arrowPoint->currentMItem->superItem;
		/* ��ת�ϻ� */
		if(direction == forwardDir) {
			/* ��ͷָ��Ĳ˵���ı��+1Ϊ�Ӳ˵���������󳤶�ʱ����ʾû����һ���˵����� */
			if(arrowPoint->currentMItem->menuItemID+1 >= ITEMS_NUM)	
				next = NULL;
			else 
				next = super->subItems[(arrowPoint->currentMItem->menuItemID)+1];
			/* ������һ���˵��� */
			if(next != NULL){	
				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "   ");
				arrowPoint->currentMItem = next;			/* ���¼�ͷ��ǰָ�� */
				if(arrowPoint->currentLine < 4) {			/* ����ͷû��ָ�����һ��ʱ */
					arrowPoint->currentLine ++;
				}else if(arrowPoint->currentLine == 4) {	/* ����ͷָ�����һ��ʱ */
					OLED_Clear();
					/* ��ʾ�˵��� */
					j = arrowPoint->currentMItem->menuItemID - 3;
					while(i <= 3){
						OLED_ShowChinese(i + 1, 1, super->subItems[j]->itemName);
						i++;
						j++;
					}
				}
//				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");
			/* ��������һ���˵��� */
			}else {
				
			}
		/* ��ת�»� */
		}else if(direction == reversalDir) {
			if(arrowPoint->currentMItem->menuItemID > 0)
				next = super->subItems[(arrowPoint->currentMItem->menuItemID)-1];
			else 
				next = NULL;
			/* ������һ���˵��� */
			if(next != NULL){
				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "   ");
				arrowPoint->currentMItem = next;			/* ���¼�ͷ��ǰָ�� */
				if(arrowPoint->currentLine > 1) {			/* ����ͷû��ָ�����һ��ʱ */
					arrowPoint->currentLine --;
				}else if(arrowPoint->currentLine == 1) {	/* ����ͷָ�����һ��ʱ */
					OLED_Clear();
					/* ��ʾ�˵��� */
					j = arrowPoint->currentMItem->menuItemID;
					while(i <= 3){
						OLED_ShowChinese(i + 1, 1, super->subItems[j]->itemName);
						i++;
						j++;
					}
				}
//				OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");
			/* ��������һ���˵��� */
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
	/* ������ʾ���˵� */
	}else if(status == superMenu) {
		OLED_Clear();
		super = arrowPoint->currentMItem->superItem;				/* ��ȡ���˵��� */
		grandSuper = super->superItem;								/* ��ȡ���˵���ĸ��˵��� */
		superId = super->menuItemID;								/* ��ȡ���˵����ID */
		currentLevel = arrowPoint->currentMenuLevel;
		superLine = arrowPoint->supersItemLine[currentLevel - 2];	/* ��ȡ��ǰ������ʾ�Ӳ˵�ʱ���˵���������� */
		
		arrowPoint->currentMItem = super;					/* ���¼�ͷָ�� */
		arrowPoint->currentLine = superLine;
		arrowPoint->currentMenuLevel--;
		arrowPoint->supersItemLine[currentLevel - 2] = 0;
		
		/* ��ʾ�˵��� */
		j = superId - (superLine - 1);
		for(; i <= 3 && grandSuper->subItems[j] != NULL; i++) {
			OLED_ShowChinese(i + 1, 1, grandSuper->subItems[j++]->itemName);
		}
//		OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");
	}
	
	/* ��ʾ��ͷ */
	OLED_ShowString(arrowPoint->currentLine, strlen(arrowPoint->currentMItem->itemName) + 1, "<--");

	
	
}

/***********************************************************************************************************************/
/***************************************----------�˵���ܺ���----------*************************************************/

/**
  * @brief  �ײ˵���ܺ���
  * @param  ��
  * @retval ��
  */
void menuTopFun(void) {
	menuFlag = menuShow;					/* ��־λ����ʾ�˵� */
	showMenu(subMenu, noDir);				/* ��ʾ�˵� */
	timingQuitSleep();						/* �˳���ʱ����ģʽ */
}

/**
  * @brief  �˳��˵���ܺ������ͷų�ʼ��ʱ��̬������ڴ�ռ�(�˵���ͼ�ͷ)
  * @param  ��
  * @retval ��
  */
void quitMenuFun(void){
	
/*
	�ͷŶ�̬����Ĳ˵����ڴ�
		��C���Ե��ڴ�����㷨ԭ���������ڴ桢�ͷ��ڴ�󣬿��ܻ�����ڴ���Ƭ��
		���¿��õĶ��ڴ�Խ��Խ�١���˲���ÿ�ν���˵�ʱ��̬�����ڴ���˵��
		�˳��˵�ʱ�ͷ��ڴ棬������ֻ���״ν���˵�ʱ�����ڴ�ռ䣬�˳��˵������ͷ����ڴ档
*/
//	uint8_t i = 0, j = 0;
//	menuItemP top = NULL, level1 = NULL;		/* topΪ�ײ˵���ָ�룬level1Ϊһ���˵���ָ�� */

//	/* �������˵���õ��ײ˵��� */
//	while(arrowPoint->currentMItem->superItem != NULL) {
//		arrowPoint->currentMItem = arrowPoint->currentMItem->superItem;
//	}
//	top = arrowPoint->currentMItem;
//	/* ����һ���˵��� */
//	while(top->subItems[i] != NULL && i < ITEMS_NUM) {
//		arrowPoint->currentMItem = top->subItems[i];
//		level1 = arrowPoint->currentMItem;
//		/* ���������˵��� */
//		while(level1->subItems[j] != NULL && j < ITEMS_NUM) {
//			arrowPoint->currentMItem = level1->subItems[j];
//			free(arrowPoint->currentMItem);		/* �ͷŶ����˵��� */
//			j++;
//		}
//		free(level1);		/* �ͷ�һ���˵��� */
//		i++;
//	}
//	free(top);				/* �ͷ��ײ˵��� */
//	free(arrowPoint);		/* �ͷż�ͷ */
	
	
	OLED_Clear();
	/* �˵���־����ʾ��׼ʱ�� */
	menuFlag = menuBlank; 	
	/* ��ͷָ��ָ���ײ˵�������´ν���˵���ʾ�˵� */
	arrowPoint->currentMItem = arrowPoint->currentMItem->superItem;
	
}

/* һ���˵���ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  ���ز˵���ܺ�����������һ���˵���ʾ���˵�
  * @param  ��
  * @retval ��
  */
void backFun(void) {
	showMenu(superMenu, noDir);
}

/**
  * @brief  ���ò˵���ܺ�������ʾ��һ���˵�
  * @param  ��
  * @retval ��
  */
void setFun(void) {
	showMenu(subMenu, noDir);
}

/**
  * @brief  ʵ�ù��߲˵���ܺ�������ʾ��һ���˵�
  * @param  ��
  * @retval ��
  */
void toolsFun(void) {
	showMenu(subMenu, noDir);
}

/**
  * @brief  �˶��������ܺ�������ʾ��һ���˵�
  * @param  ��
  * @retval ��
  */
void healthFun(void) {
	showMenu(subMenu, noDir);
}

/* ��ʱ�������ù��ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  ���붨ʱ�������ý��湦�ܺ���
  * @param  ��
  * @retval ��
  */
void sleepSetFun(void) {
	menuFlag = menuShow;
	funShowFlag = timingSleepShow;
	showMenu(subMenu, noDir);
	
}

/**
  * @brief  ��ʱ����ȷ�����ù��ܺ���
  * @param  ��
  * @retval ��
  */
void sleepEnterFun(void) {
	setTiming();
}

/**
  * @brief  �˳���ʱ���߽��湦�ܺ���
  * @param  ��
  * @retval ��
  */
void sleepBackFun(void) {
	funShowFlag = noneShow;
	backFun();
}

/* �޸�ʱ�䡢���ӹ��ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  ���ñ�׼ʱ�书�ܺ���
  * @param  ��
  * @retval ��
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
  * @brief  �������ӹ��ܺ���
  * @param  ��
  * @retval ��
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

/* ����ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  ����ܺ���
  * @param  ��
  * @retval ��
  */
void secondWatchFun(void) {
	showMenu(subMenu, noDir);
	menuFlag = menuShow;
	funShowFlag = secondWatchShow;
	
}

/**
  * @brief  ���ʼ/��ͣ����
  * @param  ��
  * @retval ��
  */
void secondWatchBeginOrSusFun(void) {
	if(strcmp(arrowPoint->currentMItem->itemName, "��ʼ") == 0) {
		arrowPoint->currentMItem->itemName = "��ͣ";
		goSecondWatchMode();
	}else {
		arrowPoint->currentMItem->itemName = "��ʼ";
		suspendSecondWatchMode();
	}
	
	showMenu(selfMenu, noDir);

}

/**
  * @brief  ����������ֵ
  * @param  ��
  * @retval ��
  */
void secondWatchReset(void) {
	resetSecondWatchMode();
}

/**
  * @brief  �˳��������ʾ����
  * @param  ��
  * @retval ��
  */
void secondWatchBackFun(void) {
	funShowFlag = noneShow;
	backFun();
}

/* �ֵ�Ͳ���ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  �ֵ�Ͳ���ܺ���
  * @param  ��
  * @retval ��
  */
void flashLightFun(void) {
	
}

/* �����¹��ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  �����¹��ܺ���
  * @param  ��
  * @retval ��
  */
void tempFun(void) {
	showMenu(subMenu, noDir);

}

/* �����ʹ��ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  �����ʹ��ܺ���
  * @param  ��
  * @retval ��
  */
void heartFun(void) {
	showMenu(subMenu, noDir);		/* ��ʾ�Ӳ˵� */
	setOnceDis();					/* ��������ʹ���ǰ�����״���ʾ��־ */
	menuFlag = menuShow;			/* �ò˵���ʾ��־ */
	funShowFlag = heartShow;		/* ��������ʾ��־ */
	
}

/**
  * @brief  �˳������ʹ��ܺ���
  * @param  ��
  * @retval ��
  */
void heartBackFun(void) {
	funShowFlag = noneShow;			/* �ù�����ʾ�ձ�־ */
	backFun();                      /* �����ϼ��˵� */
}

/**
  * @brief  ��ʼ/��ͣ������/Ѫ�����ܺ���
  * @param  ��
  * @retval ��
  */
void heartSpo2BeginOrSusFun(void) {
	/* ������˳�������/Ѫ��ģʽ */
	enterOrQuitHeartSpo2Mode();		
	/* ��ת��ʾ�˵������֣���ʼ/��ͣ�� */
	if(strcmp(arrowPoint->currentMItem->itemName, "��ʼ") == 0) {
		arrowPoint->currentMItem->itemName = "��ͣ";
	}else {
		arrowPoint->currentMItem->itemName = "��ʼ";
	}
	/* ˢ�µ�ǰ�˵���ʾ */
	showMenu(selfMenu, noDir);
}

/* ��Ѫ�����ܺ���ʵ��
***********************************************************************************************/

/**
  * @brief  ��Ѫ�����ܺ���
  * @param  ��
  * @retval ��
  */
void bloodOxygenFun(void) {
	showMenu(subMenu, noDir);		 /* ��ʾ�Ӳ˵� */
	setOnceDis();                    /* �����Ѫ������ǰ�����״���ʾ��־ */
	menuFlag = menuShow;             /* �ò˵���ʾ��־ */
	funShowFlag = spo2Show;          /* ��Ѫ����ʾ��־ */
}

/**
  * @brief  �˳���Ѫ�����ܺ���
  * @param  
  * @retval 
  */
void bloodOxygenBackFun(void){
	funShowFlag = noneShow;			/* �ù�����ʾ�ձ�־ */
	backFun();						/* �����ϼ��˵� */
}

/**
  * @brief  ��ʼ/��ͣ��Ѫ�����ܺ���
  * @param  ��
  * @retval ��
  */
//void bloodOxygenBeginOrSusFun(void){
//	enterOrQuitHeartSpo2Mode();
//	if(strcmp(arrowPoint->currentMItem->itemName, "��ʼ") == 0) {
//		arrowPoint->currentMItem->itemName = "��ͣ";
//	}else {
//		arrowPoint->currentMItem->itemName = "��ʼ";
//	}
//	showMenu(selfMenu, noDir);
//}


/* �պ���
***********************************************************************************************/

/**
  * @brief  �պ�����ʲôҲ��ִ��
  * @param  ��
  * @retval ��
  */
void NULLFun(void) {

}
