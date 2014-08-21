
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
		

UART_HANDLE			UART_Open(U8 uNum,U16 wBaudrate)
{
	switch (uNum) {
	case 0:
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
		return &u1;
		break;
	default:
		return NULL;
		break;
	};
}


//===========================================����������ר��
UART_HANDLE g_uHdbg = 0;

void UART_SetDebugHandle(UART_HANDLE uH)
{
	g_uHdbg = uH;
}


void UART_SetReadTimeout(UART_HANDLE uH,U16 wTimeout)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)uH;
	pU->wReadTimeout = wTimeout;
}

void UART_SetReadCallback(UART_HANDLE uH,fn_Readed_t pfnReaded)
{
	PUART_OPERATOR pU = (PUART_OPERATOR)uH;
	pU->pfnReaded = pfnReaded;
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



