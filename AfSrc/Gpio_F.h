


#ifndef __GPIO_F_H__
#define __GPIO_F_H__

#include "TypeDef_F.h"

#define KEY1	P0_5       //����P0.5��ΪS2���ƶ�


#define KEYS1	P0_1
#define KEYS2	P2_0



void GpioInit(void);


U8 GPIO_GetS1Status(void);
U8 GPIO_GetS2Status(void);










#endif //__GPIO_F_H__