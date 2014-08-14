

#include <iocc2530.h>

#include "PowerAndClock_F.h"






void ClockInit(void)
{
//	CLKCONCMD &= (~0x40);               //设置系统时钟源为32MHZ晶振
//    while(CLKCONSTA & 0x40);          //等待晶振稳定为32M
//    //CLKCONCMD &= ~( 0x47 | (0x04<<3) );               //设置系统主时钟频率为32MHZ
//	CLKCONCMD &= 0xC0;
//	CLKCONCMD |= 0x00 << 3;

	CLKCONCMD =	(0x00 << 7)				//32768 晶振选择 0：外部，1：内部
			|	(0x00 << 6)				//32M/16M 晶振选择 0：外部32M，1：内部16M
			|	(0x00 << 3)				//0-7 定时器主分频
			|	(0x00);					//0-7 CPU分频
	
	//while(CLKCONSTA & 0x40);
	
	while(CLKCONSTA != CLKCONCMD)
	{
	}
}