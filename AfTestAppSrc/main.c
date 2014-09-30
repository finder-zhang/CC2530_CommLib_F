
#include <ioCC2530.h>
#include <string.h>
#include "PowerAndClock_F.h"
#include "Uart_F.h"
#include "Timer_F.h"
#include "HalAes_F.h"
#include "hal_aes.h"


UART_HANDLE		g_uH;

void CommRx(void)
{
	U8 ch;
	while ( UART_Read(g_uH,&ch,1) )
	{
		UART_Write(g_uH,&ch,1);
	}
}



int main( void )
{
	ClockInit();
	
	TimerInit();
	
	g_uH = UART_Open(0,115200);
	UART_SetDebugHandle(g_uH);
	//G_SetDebugHandle(g_uH);
	UART_SetReadTimeout(g_uH,500);
	UART_SetReadCallback(g_uH,CommRx);
	
	EA = 1;
	
	Uprintf("12111111111111111111111111111\r\n");
	
	U32 dwi = 0;
	static U8 uRxBuf[128];
	
	while(1)
	{
		//DelayMs(1000);
		//Uprintf("abc");		
		//UART_Poll();
		
		Uprintf("abc %d\r\n",dwi++);
		//DelayMs(1000);		
		//int iLen = UART_Read(g_uH,uRxBuf,128);
		int iLen = UART_ReadTimeout(g_uH,uRxBuf,10,5000);
		//uRxBuf[127] = 0;
		Uprintf("rx = [%s],iLen = %d\r\n",uRxBuf,iLen);
		memset(uRxBuf,0,sizeof(uRxBuf));
		//Uprintf("rx iLen = %d\r\n",iLen);
	};
	
	//return 0;
}
