

#include "DataTransEnDecode_F.h"
#include <string.h>



static U8 _CreateCRC(const U8* inBuf,U16 inLen);
static U8 _DataEncode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen);

//static U8 _CheckCRC(const U8* inBuf,U16 inLen);
static U8 _DataDecode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen);


#define CODE_STX				(0x02)
#define CODE_ETX				(0x03)

enum OriginPos
{
	OPOS_STX		= 0,
	OPOS_LENL		= 1,
	OPOS_LENH		= 2,
	OPOS_DATA		= 3,
	
};

enum EncodePos
{
	EPOS_STX		= 0,
	EPOS_LEN0		= 1,
	EPOS_LEN1		= 2,
	EPOS_LEN2		= 3,
	EPOS_LEN3		= 4,
	EPOS_LENSIZE	= 4,
	EPOS_DATA		= 5,
	EPOS_DOFFSET	= 5	
};


//==============================================================
//													�ڲ�ʹ�ò���

//ͨ���������CRC
static U8 _CreateCRC(const U8* inBuf,U16 inLen)
{
	U8 uCRC = 0;
	while (inLen--) {
		uCRC ^= inBuf[inLen];
	}
	return uCRC;
}

//���CRC����ȷ�ԣ�
//static U8 _CheckCRC(const U8* inBuf,U16 inLen)
//{
//	return inBuf[inLen] == _CreateCRC(inBuf,inLen - 1);
//}

//Ϊ��ʹCRC�������0x02 0x03 ��������룬��CRC���2���ֽڣ�ÿ��
//�ֽڵĸ�4λ��ԭ��CRC�����ݡ�
static U16 _MakeU16CRC(U8 uCRC)
{
	U16 wCRC = (uCRC << 4) | ( ((U16)uCRC) << 8 );
	return  wCRC & 0xF0F0;
}

static U8 _MakeU8CRC(U16 wCRC)
{
	return (wCRC >> 8) | (wCRC >> 4);
}

//�������
static U8 _DataEncode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen)
{
	U16 i = 0;
	U16 oi = EPOS_DATA;	//Ϊ����д���㣬ʹ���м���� oi ����ʹ�� *outLen
	
//����ʼ�ֽ�
	outBuf[EPOS_STX] = CODE_STX;
	
//��������
	for	(i=0; i<inLen; ++i,++oi) {
		switch (inBuf[i]) {
		//����������ִ��ͬ���Ķ�����ǰ���'/'���������λ��1
		case CODE_STX:
		case CODE_ETX:
		case '/':
			outBuf[oi] = '/';
			++oi;
			outBuf[oi] = inBuf[i] | 0x80;
			break;
		default:
		//��������ֱ�ӷŵ����
			outBuf[oi] = inBuf[i];
			break;
		}
	}
	
//������
	U16 wLen = oi - EPOS_DOFFSET;	//��ʱ oi λ�� CRC����oi - EPOS_DOFFSET Ϊ���ݳ���
	outBuf[EPOS_LEN0] = wLen << 4;
	outBuf[EPOS_LEN1] = wLen & 0x00F0;
	outBuf[EPOS_LEN2] = (wLen >> 4) & 0x00F0;
	outBuf[EPOS_LEN3] = (wLen >> 8) & 0x00F0;
	
//����У��
	U8 uCRC = _CreateCRC(&outBuf[EPOS_LEN0],wLen+EPOS_LENSIZE);	//CRC��������Ϊ����������
	U16 wCRC = _MakeU16CRC(uCRC);
	outBuf[oi] = wCRC;
	++oi;
	outBuf[oi] = wCRC >> 8;
	++oi;
	
//����ETX
	outBuf[oi] = CODE_ETX;
	++oi;
	
//�����ܳ���
	*outLen	= oi;	//outLenΪ���������ܳ��ȣ�����STX,ETX
	return 1;
}





//�������
static U8 _DataDecode(const U8 * inBuf,U16 inLen,U8 * outBuf,U16* outLen)
{
	U16 i = 0;
	U16 oi = 0;
//ִ�г���������ȷ��У��
	if ( CODE_STX != inBuf[EPOS_STX] || CODE_ETX != inBuf[inLen-1] )
	{
		*outLen = 0;
		return 0;
	}

//ȡ�ó���
	U16 wLen = (inBuf[EPOS_LEN0] >> 4) | inBuf[EPOS_LEN1] 
		| (inBuf[EPOS_LEN2] << 4) | (inBuf[EPOS_LEN3] << 8);
	
//���CRC	
	U8 uCRC = _CreateCRC(inBuf+EPOS_LEN0,wLen+EPOS_LENSIZE);
	U8 uRxCRC = _MakeU8CRC(inBuf[wLen+5] | (inBuf[wLen+6])<<8 );
	if ( uCRC != uRxCRC ) {
		*outLen = 0;
		return 0;
	}
	
//���ݽ��
	for (i=0; i<wLen; ++i,++oi)
	{
		//���������⴦��
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
		//�������ݲ��ô���
		else {
			outBuf[oi] = inBuf[EPOS_DATA + i];
		}
	}
	
//�����ܳ���
	*outLen = oi;
	return 1;
}


void DataTransInit(PTransEnDecodeFunc_t pTransFunc)
{
	pTransFunc->pfnEncode = _DataEncode;
	pTransFunc->pfnDecode = _DataDecode;
}

static CatchPackageStatus		_cps = CPS_NULL;
static U8		_uRxBuf[256];
static U16		_wRxPtr = 0;
static BOOL		_bPackageOk = FALSE;
//static U16		_wRxLen = 0;

inline static void _Rollback()
{
	_wRxPtr = 0;
	_cps = CPS_NULL;	
}

inline static void _NewStart(U8 uByte)
{
	_uRxBuf[0] = uByte;
	_wRxPtr = 1;
	_cps = CPS_CATCHING;	
}



CatchPackageStatus DEN_CatchPackage(U8 uByte)
{
	//���ݰ��Ѿ��ɹ�����δ����ȡ֮ǰ������������ֽ�
	if ( TRUE == _bPackageOk ) {
		_cps = CPS_NULL;
		return _cps;
	}
	
	//����ʱ�̣�ֻҪ���� CODE_STX ����һ��ȫ�µĿ�ʼ��
	if ( CODE_STX == uByte ) {
//		_cps = CPS_CATCHING;
//		_uRxBuf[0] = CODE_STX;
//		_wRxPtr = 1;
		_NewStart(uByte);
		return _cps;
	}
	
	
	if ( CODE_ETX == uByte ) {
		//�յ� CODE_ETX ��ʱ�����״̬���ԣ���ȫ���������
		if ( _cps != CPS_CATCHING ) {
//			_cps = CPS_NULL;
//			_wRxPtr = 0;
			_Rollback();
			return _cps;
		}
		//�յ� CODE_ETX ��ʱ�����״̬�ԣ������ݰ����ճɹ�
		else {
			_uRxBuf[_wRxPtr] = CODE_ETX;
			++_wRxPtr;
			_cps = CPS_COMPLETE;
			_bPackageOk = TRUE;
			return _cps;
		}
	}
	
	
	//�յ����������ݣ��� CATCHING ״̬�£������ݷ�����
	if ( CPS_CATCHING == _cps ) {
		++_wRxPtr;
		_uRxBuf[_wRxPtr] = uByte;
	}

	return _cps;
}


BOOL DEN_GetPackage(U16* pwLen,U8* pData)
{
	if ( FALSE == _bPackageOk ) {
		return FALSE;
	}
	return _DataDecode(_uRxBuf,_wRxPtr,pData,pwLen);
}