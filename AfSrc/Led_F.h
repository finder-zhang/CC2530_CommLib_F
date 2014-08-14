

#ifndef __LED_F_H__
#define __LED_F_H__

#define LED1 P1_0       //����P1.0��ΪLED1���ƶ�
#define LED2 P1_1       //����P1.1��ΪLED2���ƶ�
#define LED3 P1_4       //����P1.4��ΪLED3���ƶ�
#define LED4 P0_1       //����P0.1��ΪLED4���ƶ�

void LedInit(void);

void SetLed1(U8 uOn);
U8 GetLed1(void);
void ToggleLed1(void);












#endif //__LED_F_H__