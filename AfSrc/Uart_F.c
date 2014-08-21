
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
		PERCFG = 0x00;					//外设控制寄存器 USART 0的 IO 位置:0为P0口位置1 
		P0SEL = 0x0c;					//P0_2,P0_3用作串口（外设功能）
		P2DIR &= ~0xC0;					//P0优先作为UART0		
		U0CSR |= 0x80;					//设置为UART方式而非
		
		SetBaud(0,wBaudrate);			//设波特率
		
		UTX0IF = 0;             //UART0 TX中断标志初始置位0
		U0CSR |= 0x40;          //允许接收 
		URX0IE = 1;				//开接收中断
		IEN2 |= (1<<2);			//开发送中断		
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


//===========================================下面调试输出专用
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
串口DEBUG专用，从串口0输出格式化字符串，最大长度为64
参数部份与 printf 函数一样
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



