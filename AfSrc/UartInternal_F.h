
/*************************************************************
文件名：			UartInternal_F.h
作者：				Finder
日期：				2014年7月
描述：				串口内部头文件，内部使用，不用给使用者
*************************************************************/

#ifndef __UART_INTERNAL_F_H__
#define __UART_INTERNAL_F_H__

#include "Uart_F.h"

//输入与输出缓冲区，可以根据需要改变其大小，只能在编译期改，不可在运行期改
#define U0_RX_BUF_SIZE						(240)
#define U0_TX_BUF_SIZE						(16)


//外部的 UART_HANDLE 到达内部转换成 UART_OPERATOR ，与串口相关的状态数据
typedef struct _UartOperator
{
	U8					uRxBuf[U0_RX_BUF_SIZE];
	U8					uTxBuf[U0_TX_BUF_SIZE];
	
	UartBufLen			uRxHead;				//接收环形缓冲
	UartBufLen			uRxTail;
	UartBufLen			uRxLen;
	
	UartBufLen			uTxHead;				//发送环形缓冲
	UartBufLen			uTxTail;
	UartBufLen			uTxLen;

	U16					wReadTimeout;			//读取超时
	volatile U16		wReadTimeoutCount;		//读取超时计数，当wReadTimeoutCount >= wReadTimeout 时则超时了
	
	volatile U8			uTxActive;				//0：发送空闲  1：发送忙  用于决定等待还是开启一个新的发送
		
	U8					uNum;					//串口号，范围 1 ～ 2
	
	fn_Readed_t			pfnReaded;

}UART_OPERATOR,*PUART_OPERATOR;


//==============================================
extern UART_OPERATOR		u0;
extern UART_OPERATOR		u1;


//==============================================输出部份内部使用
static U8			IsTxEmpty(UART_HANDLE pH);
static UartBufLen	GetTxRemain(UART_HANDLE pH);
static UartBufLen	PushTxBytes(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);
static U8			PopTxByte(UART_HANDLE pH);
static UartBufLen	_UART_WriteHAL(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);



//==============================================输入部份内部使用
static U8			IsRxEmpty(UART_HANDLE pH);
static U8			PushRxByte(UART_HANDLE pH,U8 uPush);
static U8			IsRxFull(UART_HANDLE pH);
static U8			PopRxByte(UART_HANDLE pH);



//==============================================输入部份
void UART_TimeTick(void);


#endif //__UART_INTERNAL_F_H__