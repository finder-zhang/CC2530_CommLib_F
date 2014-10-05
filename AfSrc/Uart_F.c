
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <iocc2530.h>

#include "TypeDef_F.h"
#include "CommonDef_F.h"
#include "Uart_F.h"
#include "UartInternal_F.h"


UART_OPERATOR		u0;
UART_OPERATOR		u1;

static void _InitUartOperator(PUART_OPERATOR pU)
{
	memset(pU->uRxBuf,0,U0_RX_BUF_SIZE);
	memset(pU->uTxBuf,0,U0_TX_BUF_SIZE);
	
	pU->uRxHead = 0;
	pU->uRxTail = 0;
	pU->uRxLen = 0;
	
	pU->uTxHead = 0;				//���ͻ��λ���
	pU->uTxTail = 0;
	pU->uTxLen = 0;

	pU->wReadTimeoutMilliSeconds = 0;	//��ȡ��ʱ
	pU->wReadTimeoutCount = 0;		//��ȡ��ʱ��������wReadTimeoutCount >= wReadTimeout ʱ��ʱ��
	
	pU->uTxActive = FALSE;			//0�����Ϳ���  1������æ  ���ھ����ȴ����ǿ���һ���µķ���
		
	pU->uNum = 0;					//���ںţ���Χ 1 �� 2
	
	pU->bReading = FALSE;
	pU->eReadMode = RMODE_AUTO;
	
	pU->pfnReaded = NULL;
}



#define SetUxBAUD_E(x,value)		do{ U##x##GCR &= 0xF0;U##x##GCR |= value; }while(0)
#define SetUxBAUD_M(x,value)		do{ U##x##BAUD = value; }while(0)

#define SetBaud(x,baud)\
	switch (baud) {\
	case 115200:\
		SetUxBAUD_E(x,11);\
		SetUxBAUD_M(x,216);\
		break;\
	case 9600:\
		SetUxBAUD_E(x,8);\
		SetUxBAUD_M(x,59);\
		break;\
	default:\
		return NULL;\
		break;\
	}

U8 SetBaud_(U8 uNum,U32 dwBaudrate)
{
	switch (dwBaudrate) {
	case 115200:
		break;
	case 9600:
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}
		

UART_HANDLE			UART_Open(U8 uNum,U32 wBaudrate)
{
	switch (uNum) {
	case 0:
		_InitUartOperator(&u0);
		
		PERCFG = 0x00;					//������ƼĴ��� USART 0�� IO λ��:0ΪP0��λ��1
		P0SEL = 0x0c;					//P0_2,P0_3�������ڣ����蹦�ܣ�
		P2DIR &= ~0xC0;					//P0������ΪUART0		
		U0CSR |= 0x80;					//����ΪUART��ʽ����
		
		SetBaud(0,wBaudrate);			//�貨����
		
		UTX0IF = 0;             //UART0 TX�жϱ�־��ʼ��λ0
		U0CSR |= 0x40;          //�������
		URX0IE = 1;				//�������ж�
		IEN2 |= (1<<2);			//�������ж�		
		return &u0;
		break;
		
	case 1:
		_InitUartOperator(&u1);
		return &u1;
		break;
	default:
		return NULL;
		break;
	};
}


//===========================================����������ר��
UART_HANDLE g_uHdbg = 0;

BOOL UART_SetDebugHandle(UART_HANDLE uH)
{
	if ( NULL == uH ) {
		return FALSE;
	}
	g_uHdbg = uH;
	return TRUE;
}


BOOL UART_SetReadTimeout(UART_HANDLE uH,U16 wTimeoutMilliSeconds)
{
	if ( NULL == uH ) {
		return FALSE;
	}
	PUART_OPERATOR pU = (PUART_OPERATOR)uH;
	pU->wReadTimeoutMilliSeconds = wTimeoutMilliSeconds;
	return TRUE;
}

BOOL UART_SetReadMode(UART_HANDLE uH,ReadMode mode)
{
	if ( NULL == uH ) {
		return FALSE;
	}
	if ( mode > RMODE_MAX_IDX ) {
		return FALSE;
	}
	PUART_OPERATOR pU = (PUART_OPERATOR)uH;
	pU->eReadMode = mode;
	return TRUE;
}

fnRxHandler_t UART_SetReadCallback(UART_HANDLE uH,fnRxHandler_t pfnReaded)
{
	if ( NULL == uH ) {
		return (fnRxHandler_t)-1;
	}
	PUART_OPERATOR pU = (PUART_OPERATOR)uH;
	fnRxHandler_t fnTemp = pU->pfnReaded;
	pU->pfnReaded = pfnReaded;
	return fnTemp;
}




/********************************************************
����DEBUGר�ã��Ӵ���0�����ʽ���ַ�������󳤶�Ϊ64
���������� printf ����һ��
********************************************************/
#define MAX_FORMAT_CHAR						(128)
static char _uFormatBuf[128];
void Uprintf(const char* ch,...)
{
	static int iLen;
//	static char tmp[MAX_FORMAT_CHAR];
	va_list ap;
	va_start(ap,ch);	
	iLen = vsprintf(_uFormatBuf,ch,ap);
//	UART_Write(pH,(U8*)_uFormatBuf,iLen);
	UART_Write(g_uHdbg,(U8*)_uFormatBuf,iLen);
	va_end(ap);
}
//------------------------------------------------



//======================================= C99
#define UartPrintf(...) \
	do\
	{\
		sprintf(_uFormatBuf,__VA_ARGS__);\
		UART_Write((UART_HANDLE)&u0,(U8*)_uFormatBuf,strlen(_uFormatBuf));\
	}while(0)



