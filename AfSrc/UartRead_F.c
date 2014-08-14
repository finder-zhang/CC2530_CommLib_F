


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





//=============================================����0�����ж�
__interrupt void Uart0Rx_ISR(void);
#pragma vector = URX0_VECTOR 
__interrupt void Uart0Rx_ISR(void) 
{ 
    URX0IF = 0;       // ���жϱ�־
	PushRxByte(&u0,U0DBUF);
}


//=============================================����1�����ж�
__interrupt void Uart1Rx_ISR(void);
#pragma vector = URX1_VECTOR 
__interrupt void Uart1Rx_ISR(void) 
{ 
    URX1IF = 0;       // ���жϱ�־
	PushRxByte(&u1,U1DBUF);
}



//============================================�ڲ�����
static U8			IsRxEmpty(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	return ( 0 == pU->uRxLen );
}


//ѹ��һ���ֽڣ��������ѹ�����򷵻� FALSE
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


//�Ƴ�һ���ֽڣ����Ϊ�գ����Ƴ�0xFF����ʹ��ǰ����鿴�������Ƿ�Ϊ��
//�����޷��ж��Ƴ��Ƿ�ʧ�ܡ�
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





//=============================================�ⲿ�ӿں���

//�󳤶�
UartBufLen	UART_GetRxLen(UART_HANDLE pH)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)pH;
	return pU->uRxLen;
}

//��õĶ�ȡ����
//pH		Ҫ���Ĵ��ھ��
//outBuf	�������ֽڣ��뱣֤outBuf�ĳ��� >= uLen�������ƻ��ڴ�
//uLen		Ҫ���ĳ���
//����ֵ	�����ܶ������ֽڳ���
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