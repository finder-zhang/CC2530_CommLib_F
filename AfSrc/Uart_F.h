

#ifndef __UART_F_H__
#define __UART_F_H__

#include "TypeDef_F.h"

//����Ժ� UART BUFFER �ĳ��ȳ��� 256����� UartBufLen Ϊ U16 ����
typedef		U8				UartBufLen;

//���ھ����Openʱ���س������Ժ����ʱʹ�á�����λ���ļ���������
typedef		void*			UART_HANDLE;

//==============================================�ص���������
typedef void (* fnRxHandler_t)(void);		//��ȡһ�������Ѿ����

typedef enum _ReadMode
{
	RMODE_AUTO	= 0,
	RMODE_ASYNC,
	RMODE_SYNC,
	RMODE_MAX_IDX = RMODE_SYNC
}ReadMode;


//==============================================���ò���
//void				UART_Init(void);
UART_HANDLE			UART_Open(U8 uNum,U32 wBaudrate);			//���������һ��Ҫ���õ�
BOOL				UART_SetDebugHandle(UART_HANDLE uH);			//���������һ��Ҫ���õ�
BOOL				UART_SetReadTimeout(UART_HANDLE uH,U16 wTimeoutMilliSeconds);	//��ȡʱ�ĳ�ʱ��uTimeout��0.1����Ϊ��λ
BOOL				UART_SetReadMode(UART_HANDLE uH,ReadMode mode);
fnRxHandler_t		UART_SetReadCallback(UART_HANDLE uH,fnRxHandler_t pfnReaded);
void				UART_Poll(void);
void				Uprintf(const char* ch,...);



//==============================================�������
UartBufLen			UART_GetTxLen(UART_HANDLE pH);
void				UART_Write(UART_HANDLE pH,const U8 * inBuf,UartBufLen uLen);



//==============================================���벿��
UartBufLen			UART_GetRxLen(UART_HANDLE pH);

//���Զ���uLen���ȵ����ݣ�������Ȳ��㣬�ж��ٶ����١��������Ϸ���
UartBufLen			UART_Read(UART_HANDLE pH,U8 *outBuf,UartBufLen uLen);

//����ʱ�˳���ͬ������������ UART_Read ��ȣ���δ�����㹻�ĳ���ʱ�������˳���
//���ǵȵ� dwMilliSeconds ����ĳ�ʱ����󣬲��˳���
//����ܶ���ָ���ĳ��ȣ�Ҳ�����˳���
UartBufLen			UART_ReadTimeout(UART_HANDLE pH,U8* outBuf,UartBufLen uLen,U32 dwMilliSeconds);






#endif //__UART_F_H__