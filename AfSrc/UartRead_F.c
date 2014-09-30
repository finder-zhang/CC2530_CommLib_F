


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



//=============================================����0�����ж�
__interrupt void Uart0Rx_ISR(void);
#pragma vector = URX0_VECTOR
__interrupt void Uart0Rx_ISR(void)
{
    URX0IF = 0;       // ���жϱ�־
	PushRxByte(&u0,U0DBUF);
	u0.wReadTimeoutCount = 0;
}


//=============================================����1�����ж�
__interrupt void Uart1Rx_ISR(void);
#pragma vector = URX1_VECTOR
__interrupt void Uart1Rx_ISR(void)
{
    URX1IF = 0;       // ���жϱ�־
	PushRxByte(&u1,U1DBUF);
	u1.wReadTimeoutCount = 0;
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
	}
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





//=============================================�ⲿ�ӿں���

//ʱ��ˢ�£��ú�����0.1��Ϊ����ִ�У��ɶ�ʱ������ִ��
//���ɳ�ʱ��ִ��
void UART_TimeTick()
{
	if (u0.wReadTimeoutCount < u0.wReadTimeout) {
		++u0.wReadTimeoutCount;
	}
	if (u1.wReadTimeoutCount < u1.wReadTimeout) {
		++u0.wReadTimeoutCount;
	}
}

//���ڼ�⣬�ú�������ѭ�������Եص���
void UART_Poll(void)
{
	//�����ʱʱ�䵽�������г��ȣ���ִ�ж���ʱ
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


UartBufLen			UART_ReadTimeout(UART_HANDLE pH,U8* outBuf,UartBufLen uLen,U32 dwMilliSeconds)
{
	//��ʼʱ�䣬����ʱ�䣬���ʱ��ȱ��������ڼ�ⳬʱ�˳�
	U32 dwTickStart = GetTickCount();
	U32 dwTickTimeout = dwMilliSeconds * 10;
	U32 dwTickEnd = dwTickStart + dwTickTimeout;
	U32 dwTickCurr = dwTickStart;
	
	//ʱ�� Tick Ϊ32λ����������ܣ��ȼ����Ƿ������
	BOOL bOverflow = dwTickEnd < dwTickStart;	//Check is TimeTick rollback
	
	UartBufLen uReaded = 0;
	
	while( uReaded < uLen )
	{
		uReaded += UART_Read(pH,outBuf + uReaded,uLen - uReaded);
		dwTickCurr = GetTickCount();
		if ( bOverflow ) {		//�����ʱ���ж�
			if ( (dwTickCurr >= dwTickEnd) && (dwTickCurr < dwTickStart) ) {
				break;
			}
		}
		else {		//û���ʱ
			if ( dwTickCurr >= dwTickEnd ) {
				break;
			}
		}
	}
	
	return uReaded;
}