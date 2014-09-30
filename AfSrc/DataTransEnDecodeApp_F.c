

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataTransEnDecode_F.h"

U8 chIn[1024];
U8 chOut[1024];
U8 chOut02[1024];

void CreateInBuf(U8* in,U16* pwLen)
{
	*pwLen = rand() / 512;
	for (U16 i=0; i<*pwLen;++i) {
		in[i] = rand();
	}
}

U8 CheckEncode(PTransEnDecodeFunc_t pTED)
{
	U16 inLen;
	U16 outLen;
	U16 outLen02;
	
	CreateInBuf(chIn,&inLen);
	
	pTED->pfnEncode(chIn,inLen,chOut,&outLen);
	
	pTED->pfnDecode(chOut,outLen,chOut02,&outLen02);
	
	if ( inLen != outLen02 ) {
		return 0;
	}
	
	U8 uRet = 0 == memcmp(chIn,chOut02,inLen);
	return uRet;
}


U8 DataTransTest()
{
	TransEnDecodeFunc_t ted;
	DataTransInit(&ted);
	
	U8 uOk = 1;	
	for (int i=0;i<1024;++i) {
		uOk &= CheckEncode(&ted);
	}
	
	printf("uOk = %d\r\n",uOk);
	
	return uOk;
}