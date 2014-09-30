
#include <iocc2530.h>

//#include "Includes_F.h"

#include "Timer_F.h"

#include "UartInternal_F.h"


static volatile U32 _TimeTick = 0;		//ϵͳʱ�䣬��λΪ ʮ��֮һ ����

U32 GetTickCount(void)
{
	return _TimeTick;
}

void TimerInit(void)
{
//============================================================
//												��ʱ��3����
	T3CTL = (0x07 << 5)		//��Ƶ��
		|   (0x00 << 3)		//����жϣ�����Ƚϣ�ֻҪ��0xFF����0���ж�
		|	(0x02);			//ѭ�����ӹ���ģʽ

	T3CCTL0 |= 0x04;		//ʹ�ñȽ�ģʽ
	//T3CC0 = 24;
	T3CC0 = 24;				//128��Ƶʱ��4uS��1��25�¸պ�0.1mS
    T3CTL |= 0x10;           //����	
	T3IE = 1;                //��T3�ж�
	
	
	
//============================================================
//												��ʱ��4����
//	T4CTL = (0x07 << 5)		//��Ƶ��
//		|   (0x00 << 3)		//������ж�
//		|	(0x00);			//ѭ�����ӹ���ģʽ
//	
//	T4CCTL0 =	(0x01 << 6)	//ͨ��0�ж�  0�������Σ�1������
//		|		(0x00 << 3)	//�Ƚ����ģʽ
//		|		(0x00 << 2)	//ģʽ 0������1���Ƚ�
//		|		(0x01 << 1)	//���� 1���½��ز���
//		|		(0x01 << 0);//���� 1�������ز���
	
//	T4CCTL0 |= 0x04;
	
//	P2SEL |= 0x01;			//P20ָ��ʱ��4������		
//	PERCFG |= 0x10;			//Timer4�ⲿ����ʹ�ñ���λ��2
	
//	T4CC0 = 249;				//128��Ƶʱ��4uS��1��250�¸պ�1mS
//	T4CTL |= 0x10;           //����	
//	T4IE = 1;                //��T4�ж�	
}


//This function can success even _TimeTick is overflow.
//delay must base on Timer3's _TimeTick count,
//_TimeTick 's unit is 0.1mS
void DelayMs(U16 wTime)
{
	U32 tmp = _TimeTick;
	tmp += wTime*10;
	while(tmp != _TimeTick){ }
}


//__interrupt void Timer1_ISR(void);
//#pragma vector = T1_VECTOR
//__interrupt void Timer1_ISR(void)
//{
//}


__interrupt void Timer3_ISR(void);
#pragma vector = T3_VECTOR
__interrupt void Timer3_ISR(void)
{
	static U8 _count = 0;
	//=======================================
	//						  ϵͳ1mS��ʱʹ��
	++_TimeTick;
	UART_TimeTick();
	
	//=======================================
	//							ÿ5mSִ��һ��
	++_count;
	if ( _count > 49 ){
		_count = 0;
		//SetKeyCheckFlag((U8)1);
		
//		if (g_fnTimeToCheckKey) {
//			g_fnTimeToCheckKey();
//		}
		//osal_set_event(CP_App_TaskID,CP_EVENT_CHECK_KEY);
	}
	
//	if ( T3CH0IF ) {
//		T3CH0IF = 0;
//	}	
	
}



//#pragma vector = T4_VECTOR
//__interrupt void Timer4_ISR(void)
//{
//	if ( T4CH0IF ) {
//		T4CH0IF = 0;
//		//T4CC0 = 252;
////		LED3 = 1;
////		LED2 = 1;
////		LED1 = 1;
////		LED4 = 1;
//		//Uprintf(&u0,"aa\r\n");
//	}
//	
//	//T4CH1IF = 0;
//	
//	
//	LED4 ^= 1;
//}


void TMR_Poll(void)
{
	UART_Poll();
}