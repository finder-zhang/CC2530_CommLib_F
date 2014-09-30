

#ifndef __HAL_AES_F_H__
#define __HAL_AES_F_H__

#include "TypeDef_F.h"

enum EncodeMode
{
	EM_CBC = 0,
	EM_CFB,
	EM_OFB,
	EM_CTR,
	EM_ECB,
	EM_MAC,
	EM_MAX_IDX = EM_MAC
};

//U8 HalAesInit();
//U8 HalAesDeinit();

U8 HAES_SetEncodeMode(U8 uMode);

U8 HAES_SetKey(const U8* pKey,U8 uKeyLen);
U8 HAES_SetIV(const U8* pIV,U8 uIVLen);

U8 HAES_Encode(const U8* inData,U16 wLen,U8* outData);
U8 HAES_Decode(const U8* inData,U16 wLen,U8* outData);








#endif //__HAL_AES_F_H__