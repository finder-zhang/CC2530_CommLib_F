

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


//=============================================����0�����ж�
__interrupt void Uart0Tx_ISR(void);
#pragma vector = UTX0_VECTOR 
__interrupt void Uart0Tx_ISR(void) 
{ 
    UTX0IF = 0;       // ���жϱ�־
	if ( IsTxEmpty(&u0) ) {
		u0.uTxActive = 0;
	}
	else {
		U0DBUF = PopTxByte(&u0);
	}
}

//=============================================����1�����ж�
__interrupt void Uart1Tx_ISR(void);
#pragma vector = UTX1_VECTOR 
__interrupt void Uart1Tx_ISR(void) 
{ 
    UTX0IF = 0;       // ���жϱ�־
	if ( IsTxEmpty(&u1) ) {
		u1.uTxActive = 0;
	}
	else {
		U1DBUF = PopTxByte(&u1);
	}
}


//==================================================�ڲ�����
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
��Ҫ���͵��ֽ��������������������β�����������������������ƶ��������
inBuf			Ҫ������ֽ�����
uLen			Ҫ������ֽ�����
����ֵ			ʵ�����������������������С��uLenʱ������ֵС��uLen
				��������·���ֵ���� uLen
***********************************************/
static U8 PushTxBytes(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	UartBufLen uPush = GetTxRemain(pH);
	uPush = min(uPush,uLen);	//����ĳ��Ȳ����Դ���TxRemain
	
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
���������������ͷ����һ���ֽ�����
����ֵ			0������������ա�����ֵ��������ͷ���ֽ�
				ע�⣺����0������ζ��ʧ�ܣ���Ϊ��������
				������Ҳ������0��ֻ����ִ�б�����ǰ����
				�жϻ������Ƿ������ݣ����ȷ��㣩��

				���Ƿ���������������ʹ��BOOLֵ���أ�Ȼ
				�������������ڲ�������ָ����ʽ���ػ��
				�Ͻ�����ִ��Ч�����ڴ�ʹ�ö������롣
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
ͬ�������ֽڣ�ִ���꺯�������Ϸ��ء�
pH			���� Handle
inBuf		Ҫд�����ڵ�����
uLen		Ҫд�����ڵĳ���
����ֵ		��������£�����ֵ = uLen
			���������������ʱ������ֵ < uLen
**********************************************/
UartBufLen UART_Write(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	U8 uPushLen = PushTxBytes(pU,inBuf,uLen);
	//uTxActive ����ڷ����ж��ڲ�����Ҫ���͵ĳ���Ϊ0ʱ����uTxActive
	//��0����������Ҫ�� UTX0IF ��λ�Դ�����һ�ֵķ���
	//��� uTxActive Ϊ1������Push���ֽڻ�����Զ��������ö��⴦��
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