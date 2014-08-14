
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
//#include "TypeDef_F.h"


void TimerInit(void);

void DelayMs(U8 uTime);

U32 GetTickCount(void);			//取时间 Tick ，单位为 0.1 mS

typedef void (*PFN_TimerPorc)(void);
void StartOnceTimer(U16 time,PFN_TimerPorc pfnTimerProc);












#endif //__TIMER0_F_H__