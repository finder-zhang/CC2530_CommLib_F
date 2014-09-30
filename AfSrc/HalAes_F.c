
#include <ioCC2530.h>
#include "HalAes_F.h"
#include "Uart_F.h"
#include "Timer_F.h"

//==========================================常量
enum EncodeCommand
{
	EC_ENCODE = 0,
	EC_DECODE,
	EC_SETKEY,
	EC_SETIV,
	EC_MAX_IDX = EC_SETIV
};



//===========================================内部函数声明
static void _Start(void);
static U8 _IsBusy(void);
static void _WaitFree(void);
static U8 _SetCommand(U8 uCmd);
static void _Write(const U8* pData,U16 wLen);
static void _Read(U8* pData,U16 wLen);



//===========================================内部函数定义
static void _Start(void)
{
	ENCCS |= 0x01;
}
static U8 _IsBusy(void)
{
	return ( 0 == (ENCCS & 0x08) );
}
static void _WaitFree(void)
{
	static U8 i = 0;
	while ( 0 == (ENCCS & 0x08) )
	{
		//Uprintf("WaitForFree %d\r\n",i++);
	}
}
static U8 _SetCommand(U8 uCmd)
{
	if (uCmd > EC_MAX_IDX) {
		return FALSE;
	}
	ENCCS |= (uCmd << 1);
	return TRUE;		
}
static void _Write(const U8* pData,U16 wLen)
{
	while(wLen--)
	{
		ENCDI = *pData++;
	}
}
static void _Read(U8* pData,U16 wLen)
{
	while(wLen--)
	{
		*pData++ = ENCDO;
	}
}




//===========================================外部函数定义
//U8 HalAesInit(void)
//{
//	return TRUE;
//}
//
//
//U8 HalAesDeinit()
//{
//	return TRUE;
//}

U8 HAES_SetEncodeMode(U8 uMode)
{
	if (uMode > EM_MAX_IDX) {
		return FALSE;
	}
	_WaitFree();
	ENCCS |= (uMode << 4);
	return TRUE;
}

U8 HAES_SetKey(const U8* pKey,U8 uKeyLen)
{
	if (	(uKeyLen != 16) 
		&&	(uKeyLen != 24)
		&&	(uKeyLen != 32)		)
	{
		return FALSE;
	}
	
	_SetCommand(EC_SETKEY);
	_Start();
	_Write(pKey,uKeyLen);
	DelayMs(1);
	return TRUE;
}

U8 HAES_SetIV(const U8* pIV,U8 uIVLen)
{
	_SetCommand(EC_SETIV);
	_Start();
	_Write(pIV,uIVLen);
	DelayMs(1);
	return TRUE;
}

U8 HAES_Encode(const U8* inData,U16 wLen,U8* outData)
{
//	const U8 FILL_ZERO[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//	U8 uFillLen = ( 0x10 - (wLen & 0xFFF0) ) & 0x0F;
//	_WaitFree();
//	Uprintf("SetEncodeMode\r\n");
//	_SetCommand(EC_ENCODE);
//	Uprintf("WriteData\r\n");
//	_Start();
//	_Write(inData,wLen);
//	_Write(FILL_ZERO,uFillLen);
//	Uprintf("Start!\r\n");
//	//_Start();
//	Uprintf("Wait ok!\r\n");
//	_WaitFree();
//	Uprintf("Decode complete!\r\n");
//	_Read(outData,wLen + uFillLen);
	_WaitFree();
	_Start();
	_Write(inData,16);
	DelayMs(10);
	_Read(outData,16);
	return TRUE;
}

U8 HAES_Decode(const U8* inData,U16 wLen,U8* outData)
{
	//如果要解密的数据长度不是16的整倍，则直接返回错误
	if ( wLen & 0x0F ) {
		return FALSE;
	}
	_WaitFree();
	_SetCommand(EC_DECODE);
	_Write(inData,wLen);
	_Start();
	_WaitFree();
	_Read(outData,wLen);
	return TRUE;
}