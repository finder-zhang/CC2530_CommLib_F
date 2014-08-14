


#include "Includes_F.h"

#include "Led_F.h"







void LedInit(void)
{
	//==========================大板的 LED 设置
	//
//	P1DIR |= 0x13;         //P1.0、P1.1、P1.4定义为输出
//	P0DIR |= 0x02;         //P0.1定义为输出
	
	
	//==========================新小板的 LED 设置
	P1DIR |= 0x13;		//P1 0,1,4 为LED 1 2 3
}

void SetLed1(U8 uOn)
{
	LED1 = uOn;
}

U8 GetLed1(void)
{
	return LED1;
}

void ToggleLed1(void)
{
	LED1 ^= 1;
}