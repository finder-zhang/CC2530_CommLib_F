
#include "Includes_F.h"

#include "Gpio_F.h"

void GpioInit(void)
{
	//===========================��� IO �ڳ�ʼ��
//	P0DIR |= 0x80;	
//	P0_7 = 0;
//	
//	P0IE = 1;		//ʹ�� P0 ���ж�
//	P0IEN |= (1 << 6);
//	PICTL |= (1 << 6);
//	P0IFG = 0x00;
	
	//===========================С�� IO �ڳ�ʼ�����жϷ�ʽ
	// P01 P20 �ֱ�������������
//	P0IE = 1;					//P0IE = 1��P0�ж��ܿ���
//	IEN2 |= (1<<1);			//P2IE = 1��P2�ж��ܿ���
//	
//	P0IEN |= (1<<1);			//��ӦP01�ڵ��ж�ʹ��
//	P2IEN |= (1<<0);			//��ӦP20�ڵ��ж�ʹ��
//	
//	PICTL |= (1<<0);			//P0�ڣ��½����ж�
//	PICTL |= (1<<3);			//P2�ڣ��½����ж�
//	
//	P0IFG = 0;					//P0�������ж����
//	P2IFG = 0;					//P2�������ж����
	
	
	
}


__interrupt void Gpio0_ISR(void);
#pragma vector = P0INT_VECTOR
__interrupt void Gpio0_ISR(void)
{	
	static U8 uP0IFG = 0;
	uP0IFG = P0IFG;
	P0IFG = 0;
	P0IF = 0;
	
	if ( uP0IFG & (1 << 1) ) {
	}
}




static U32	_dwS1DnTm = 0;
static U32	_dwS1UpTm = 0;
static U32	_dwS1HoldTm = 0;
static U8	_uS1Status = NO_PRESS;


static U32	_dwS2DnTm = 0;
static U32	_dwS2UpTm = 0;
static U32	_dwS2HoldTm = 0;
static U8	_uS2Status = NO_PRESS;

U8 GPIO_GetS1Status(void)
{
	return _uS1Status;
}

U8 GPIO_GetS2Status(void)
{
	return _uS2Status;
}







__interrupt void Gpio2_ISR(void);
#pragma vector = P2INT_VECTOR
__interrupt void Gpio2_ISR(void)
{	
	//===================�����жϱ�־����һʱ�����жϱ�־
	static U8 uP2IFG;
	uP2IFG = P2IFG;	
	P2IFG = 0;
	P2IF = 0;
	
	return ;
//	if ( PICTL & (1<<3) ) {
//		//����������½����ж�
//		LED3 = 1;
//		osal_start_timerEx(CP_App_TaskID,CP_EVENT_KEY_S2_CNF_DN,50);		
//		return;
//	}
//	
//	if  ( 0 == ( PICTL & (1<<3) ) ) {
//		//����������������ж�
//		osal_set_event(CP_App_TaskID,CP_EVENT_KEY_S2_UP);
//		return;
//	}
	
	
	static U8 uPressComplete = 0;
	if ( uP2IFG & (1<<0) ) {
		if ( 0 == P2_0 ) {
			PICTL &= ~(1<<3);		//P2 ���������ж�
			_dwS2DnTm = GetTickCount();
			uPressComplete = 0;
		}		
		else { 	//( 1 == P2_0 )
			PICTL |= (1<<3);		//P2 ���½����ж�
			_dwS2UpTm = GetTickCount();
			uPressComplete = 1;
		}
		if ( uPressComplete ) {
			uPressComplete = 0;
			
			_dwS2HoldTm = _dwS2UpTm - _dwS2DnTm;
			_uS1Status = NO_PRESS;
			if ( _dwS2HoldTm > 100000L ) {
				_uS2Status = LONG_PRESS_02;
			}
			else if ( _dwS2HoldTm > 50000L ) {
				_uS2Status = LONG_PRESS_01;
			}
			else if ( _dwS2HoldTm > 500 ) {	//����50mS
				_uS2Status = SINGLE_PRESS;
			}
			
			if ( NO_PRESS != _uS2Status ) {
				_dwS2DnTm = _dwS2UpTm;
				//osal_set_event(CP_App_TaskID,CP_EVENT_CHECK_KEY);
			}
		}
	}
	
}