
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


typedef enum _CatchPackageStatus
{
	CPS_NULL			= 0,
	CPS_CATCHING		,
	CPS_COMPLETE
//	CPS_DATA_ERROR
}CatchPackageStatus;

void DataTransInit(PTransEnDecodeFunc_t pTransFunc);
CatchPackageStatus DEN_CatchPackage(U8 uByte);			//�ڴ��ڽ��պ���������
BOOL DEN_GetPackage(U16* pwLen,U8* pData);


/*********************************************************
ʹ�÷�����
void main()
{
	TransEnDecodeFunc_t		ted;
	DataTransInit(&ted);
}



U16 wLen;
U8 uRxBuf[256];

xxx �����жϻ�ص�(U8 uByte)
{
	if ( CPS_COMPLETE == DEN_CatchPackage(uByte) ) {
		DEN_GetPackage(&wLen,uRxBuf);		//�����͵õ������ݵĳ���������
	}
}
*********************************************************/









#endif //__DATA_TRANS_EN_DECODE_F_H__