

#ifndef __UART_F_H__
#define __UART_F_H__

#include "TypeDef_F.h"

//如果以后 UART BUFFER 的长度超过 256，则改 UartBufLen 为 U16 类型
typedef		U8				UartBufLen;

//串口句柄，Open时返回出来，以后操作时使用。与上位机文件操作类似
typedef		void*			UART_HANDLE;

//==============================================回调函数定义
typedef void (* fn_Readed_t)(void);		//读取一段内容已经完成


//==============================================共用部份
//void				UART_Init(void);
UART_HANDLE			UART_Open(U8 uNum,U16 wBaudrate);			//这个函数是一定要调用的
void				UART_SetDebugHandle(UART_HANDLE uH);			//这个函数是一定要调用的
void				UART_SetReadTimeout(UART_HANDLE uH,U16 wTimeout);	//读取时的超时，uTimeout以0.1毫秒为单位
void				UART_SetReadCallback(UART_HANDLE uH,fn_Readed_t pfnReaded);
void				UART_Poll(void);
void				Uprintf(const char* ch,...);



//==============================================输出部份
UartBufLen			UART_GetTxLen(UART_HANDLE pH);
void				UART_Write(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);



//==============================================输入部份
UartBufLen			UART_GetRxLen(UART_HANDLE pH);
UartBufLen			UART_Read(UART_HANDLE pH,U8 *outBuf,UartBufLen uLen);







//==========================================调试输出说明
//以下代码只在 C99 标准或以上使用
//如果只要Debug才输出，请使用	Dprintf
//如果任意时候都要输出，请使用	Aprintf
//extern UART_HANDLE g_uHdbg;

#define _TOSTR(str)		#str
#define TOSTR(str)		_TOSTR(str)
#define Dpvar(var)		Uprintf(TOSTR(var = %d\r\n),var)

#define Aprintf(...)		Uprintf(__VA_ARGS__)

#ifdef CP_DEBUG
	#define Dprintf(...)	Uprintf(__VA_ARGS__)
	#define Dpvar(var)		Uprintf(TOSTR(var = %d\r\n),var)
	#define Dpxvar(var)		Uprintf(TOSTR(var = %x\r\n),var)
#else
	#define Dprintf(...)	;
	#define Dpvar(var)		;
#endif //CP_DEBUG


//下面加入各模块的 DEBUG 输出开关



#endif //__UART_F_H__