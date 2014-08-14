
#include "Includes_F.h"

#include "KeyCheck_F.h"


static U8 _bCheckKey;

//===========================================================
//��ť��ر���
	U8 a_uKeyDown[KEYNUM] = {0};
	U8 a_uKeyDownCount[KEYNUM] = {0};
	U8 a_uKeyAction[KEYNUM] = {0};
	U8 a_uKeyValue[KEYNUM] = {0};
	U8 m_uKeyNum = 0;
//��ť��������
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

	if(!Key)	//���а���
	{
		if(a_uKeyDown[m_uKeyNum] == 0)	//����Ǹհ��£���׼�������ʱ
		{
			a_uKeyDown[m_uKeyNum] = 1;		//����һ�ν���ʱ������֪��֮ǰ�а���
			a_uKeyDownCount[m_uKeyNum] = 1;		//����ֵ�ӵ�1
		}
		else	//֮ǰ�Ѿ��а��£����ڼ�ʱ
		{
			if(a_uKeyDownCount[m_uKeyNum] > CHKTIME)
			{
				a_uKeyValue[m_uKeyNum] = KEYHOLD;
			}
			else if((a_uKeyDownCount[m_uKeyNum] < CHKTIME))
			{
				a_uKeyDownCount[m_uKeyNum]++;	//�Ѱ���ʱ���ۼ�
			}

			if(a_uKeyDownCount[m_uKeyNum] == CHKTIME)
			{
				a_uKeyAction[m_uKeyNum] = 1;		//����ʱ�乻���ˣ�ACTION��Ч
				a_uKeyDownCount[m_uKeyNum]++;
				a_uKeyValue[m_uKeyNum] = KEYDOWN;
			}
		}
	}
	else	//��������
	{
		if(a_uKeyAction[m_uKeyNum] == 0)		//֮ǰδ��ACTION
		{										//ȫ��ͳ��������
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			a_uKeyValue[m_uKeyNum] = KEYNULL;
		}
		else	//֮ǰ����ACTION����������
		{
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			a_uKeyAction[m_uKeyNum] = 0;
			a_uKeyValue[m_uKeyNum] = KEYUP;		//ȫ�����㣬����1ֵ
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
	if(Key)	//���а���
	{
		if(a_uKeyDown[m_uKeyNum] == 0)	//����Ǹհ��£���׼�������ʱ
		{
			a_uKeyDown[m_uKeyNum] = 1;
			a_uKeyDownCount[m_uKeyNum] = 1;
		}
		else	//֮ǰ�Ѿ��а��£����ڼ�ʱ
		{
			if(a_uKeyDownCount[m_uKeyNum] >= CHKTIME)
			{
				a_uKeyAction[m_uKeyNum] = 1;		//����ʱ�乻���ˣ�ACTION��Ч
			}
			else
			{
				a_uKeyDownCount[m_uKeyNum]++;	//�Ѱ���ʱ���ۼ�
			}
		}
		m_u1KeyValue = 0;	//����ʱһ������0ֵ����ΪҪ���ֲŶ���
	}
	else	//��������
	{
		if(a_uKeyAction[m_uKeyNum] == 0)		//֮ǰδ��ACTION
		{										//ȫ��ͳ��������
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			m_u1KeyValue = 0;
		}
		else	//֮ǰ����ACTION����������
		{
			a_uKeyDownCount[m_uKeyNum] = 0;
			a_uKeyDown[m_uKeyNum] = 0;
			a_uKeyAction[m_uKeyNum] = 0;
			m_u1KeyValue = 1;		//ȫ�����㣬����1ֵ
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
	else if ( _dwS2HoldTm > 500 ) {	//����50mS
		return SINGLE_PRESS;
	}
	return NO_PRESS;
}