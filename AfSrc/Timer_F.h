
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


//===================================！！！！一定要读我！！！！
//TMR_Poll函数要周期性地被调用。才能产生各种与定时器相关的事件
//例如串口接收超时事件
void TMR_Poll(void);


void TimerInit(void);

void DelayMs(U8 uTime);

U32 GetTickCount(void);			//取时间 Tick ，单位为 0.1 mS

typedef void (*PFN_TimerPorc)(void);
void StartOnceTimer(U16 time,PFN_TimerPorc pfnTimerProc);













#endif //__TIMER0_F_H__