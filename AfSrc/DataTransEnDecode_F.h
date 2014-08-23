
/*******************************************
���ݴ�����������
*******************************************/


#ifndef __DATA_TRANS_EN_DECODE_F_H__
#define __DATA_TRANS_EN_DECODE_F_H__

#include "TypeDef_F.h"

//��������ӿں���
//inBuf		Ҫ���������
//uLen		Ҫ����ĳ���
//outBuf	���������
//outLen	����ĳ���
typedef U8 (*fnTransEnDecode_t)(const U8* inBuf,U16 inLen,U8* outBuf,U16* outLen);


typedef struct _TransEnDecodeFunc
{
	fnTransEnDecode_t		pfnEncode;
	fnTransEnDecode_t		pfnDecode;
}TransEnDecodeFunc_t,*PTransEnDecodeFunc_t;


void DataTransInit(PTransEnDecodeFunc_t pTransFunc);









#endif //__DATA_TRANS_EN_DECODE_F_H__