

#include "DataTransEnDecode_F.h"




static U8 _CreateCRC(const U8* inBuf,U16 inLen);
static U8 _DataEncode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen);

static U8 _CheckCRC(const U8* inBuf,U16 inLen);
static U8 _DataDecode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen);


#define CODE_STX				(0x02)
#define CODE_ETX				(0x03)

enum EncodePos
{
	EPOS_STX		= 0,
	EPOS_LEN		= 1,
	EPOS_DATA		= 2
};

//==============================================================
//													内部使用部份

static U8 _CreateCRC(const U8* inBuf,U16 inLen)
{
	U8 uCRC = 0;
	while (inLen--) {
		uCRC ^= inBuf[inLen];
	}
	return uCRC;
}

//基础打包
static U8 _DataEncode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen)
{
	U8 i = 0;
	U8 oi = EPOS_DATA;	//为了书写方便，使用中间变量 oi 而不使用 *outLen
	
	outBuf[EPOS_STX] = CODE_STX;
	for	(i=0; i<inLen; ++i,++oi) {
		switch (inBuf[i]) {
		//所有特殊字符执行同样的动作
		case CODE_STX:
		case CODE_ETX:
			outBuf[oi] = '/';
			++oi;
			outBuf[oi] = inBuf[i] | 0x80;
			break;
		default:
			outBuf[oi] = inBuf[i];
			break;
		}
	}
	outBuf[EPOS_LEN] = oi - 2;	//长度是CRC位置 - 2
	outBuf[oi] = _CreateCRC(&outBuf[EPOS_LEN],outBuf[EPOS_LEN]);	//CRC运算内容为长度与数据
	++oi;
	outBuf[oi] = CODE_ETX;
	*outLen	= oi;
	return 1;
}


//检查CRC的正确性，
static U8 _CheckCRC(const U8* inBuf,U16 inLen)
{
	return inBuf[inLen] == _CreateCRC(inBuf,inLen - 1);
}

//基础解包
static U8 _DataDecode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen)
{
	U8 i = 0;
	U8 oi = 0;
	if ( CODE_STX != inBuf[EPOS_STX] || CODE_ETX != inBuf[inBuf[EPOS_LEN] + 3] ) 
	{
		*outLen = 0;
		return 0;
	}
	
	if ( 0 == _CheckCRC(&inBuf[EPOS_LEN],inBuf[EPOS_LEN]+2) ) {
		*outLen = 0;
		return 0;
	}
	
	for (i=0; i<inBuf[EPOS_LEN]; ++i,++oi)
	{
		if ( '/' == inBuf[EPOS_DATA + i] ) {
			switch ( inBuf[EPOS_DATA + i + 1] ) {
			case '/':
				outBuf[oi] = '/';
				break;
			default:
				outBuf[oi] = inBuf[EPOS_DATA + i + 1] & 0x7F;
				break;
			}
			++i;
		}
		else {
			outBuf[oi] = inBuf[EPOS_DATA + i];
		}
	}
	*outLen = oi;
	return 1;
}


void DataTransInit(PTransEnDecodeFunc_t pTransFunc)
{
	pTransFunc->pfnEncode = _DataEncode;
	pTransFunc->pfnDecode = _DataDecode;
}