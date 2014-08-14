


#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <iocc2530.h>

//#include "Includes_F.h"

#include "TypeDef_F.h"
#include "CommonDef_F.h"
#include "Uart_F.h"
#include "UartInternal_F.h"





//=============================================串口0接收中断
__interrupt void Uart0Rx_ISR(void);
#pragma vector = URX0_VECTOR 
__interrupt void Uart0Rx_ISR(void) 
{ 
    URX0IF = 0;       // 清中断标志
	PushRxByte(&u0,U0DBUF);
}


//=============================================串口1接收中断
__interrupt void Uart1Rx_ISR(void);
#pragma vector = URX1_VECTOR 
__interrupt void Uart1Rx_ISR(void) 
{ 
    URX1IF = 0;       // 清中断标志
	PushRxByte(&u1,U1DBUF);
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
	};	
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
		return 0xFF;
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