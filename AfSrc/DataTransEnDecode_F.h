
/*******************************************
数据传输编码与解码
*******************************************/


#ifndef __DATA_TRANS_EN_DECODE_F_H__
#define __DATA_TRANS_EN_DECODE_F_H__

#include "TypeDef_F.h"

//打包与解包接口函数
//对于打包时
//inBuf		要输入的内容	长度（2B）数据（=长度）
//inLen		要输入的长度	以上总长度
//outBuf	输出的内容		STX（1B）长度（4B）输入内容整理后（=长度）CRCL（1B）CRCH（1B）ETX（1B）
//outLen	输出的长度		以上总长度

//对于解包时
//inBuf		要解包的内容	STX（1B）长度（4B）输入内容整理后（=长度）CRCL（1B）CRCH（1B）ETX（1B）
//inLen		要解包的长度	以上总长度
//outBuf	数据内容		长度（2B）数据（=长度）
//outLen	数据长度		以上总长度
typedef U8 (*fnTransEnDecode_t)(const U8* inBuf,U16 inLen,U8* outBuf,U16* outLen);


typedef struct _TransEnDecodeFunc
{
	fnTransEnDecode_t		pfnEncode;
	fnTransEnDecode_t		pfnDecode;
}TransEnDecodeFunc_t,*PTransEnDecodeFunc_t;


typedef enum _CatchPackageStatus
{
	CPS_NULL			= 0,
	CPS_CATCHING		,
	CPS_COMPLETE
//	CPS_DATA_ERROR
}CatchPackageStatus;

void DataTransInit(PTransEnDecodeFunc_t pTransFunc);
CatchPackageStatus DEN_CatchPackage(U8 uByte);			//在串口接收函数处调用
BOOL DEN_GetPackage(U16* pwLen,U8* pData);


/*********************************************************
使用方法：
void main()
{
	TransEnDecodeFunc_t		ted;
	DataTransInit(&ted);
}



U16 wLen;
U8 uRxBuf[256];

xxx 串口中断或回调(U8 uByte)
{
	if ( CPS_COMPLETE == DEN_CatchPackage(uByte) ) {
		DEN_GetPackage(&wLen,uRxBuf);		//这样就得到了数据的长度与内容
	}
}
*********************************************************/









#endif //__DATA_TRANS_EN_DECODE_F_H__