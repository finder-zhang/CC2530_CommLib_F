
#include <iocc2530.h>

//#include "Includes_F.h"

#include "Timer_F.h"

#include "UartInternal_F.h"


static volatile U32 _TimeTick = 0;		//系统时间，单位为 十分之一 毫秒

U32 GetTickCount(void)
{
	return _TimeTick;
}

void TimerInit(void)
{
//============================================================
//												定时器3部份
	T3CTL = (0x07 << 5)		//分频数
		|   (0x00 << 3)		//溢出中断，不需比较，只要由0xFF跳变0则中断
		|	(0x02);			//循环增加工作模式

	T3CCTL0 |= 0x04;		//使用比较模式
	//T3CC0 = 24;
	T3CC0 = 24;				//128分频时，4uS加1，25下刚好0.1mS
    T3CTL |= 0x10;           //启动	
	T3IE = 1;                //开T3中断
	
	
	
//============================================================
//												定时器4部份
//	T4CTL = (0x07 << 5)		//分频数
//		|   (0x00 << 3)		//开溢出中断
//		|	(0x00);			//循环增加工作模式
//	
//	T4CCTL0 =	(0x01 << 6)	//通道0中断  0：不屏蔽，1：屏蔽
//		|		(0x00 << 3)	//比较输出模式
//		|		(0x00 << 2)	//模式 0：捕获，1：比较
//		|		(0x01 << 1)	//捕获 1：下降沿捕获
//		|		(0x01 << 0);//捕获 1：上升沿捕获
	
//	T4CCTL0 |= 0x04;
	
//	P2SEL |= 0x01;			//P20指向定时器4的输入		
//	PERCFG |= 0x10;			//Timer4外部引脚使用备用位置2
	
//	T4CC0 = 249;				//128分频时，4uS加1，250下刚好1mS
//	T4CTL |= 0x10;           //启动	
//	T4IE = 1;                //开T4中断	
}


//This function can success even _TimeTick is overflow.
//delay must base on Timer3's _TimeTick count,
//_TimeTick 's unit is 0.1mS
void DelayMs(U16 wTime)
{
	U32 tmp = _TimeTick;
	tmp += wTime*10;
	while(tmp != _TimeTick){ }
}


//__interrupt void Timer1_ISR(void);
//#pragma vector = T1_VECTOR
//__interrupt void Timer1_ISR(void)
//{
//}


__interrupt void Timer3_ISR(void);
#pragma vector = T3_VECTOR
__interrupt void Timer3_ISR(void)
{
	static U8 _count = 0;
	//=======================================
	//						  系统1mS定时使用
	++_TimeTick;
	UART_TimeTick();
	
	//=======================================
	//							每5mS执行一次
	++_count;
	if ( _count > 49 ){
		_count = 0;
		//SetKeyCheckFlag((U8)1);
		
//		if (g_fnTimeToCheckKey) {
//			g_fnTimeToCheckKey();
//		}
		//osal_set_event(CP_App_TaskID,CP_EVENT_CHECK_KEY);
	}
	
//	if ( T3CH0IF ) {
//		T3CH0IF = 0;
//	}	
	
}



//#pragma vector = T4_VECTOR
//__interrupt void Timer4_ISR(void)
//{
//	if ( T4CH0IF ) {
//		T4CH0IF = 0;
//		//T4CC0 = 252;
////		LED3 = 1;
////		LED2 = 1;
////		LED1 = 1;
////		LED4 = 1;
//		//Uprintf(&u0,"aa\r\n");
//	}
//	
//	//T4CH1IF = 0;
//	
//	
//	LED4 ^= 1;
//}


void TMR_Poll(void)
{
	UART_Poll();
}