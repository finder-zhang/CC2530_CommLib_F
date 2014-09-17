
/*******************************************
���ݴ�����������
*******************************************/


#ifndef __DATA_TRANS_EN_DECODE_F_H__
#define __DATA_TRANS_EN_DECODE_F_H__

#include "TypeDef_F.h"

//��������ӿں���
//���ڴ��ʱ
//inBuf		Ҫ���������	���ȣ�2B�����ݣ�=���ȣ�
//inLen		Ҫ����ĳ���	�����ܳ���
//outBuf	���������		STX��1B�����ȣ�4B���������������=���ȣ�CRCL��1B��CRCH��1B��ETX��1B��
//outLen	����ĳ���		�����ܳ���

//���ڽ��ʱ
//inBuf		Ҫ���������	STX��1B�����ȣ�4B���������������=���ȣ�CRCL��1B��CRCH��1B��ETX��1B��
//inLen		Ҫ����ĳ���	�����ܳ���
//outBuf	��������		���ȣ�2B�����ݣ�=���ȣ�
//outLen	���ݳ���		�����ܳ���
typedef U8 (*fnTransEnDecode_t)(const U8* inBuf,U16 inLen,U8* outBuf,U16* outLen);


typedef struct _TransEnDecodeFunc
{
	fnTransEnDecode_t		pfnEncode;
	fnTransEnDecode_t		pfnDecode;
}TransEnDecodeFunc_t,*PTransEnDecodeFunc_t;


void DataTransInit(PTransEnDecodeFunc_t pTransFunc);









#endif //__DATA_TRANS_EN_DECODE_F_H__