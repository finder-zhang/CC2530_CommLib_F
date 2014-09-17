
#include <string.h>
//#include <math.h>
#include <stdlib.h>
#include "./AfSrc/DataTransEnDecode_F.h"




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

int main( void )
{
	TransEnDecodeFunc_t ted;
	DataTransInit(&ted);
	
//	char chIn[];
//	char chOut[64];
//	char chOut02[64];
	

//	U16 inLen;
//	U16 outLen;
//	U16 outLen02;	
//	CreateInBuf(chIn,&inLen);	
//	ted.pfnEncode(chIn,inLen,chOut,&outLen);	
//	ted.pfnDecode(chOut,outLen,chOut02,&outLen02);
	
	U8 uOk = 1;
	U8 uOk2 = 0;
	for (int i=0;i<1024;++i) {
		uOk2 = CheckEncode(&ted);
		uOk = uOk & uOk2;
	}
	
	printf("uOk = %d\r\n",uOk);
	
	return 0;
}
