
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


void DataTransInit(PTransEnDecodeFunc_t pTransFunc);









#endif //__DATA_TRANS_EN_DECODE_F_H__