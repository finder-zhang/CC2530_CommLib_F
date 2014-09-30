


#ifndef __DEBUG_F_H__
#define __DEBUG_F_H__

#include "Uart_F.h"

void		DBG_SetDebugHandle(UART_HANDLE uH);			//要使用调试的话，这个函数是一定要先调用的


//==========================================调试输出说明
//以下代码只在 C99 标准或以上使用
//如果只要Debug才输出，请使用	Dprintf
//如果任意时候都要输出，请使用	Aprintf
//extern UART_HANDLE g_uHdbg;

#define _TOSTR(str)		#str
#define TOSTR(str)		_TOSTR(str)


#define Aprintf(...)		Uprintf(__VA_ARGS__)

#ifdef CP_DEBUG
	#define Dprintf(...)			Uprintf(__VA_ARGS__)
	#define Dpvar(var)				Uprintf(TOSTR(var = %d\r\n),var)
	#define Dpxvar(var)				Uprintf(TOSTR(var = %x\r\n),var)
	#define Dpvarc(var,comment)		Uprintf(TOSTR(comment var = %d\r\n),var)
	#define Dpxvarc(var,comment)	Uprintf(TOSTR(comment var = %x\r\n),var)
#else
	#define Dprintf(...)	;
	#define Dpvar(var)		;
#endif //CP_DEBUG


//下面加入各模块的 DEBUG 输出开关






#endif //__DEBUG_F_H__