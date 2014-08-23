

#ifndef __UART_F_H__
#define __UART_F_H__

#include "TypeDef_F.h"

//����Ժ� UART BUFFER �ĳ��ȳ��� 256����� UartBufLen Ϊ U16 ����
typedef		U8				UartBufLen;

//���ھ����Openʱ���س������Ժ����ʱʹ�á�����λ���ļ���������
typedef		void*			UART_HANDLE;

//==============================================�ص���������
typedef void (* fn_Readed_t)(void);		//��ȡһ�������Ѿ����


//==============================================���ò���
//void				UART_Init(void);
UART_HANDLE			UART_Open(U8 uNum,U16 wBaudrate);			//���������һ��Ҫ���õ�
void				UART_SetDebugHandle(UART_HANDLE uH);			//���������һ��Ҫ���õ�
void				UART_SetReadTimeout(UART_HANDLE uH,U16 wTimeout);	//��ȡʱ�ĳ�ʱ��uTimeout��0.1����Ϊ��λ
void				UART_SetReadCallback(UART_HANDLE uH,fn_Readed_t pfnReaded);
void				UART_Poll(void);
void				Uprintf(const char* ch,...);



//==============================================�������
UartBufLen			UART_GetTxLen(UART_HANDLE pH);
void				UART_Write(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);



//==============================================���벿��
UartBufLen			UART_GetRxLen(UART_HANDLE pH);
UartBufLen			UART_Read(UART_HANDLE pH,U8 *outBuf,UartBufLen uLen);







//==========================================�������˵��
//���´���ֻ�� C99 ��׼������ʹ��
//���ֻҪDebug���������ʹ��	Dprintf
//�������ʱ��Ҫ�������ʹ��	Aprintf
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


//��������ģ��� DEBUG �������



#endif //__UART_F_H__