


#ifndef __DEBUG_F_H__
#define __DEBUG_F_H__

#include "Uart_F.h"

void		DBG_SetDebugHandle(UART_HANDLE uH);			//Ҫʹ�õ��ԵĻ������������һ��Ҫ�ȵ��õ�


//==========================================�������˵��
//���´���ֻ�� C99 ��׼������ʹ��
//���ֻҪDebug���������ʹ��	Dprintf
//�������ʱ��Ҫ�������ʹ��	Aprintf
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


//��������ģ��� DEBUG �������






#endif //__DEBUG_F_H__