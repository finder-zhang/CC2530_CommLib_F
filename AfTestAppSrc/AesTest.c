
#include "TypeDef_F.h"

#include "hal_aes.h"
#include "HalAes_F.h"




	static U8 inBuf[16] = {
		0x32,0x43,0xF6,0xA8,		0x88,0x5A,0x30,0x8D,
		0x31,0x31,0x98,0xA2,		0xE0,0x37,0x07,0x34		};
	
	static U8 outCheck[16] = {
		0x39,0x25,0x84,0x1D,		0x02,0xDC,0x09,0xFB,
		0xDC,0x11,0x85,0x97,		0x19,0x6A,0x0B,0x32		};
	
	static U8 Key[16] = {
		0x2b, 0x7e, 0x15, 0x16, 	0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88,		0x09, 0xcf, 0x4f, 0x3c	};
	
	static U8 IV[16] = {
		0x00, 0x00, 0x00, 0x00,		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,		0x00, 0x00, 0x00, 0x00	};
	
	static U8 outBuf[16];
	static U8 outBuf2[16];
	
	
	
	
	
	
	
	
	
	
	
void AesTest(void)
{
	
		//const U8 ZeroS[16] =
	

	memset(outBuf,0,16);
	memset(outBuf2,0,16);
	
	U8 inb02[16];
	U8 key02[16];
	U8 ivb02[16];
	U8 oub02[16];
	memset(inb02,0,16);
	memset(key02,0,16);
	memset(ivb02,0,16);
	memset(oub02,0,16);
	
	//HAES_SetEncodeMode(EM_ECB);
	//HAES_SetKey(Key,16);
	//HAES_Encode(inBuf,16,outBuf);
	
	U8* pIn = inBuf;//inb02;
	U8* pOu = oub02;
	U8* pKe = key02;
	U8* pIv = ivb02;
	
//	U8* pIn = inBuf;
//	U8* pOu = outBuf;
//	U8* pKe = Key;
//	U8* pIv = IV;
	
//	U8 uMode = ECB;
//	//AES_SETMODE(ECB);
//	AES_SETMODE(uMode);
//	
//	AES_START();	
//	AesLoadKey(pKe);
//	AES_START();
//	AesLoadIV(pIv);
//	AES_START();
//	AesStartBlock(pOu,pIn);
//	
//	Uprintf("test %02x\r\n",uMode);
//	
//	for (int i=0; i<16; ++i)
//	{
//		Uprintf("0x%02x ",pOu[i]);
//		if ((i+1)%4 == 0) {
//			Uprintf("\r\n");
//		}
//	}
	
}