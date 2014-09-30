


#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <iocc2530.h>

//#include "Includes_F.h"

#include "TypeDef_F.h"
#include "CommonDef_F.h"
#include "Timer_F.h"
#include "Uart_F.h"
#include "UartInternal_F.h"



//=============================================串口0接收中断
__interrupt void Uart0Rx_ISR(void);
#pragma vector = URX0_VECTOR
__interrupt void Uart0Rx_ISR(void)
{
    URX0IF = 0;       // 清中断标志
	PushRxByte(&u0,U0DBUF);
	u0.wReadTimeoutCount = 0;
}


//=============================================串口1接收中断
__interrupt void Uart1Rx_ISR(void);
#pragma vector = URX1_VECTOR
__interrupt void Uart1Rx_ISR(void)
{
    URX1IF = 0;       // 清中断标志
	PushRxByte(&u1,U1DBUF);
	u1.wReadTimeoutCount = 0;
}


//============================================内部函数
static U8			IsRxEmpty(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	return ( 0 == pU->uRxLen );
}


//压入一个字节，如果满了压不入则返回 FALSE
static U8			PushRxByte(UART_HANDLE pH,U8 uPush)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	if ( IsRxFull(pH) ) {
		return FALSE;
	}
	pU->uRxBuf[pU->uRxTail] = uPush;
	++pU->uRxTail;
	if ( pU->uRxTail >= U0_RX_BUF_SIZE ) {
		pU->uRxTail = 0;
	}
	++pU->uRxLen;
	return TRUE;
}


//推出一个字节，如果为空，则推出0xFF，在使用前必须查看缓冲区是否为空
//否则无法判断推出是否失败。
static U8			PopRxByte(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	U8 uPop = 0;
	if ( IsRxEmpty(pH) ) {
		return 'a';
	}
	uPop = pU->uRxBuf[pU->uRxHead];
	++pU->uRxHead;
	if ( pU->uRxHead >= U0_RX_BUF_SIZE ) {
		pU->uRxHead = 0;
	}
	--pU->uRxLen;
	return uPop;
}

static U8			IsRxFull(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	return ( (pU->uRxLen+1) >= U0_RX_BUF_SIZE );
}





//=============================================外部接口函数

//时间刷新，该函数以0.1秒为周期执行，由定时器负责执行
//不可长时间执行
void UART_TimeTick()
{
	if (u0.wReadTimeoutCount < u0.wReadTimeout) {
		++u0.wReadTimeoutCount;
	}
	if (u1.wReadTimeoutCount < u1.wReadTimeout) {
		++u0.wReadTimeoutCount;
	}
}

//串口检测，该函数由主循环周期性地调用
void UART_Poll(void)
{
	//如果超时时间到，并且有长度，则执行读超时
	if (	(u0.wReadTimeoutCount >= u0.wReadTimeout)
		&&	(u0.uRxLen > 0)		)
	{
		if ( u0.pfnReaded ) {
			u0.pfnReaded();
		}
	}
	
		
	if (	(u1.wReadTimeoutCount >= u1.wReadTimeout)
		&&	(u1.uRxLen > 0)		)
	{
		if ( u1.pfnReaded ) {
			u1.pfnReaded();
		}
	}
}


//求长度
UartBufLen	UART_GetRxLen(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	return pU->uRxLen;
}

//最常用的读取函数
//pH		要读的串口句柄
//outBuf	读出的字节，请保证outBuf的长度 >= uLen，否则破坏内存
//uLen		要读的长度
//返回值	真正能读出的字节长度
UartBufLen	UART_Read(UART_HANDLE pH,U8 *outBuf,UartBufLen uLen)
{
	//PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	UartBufLen i = 0;
	for (i=0; i<uLen; ++i) {
		if ( IsRxEmpty(pH) ) {
			break;
		}
		outBuf[i] = PopRxByte(pH);
	}
	return i;
}


UartBufLen			UART_ReadTimeout(UART_HANDLE pH,U8* outBuf,UartBufLen uLen,U32 dwMilliSeconds)
{
	//开始时间，结束时间，最近时间等变量，用于检测超时退出
	U32 dwTickStart = GetTickCount();
	U32 dwTickTimeout = dwMilliSeconds * 10;
	U32 dwTickEnd = dwTickStart + dwTickTimeout;
	U32 dwTickCurr = dwTickStart;
	
	//时间 Tick 为32位，有溢出可能，先计算是否有溢出
	BOOL bOverflow = dwTickEnd < dwTickStart;	//Check is TimeTick rollback
	
	UartBufLen uReaded = 0;
	
	while( uReaded < uLen )
	{
		uReaded += UART_Read(pH,outBuf + uReaded,uLen - uReaded);
		dwTickCurr = GetTickCount();
		if ( bOverflow ) {		//有溢出时的判断
			if ( (dwTickCurr >= dwTickEnd) && (dwTickCurr < dwTickStart) ) {
				break;
			}
		}
		else {		//没溢出时
			if ( dwTickCurr >= dwTickEnd ) {
				break;
			}
		}
	}
	
	return uReaded;
}