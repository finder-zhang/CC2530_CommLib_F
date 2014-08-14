

#include <iocc2530.h>

#include "PowerAndClock_F.h"






void ClockInit(void)
{
//	CLKCONCMD &= (~0x40);               //����ϵͳʱ��ԴΪ32MHZ����
//    while(CLKCONSTA & 0x40);          //�ȴ������ȶ�Ϊ32M
//    //CLKCONCMD &= ~( 0x47 | (0x04<<3) );               //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ
//	CLKCONCMD &= 0xC0;
//	CLKCONCMD |= 0x00 << 3;

	CLKCONCMD =	(0x00 << 7)				//32768 ����ѡ�� 0���ⲿ��1���ڲ�
			|	(0x00 << 6)				//32M/16M ����ѡ�� 0���ⲿ32M��1���ڲ�16M
			|	(0x00 << 3)				//0-7 ��ʱ������Ƶ
			|	(0x00);					//0-7 CPU��Ƶ
	
	//while(CLKCONSTA & 0x40);
	
	while(CLKCONSTA != CLKCONCMD)
	{
	}
}