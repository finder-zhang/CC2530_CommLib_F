
#include "Includes_F.h"

#include "KeyCheck_F.h"


static U8 _bCheckKey;

//===========================================================
//按钮相关变量
	U8 a_uKeyDown[KEYNUM] = {0};
	U8 a_uKeyDownCount[KEYNUM] = {0};
	U8 a_uKeyAction[KEYNUM] = {0};
	U8 a_uKeyValue[KEYNUM] = {0};
	U8 m_uKeyNum = 0;
//按钮变量结束
//------------------------------------------------------------

	
PFN_TimeToCheckKeyCB g_fnTimeToCheckKey;
	
void KeyCheckInit(void)
{
	_bCheckKey = 0;
}

void SetKeyCheckFlag(U8 uFlag)
{
	_bCheckKey = uFlag;
}

U8 GetKeyCheckFlag(void)
{
	return _bCheckKey;
}
	

U8 KeyCheck(U8 Key)
{
	U8	uKeyValue = 0;

	if(!Key)	//键有按下
	{
		if(a_uKeyDown[m_uKeyNum] == 0)	//如果是刚按下，则准备进入计时
		{
			a_uKeyDown[m_uKeyNum] = 1;		//到下一次进来时，可以知道之前有按下
			a_uKeyDownCount[m_uKeyNum] = 1;		//计数值加到1
		}
		else	//之前已经有按下，现在计时
		{
			if(a_uKeyDownCount[m_uKeyNum] > CHKTIME)
			{
				a_uKeyValue[m_uKeyNum] = KEYHOLD;
			}
			else if((a_uKeyDownCount[m_uKeyNum] < CHKTIME))
			{
				a_uKeyDownCount[m_uKeyNum]++;	//已按下时间累加
			}

			if(a_uKeyDownCount[m_uKeyNum] == CHKTIME)
			{
				a_uKeyAction[m_uKeyNum] = 1;		//按下时间够长了，ACTION生效
				a_uKeyDownCount[m_uKeyNum]++;
				a_uKeyValue[m_uKeyNum] = KEYDOWN;
			}
		}
	}
	else	//键弹起了
	{
		if(a_uKeyAction[m_uKeyNum] == 0)		//之前未有ACTION
		{										//全部统计数清零
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			a_uKeyValue[m_uKeyNum] = KEYNULL;
		}
		else	//之前已有ACTION，按键起了
		{
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			a_uKeyAction[m_uKeyNum] = 0;
			a_uKeyValue[m_uKeyNum] = KEYUP;		//全部清零，返回1值
		}
	}

	uKeyValue = a_uKeyValue[m_uKeyNum];

	m_uKeyNum++;
	if(m_uKeyNum >= KEYNUM)
	{
		m_uKeyNum = 0;
	}
	return uKeyValue;
}


/*
U8 a_uKeyDown[KEYNUM]={0,0,0,0};
U8 a_uKeyDownCount[KEYNUM]={0,0,0,0};
U8 a_uKeyAction[KEYNUM]={0,0,0,0};
U8 m_u1KeyValue = 0;
U8 m_uKeyNum = 0;

U8 KeyCheck(U8 Key)
{
	if(Key)	//键有按下
	{
		if(a_uKeyDown[m_uKeyNum] == 0)	//如果是刚按下，则准备进入计时
		{
			a_uKeyDown[m_uKeyNum] = 1;
			a_uKeyDownCount[m_uKeyNum] = 1;
		}
		else	//之前已经有按下，现在计时
		{
			if(a_uKeyDownCount[m_uKeyNum] >= CHKTIME)
			{
				a_uKeyAction[m_uKeyNum] = 1;		//按下时间够长了，ACTION生效
			}
			else
			{
				a_uKeyDownCount[m_uKeyNum]++;	//已按下时间累加
			}
		}
		m_u1KeyValue = 0;	//按下时一定返回0值，因为要放手才动作
	}
	else	//键弹起了
	{
		if(a_uKeyAction[m_uKeyNum] == 0)		//之前未有ACTION
		{										//全部统计数清零
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			m_u1KeyValue = 0;
		}
		else	//之前已有ACTION，按键起了
		{
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			a_uKeyAction[m_uKeyNum] = 0;
			m_u1KeyValue = 1;		//全部清零，返回1值
		}
	}
	m_uKeyNum++;
	if(m_uKeyNum >= KEYNUM)
	{
		m_uKeyNum = 0;
	}
	return m_u1KeyValue;
}
*/


static U32	_dwS2DnTm = 0;
static U32	_dwS2UpTm = 0;
static U32	_dwS2HoldTm = 0;
static U8	_uS2Status = NO_PRESS;

void PressConfirm(void)
{
	_dwS2DnTm = GetTickCount();
	PICTL &= ~(1<<3);
}

U8 CheckPressWhenUp(void)
{
	PICTL |= (1<<3);
	
	_dwS2UpTm = GetTickCount();
	_dwS2HoldTm = _dwS2UpTm - _dwS2DnTm;
	
	_uS2Status = NO_PRESS;
	if ( _dwS2HoldTm > 100000L ) {
		return LONG_PRESS_02;
	}
	else if ( _dwS2HoldTm > 50000L ) {
		return LONG_PRESS_01;
	}
	else if ( _dwS2HoldTm > 500 ) {	//大于50mS
		return SINGLE_PRESS;
	}
	return NO_PRESS;
}