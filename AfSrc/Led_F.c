


#include "Includes_F.h"

#include "Led_F.h"







void LedInit(void)
{
	//==========================���� LED ����
	//
//	P1DIR |= 0x13;         //P1.0��P1.1��P1.4����Ϊ���
//	P0DIR |= 0x02;         //P0.1����Ϊ���
	
	
	//==========================��С��� LED ����
	P1DIR |= 0x13;		//P1 0,1,4 ΪLED 1 2 3
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