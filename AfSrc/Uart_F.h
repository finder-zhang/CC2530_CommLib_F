

#ifndef __UART_F_H__
#define __UART_F_H__

#include "TypeDef_F.h"

//如果以后 UART BUFFER 的长度超过 256，则改 UartBufLen 为 U16 类型
typedef		U8				UartBufLen;

//串口句柄，Open时返回出来，以后操作时使用。与上位机文件操作类似
typedef		void*			UART_HANDLE;

//==============================================回调函数定义
typedef void (* fnRxHandler_t)(void);		//读取一段内容已经完成

typedef enum _ReadMode
{
	RMODE_AUTO	= 0,
	RMODE_ASYNC,
	RMODE_SYNC,
	RMODE_MAX_IDX = RMODE_SYNC
}ReadMode;


//==============================================共用部份
//void				UART_Init(void);
UART_HANDLE			UART_Open(U8 uNum,U32 wBaudrate);			//这个函数是一定要调用的
BOOL				UART_SetDebugHandle(UART_HANDLE uH);			//这个函数是一定要调用的
BOOL				UART_SetReadTimeout(UART_HANDLE uH,U16 wTimeoutMilliSeconds);	//读取时的超时，uTimeout以0.1毫秒为单位
BOOL				UART_SetReadMode(UART_HANDLE uH,ReadMode mode);
fnRxHandler_t		UART_SetReadCallback(UART_HANDLE uH,fnRxHandler_t pfnReaded);
void				UART_Poll(void);
void				Uprintf(const char* ch,...);



//==============================================输出部份
UartBufLen			UART_GetTxLen(UART_HANDLE pH);
void				UART_Write(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);



//==============================================输入部份
UartBufLen			UART_GetRxLen(UART_HANDLE pH);

//尝试读入uLen长度的数据，如果长度不足，有多少读多少。读完马上返回
UartBufLen			UART_Read(UART_HANDLE pH,U8 *outBuf,UartBufLen uLen);

//带超时退出的同步读函数，与 UART_Read 相比，当未读到足够的长度时，并不退出，
//而是等到 dwMilliSeconds 毫秒的超时到达后，才退出，
//如果能读到指定的长度，也马上退出。
UartBufLen			UART_ReadTimeout(UART_HANDLE pH,U8* outBuf,UartBufLen uLen,U32 dwMilliSeconds);






#endif //__UART_F_H__