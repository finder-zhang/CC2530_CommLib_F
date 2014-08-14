

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


extern UART_OPERATOR		u0;
extern UART_OPERATOR		u1;


//=============================================串口0发送中断
__interrupt void Uart0Tx_ISR(void);
#pragma vector = UTX0_VECTOR 
__interrupt void Uart0Tx_ISR(void) 
{ 
    UTX0IF = 0;       // 清中断标志
	if ( IsTxEmpty(&u0) ) {
		u0.uTxActive = 0;
	}
	else {
		U0DBUF = PopTxByte(&u0);
	}
}

//=============================================串口1发送中断
__interrupt void Uart1Tx_ISR(void);
#pragma vector = UTX1_VECTOR 
__interrupt void Uart1Tx_ISR(void) 
{ 
    UTX0IF = 0;       // 清中断标志
	if ( IsTxEmpty(&u1) ) {
		u1.uTxActive = 0;
	}
	else {
		U1DBUF = PopTxByte(&u1);
	}
}


//==================================================内部函数
static U8 IsTxEmpty(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	return (pU->uTxHead == pU->uTxTail);
}

static UartBufLen GetTxRemain(UART_HANDLE pH)
{
	return U0_TX_BUF_SIZE - UART_GetTxLen(pH) - 1;
}


/***********************************************
把要发送的字节数组推入输出缓冲区的尾部，如遇缓冲区满，则能推多少算多少
inBuf			要推入的字节数组
uLen			要推入的字节数量
返回值			实际推入的数量，当缓冲区将小于uLen时，返回值小于uLen
				正常情况下返回值等于 uLen
***********************************************/
static U8 PushTxBytes(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	UartBufLen uPush = GetTxRemain(pH);
	uPush = min(uPush,uLen);	//推入的长度不可以大于TxRemain
	
	for (uLen = 0;uLen < uPush; ++uLen)
	{
		pU->uTxBuf[pU->uTxTail] = inBuf[uLen];
		++pU->uTxTail;
		if (pU->uTxTail >= U0_TX_BUF_SIZE)
		{
			pU->uTxTail = 0;
		}
		++pU->uTxLen;
	}
	return uPush;
}

/***************************************************
在输出缓冲区，把头部的一个字节拉出
返回值			0：输出缓冲区空。其他值：缓冲区头部字节
				注意：返回0并非意味着失败，因为缓冲区内
				的内容也可以是0，只能在执行本函数前，先
				判断缓冲区是否有内容（长度非零）。

				把是否能正常拉出数据使用BOOL值返回，然
				后拉出的数据在参数处以指针形式返回会更
				严谨，但执行效率与内存使用都不理想。
***************************************************/
static U8 PopTxByte(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	U8 uRet = 0;
	if ( IsTxEmpty(pH) ) {
		return 0;
	}
	else {
		uRet = pU->uTxBuf[pU->uTxHead];
		++pU->uTxHead;
		if (pU->uTxHead >= U0_TX_BUF_SIZE) {
			pU->uTxHead = 0;
		}
		--pU->uTxLen;
		return uRet;
	}
}














UartBufLen UART_GetTxLen(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	return pU->uTxLen;
//	if (pU->uTxTail >= pU->uTxHead) {
//		return pU->uTxTail - pU->uTxHead;
//	}
//	else {
//		return U0_TX_BUF_SIZE - ( pU->uTxHead - pU->uTxTail );
//	}
}

/**********************************************
同步发送字节，执行完函数后，马上返回。
pH			串口 Handle
inBuf		要写到串口的内容
uLen		要写到串口的长度
返回值		正常情况下，返回值 = uLen
			当输出缓冲区不足时，返回值 < uLen
**********************************************/
UartBufLen UART_Write(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	U8 uPushLen = PushTxBytes(pU,inBuf,uLen);
	//uTxActive 如果在发送中断内部发现要发送的长度为0时，把uTxActive
	//清0。则在这里要把 UTX0IF 置位以触发新一轮的发送
	//如果 uTxActive 为1，则新Push的字节会跟着自动发，不用额外处理
	if ( 0 == pU->uTxActive ) {
		pU->uTxActive = 1;
		if (0 == pU->uNum) {		
			UTX0IF = 1;			
		}
		else {
			UTX1IF = 1;
		}
	}
	return uPushLen;
}

void UART_WriteSync(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen)
{
	UartBufLen uWrited = 0;
	do
	{
		uWrited += UART_Write(pH,inBuf + uWrited,uLen - uWrited);
	}while(uWrited != uLen);
}