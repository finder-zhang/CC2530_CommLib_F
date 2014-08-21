
/*************************************************************
�ļ�����			UartInternal_F.h
���ߣ�				Finder
���ڣ�				2014��7��
������				�����ڲ�ͷ�ļ����ڲ�ʹ�ã����ø�ʹ����
*************************************************************/

#ifndef __UART_INTERNAL_F_H__
#define __UART_INTERNAL_F_H__

#include "Uart_F.h"

//��������������������Ը�����Ҫ�ı����С��ֻ���ڱ����ڸģ������������ڸ�
#define U0_RX_BUF_SIZE						(240)
#define U0_TX_BUF_SIZE						(16)


//�ⲿ�� UART_HANDLE �����ڲ�ת���� UART_OPERATOR ���봮����ص�״̬����
typedef struct _UartOperator
{
	U8					uRxBuf[U0_RX_BUF_SIZE];
	U8					uTxBuf[U0_TX_BUF_SIZE];
	
	UartBufLen			uRxHead;				//���ջ��λ���
	UartBufLen			uRxTail;
	UartBufLen			uRxLen;
	
	UartBufLen			uTxHead;				//���ͻ��λ���
	UartBufLen			uTxTail;
	UartBufLen			uTxLen;

	U16					wReadTimeout;			//��ȡ��ʱ
	volatile U16		wReadTimeoutCount;		//��ȡ��ʱ��������wReadTimeoutCount >= wReadTimeout ʱ��ʱ��
	
	volatile U8			uTxActive;				//0�����Ϳ���  1������æ  ���ھ����ȴ����ǿ���һ���µķ���
		
	U8					uNum;					//���ںţ���Χ 1 �� 2
	
	fn_Readed_t			pfnReaded;

}UART_OPERATOR,*PUART_OPERATOR;


//==============================================
extern UART_OPERATOR		u0;
extern UART_OPERATOR		u1;


//==============================================��������ڲ�ʹ��
static U8			IsTxEmpty(UART_HANDLE pH);
static UartBufLen	GetTxRemain(UART_HANDLE pH);
static UartBufLen	PushTxBytes(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);
static U8			PopTxByte(UART_HANDLE pH);
static UartBufLen	_UART_WriteHAL(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);



//==============================================���벿���ڲ�ʹ��
static U8			IsRxEmpty(UART_HANDLE pH);
static U8			PushRxByte(UART_HANDLE pH,U8 uPush);
static U8			IsRxFull(UART_HANDLE pH);
static U8			PopRxByte(UART_HANDLE pH);



//==============================================���벿��
void UART_TimeTick(void);


#endif //__UART_INTERNAL_F_H__