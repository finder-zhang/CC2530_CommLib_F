
#include "Includes_F.h"

#include "Gpio_F.h"

void GpioInit(void)
{
	//===========================大板 IO 口初始化
//	P0DIR |= 0x80;	
//	P0_7 = 0;
//	
//	P0IE = 1;		//使能 P0 口中断
//	P0IEN |= (1 << 6);
//	PICTL |= (1 << 6);
//	P0IFG = 0x00;
	
	//===========================小板 IO 口初始化，中断方式
	// P01 P20 分别连接两个按键
//	P0IE = 1;					//P0IE = 1，P0中断总开关
//	IEN2 |= (1<<1);			//P2IE = 1，P2中断总开关
//	
//	P0IEN |= (1<<1);			//对应P01口的中断使能
//	P2IEN |= (1<<0);			//对应P20口的中断使能
//	
//	PICTL |= (1<<0);			//P0口，下降沿中断
//	PICTL |= (1<<3);			//P2口，下降沿中断
//	
//	P0IFG = 0;					//P0口已有中断清除
//	P2IFG = 0;					//P2口已有中断清除
	
	
	
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
	//===================保存中断标志，第一时间清中断标志
	static U8 uP2IFG;
	uP2IFG = P2IFG;	
	P2IFG = 0;
	P2IF = 0;
	
	return ;
//	if ( PICTL & (1<<3) ) {
//		//如果现在是下降沿中断
//		LED3 = 1;
//		osal_start_timerEx(CP_App_TaskID,CP_EVENT_KEY_S2_CNF_DN,50);		
//		return;
//	}
//	
//	if  ( 0 == ( PICTL & (1<<3) ) ) {
//		//如果现在是上升沿中断
//		osal_set_event(CP_App_TaskID,CP_EVENT_KEY_S2_UP);
//		return;
//	}
	
	
	static U8 uPressComplete = 0;
	if ( uP2IFG & (1<<0) ) {
		if ( 0 == P2_0 ) {
			PICTL &= ~(1<<3);		//P2 改上升沿中断
			_dwS2DnTm = GetTickCount();
			uPressComplete = 0;
		}		
		else { 	//( 1 == P2_0 )
			PICTL |= (1<<3);		//P2 改下降沿中断
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
			else if ( _dwS2HoldTm > 500 ) {	//大于50mS
				_uS2Status = SINGLE_PRESS;
			}
			
			if ( NO_PRESS != _uS2Status ) {
				_dwS2DnTm = _dwS2UpTm;
				//osal_set_event(CP_App_TaskID,CP_EVENT_CHECK_KEY);
			}
		}
	}
	
}