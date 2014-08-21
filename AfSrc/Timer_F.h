
/***********************************************
Author				Finder
FileName			Timer0_F.h
Version				V1.0
Date				2014-07-21
Description
***********************************************/

#ifndef __TIMER0_F_H__
#define __TIMER0_F_H__


#include <iocc2530.h>
#include "TypeDef_F.h"


//===================================��������һ��Ҫ���ң�������
//TMR_Poll����Ҫ�����Եر����á����ܲ��������붨ʱ����ص��¼�
//���紮�ڽ��ճ�ʱ�¼�
void TMR_Poll(void);


void TimerInit(void);

void DelayMs(U8 uTime);

U32 GetTickCount(void);			//ȡʱ�� Tick ����λΪ 0.1 mS

typedef void (*PFN_TimerPorc)(void);
void StartOnceTimer(U16 time,PFN_TimerPorc pfnTimerProc);













#endif //__TIMER0_F_H__