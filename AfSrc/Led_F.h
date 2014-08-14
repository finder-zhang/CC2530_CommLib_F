

#ifndef __LED_F_H__
#define __LED_F_H__

#define LED1 P1_0       //定义P1.0口为LED1控制端
#define LED2 P1_1       //定义P1.1口为LED2控制端
#define LED3 P1_4       //定义P1.4口为LED3控制端
#define LED4 P0_1       //定义P0.1口为LED4控制端

void LedInit(void);

void SetLed1(U8 uOn);
U8 GetLed1(void);
void ToggleLed1(void);












#endif //__LED_F_H__