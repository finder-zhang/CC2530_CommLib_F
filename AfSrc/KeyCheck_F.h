
#ifndef __KEY_CHECK_H__
#define __KEY_CHECK_H__

//#include "Incs.h"


void KeyCheckInit(void);
void SetKeyCheckFlag(U8 uFlag);
U8 GetKeyCheckFlag(void);
U8 KeyCheck(U8 Key);

typedef void (*PFN_TimeToCheckKeyCB)(void);

extern PFN_TimeToCheckKeyCB g_fnTimeToCheckKey;

//---------------------------------------
//与按键控制有关的定义
	#define KEYNUM	2			//共有多少个按钮
//	#define CHKTIME 3
	#define CHKTIME 4			//消抖时检测的次数
//	#define CHKTIME	12
	#define KEYNULL	0			//按钮的状态
	#define KEYDOWN	1
	#define KEYUP	2
	#define KEYHOLD	3
//与按键控制有关的定义结束
//---------------------------------------

enum _PRESS_STATUS
{
	NO_PRESS,
	SINGLE_PRESS,
	LONG_PRESS_01,
	LONG_PRESS_02
};

void PressConfirm(void);
U8 CheckPressWhenUp(void);







#endif //__KEY_CHECK_H__
