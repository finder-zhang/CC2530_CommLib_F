

#ifndef __UART_F_H__
#define __UART_F_H__

#include "TypeDef_F.h"

//����Ժ� UART BUFFER �ĳ��ȳ��� 256����� UartBufLen Ϊ U16 ����
typedef		U8				UartBufLen;

//���ھ����Openʱ���س������Ժ����ʱʹ�á�����λ���ļ���������
typedef		void*			UART_HANDLE;



//==============================================���ò���
//void				UART_Init(void);
UART_HANDLE			UART_Open(U8 uNum,U16 wBaudrate);			//���������һ��Ҫ���õ�
void				SetDebugUartHandle(UART_HANDLE uH);			//���������һ��Ҫ���õ�
void				Uprintf(const char* ch,...);



//==============================================�������
UartBufLen			UART_GetTxLen(UART_HANDLE pH);
UartBufLen			UART_Write(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);
void				UART_WriteSync(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);



//==============================================���벿��
UartBufLen			UART_GetRxLen(UART_HANDLE pH);
UartBufLen			UART_Read(UART_HANDLE pH,U8 *outBuf,UartBufLen uLen);





//==========================================�������˵��
//���´���ֻ�� C99 ��׼������ʹ��
//���ֻҪDebug���������ʹ��	Dprintf
//�������ʱ��Ҫ�������ʹ��	Aprintf
//extern UART_HANDLE g_uHdbg;

#define Aprintf(...)		Uprintf(__VA_ARGS__)

#ifdef CP_DEBUG
	#define Dprintf(...)	Uprintf(__VA_ARGS__)
#else
	#define Dprintf(...)	;
#endif //CP_DEBUG


//��������ģ��� DEBUG �������



#endif //__UART_F_H__