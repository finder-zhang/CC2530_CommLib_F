
/*******************************************
数据传输编码与解码
*******************************************/


#ifndef __DATA_TRANS_EN_DECODE_F_H__
#define __DATA_TRANS_EN_DECODE_F_H__

#include "TypeDef_F.h"

//打包与解包接口函数
//inBuf		要输入的内容
//uLen		要输入的长度
//outBuf	输出的内容
//outLen	输出的长度
typedef U8 (*fnTransEnDecode_t)(const U8* inBuf,U16 inLen,U8* outBuf,U16* outLen);


typedef struct _TransEnDecodeFunc
{
	fnTransEnDecode_t		pfnEncode;
	fnTransEnDecode_t		pfnDecode;
}TransEnDecodeFunc_t,*PTransEnDecodeFunc_t;


void DataTransInit(PTransEnDecodeFunc_t pTransFunc);









#endif //__DATA_TRANS_EN_DECODE_F_H__