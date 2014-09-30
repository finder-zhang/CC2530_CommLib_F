

#include "Dma_F.h"

#include <ioCC2530.h>

static DMA_DESC_t		_dmaDesc[5];



void DmaInit(void)
{	
//	DMA0CFGH = ((U16)_dmaDesc) >> 8;
//	DMA0CFGL = (U8)_dmaDesc;
//	DMA1CFGH = (&_dmaDesc[1]) >> 8;
//	DMA1CFGL = (&_dmaDesc[1]);
//	
//	U8 uIdx = 0;
//	_dmaDesc[uIdx].srcAddrH = 0;
//	_dmaDesc[uIdx].srcAddrL = ENCDO;
//	_dmaDesc[uIdx].dstAddrH = 0;
//	_dmaDesc[uIdx].dstAddrL = ENCDI;
//	_dmaDesc[uIdx].xferLenV = 0x00;
//	_dmaDesc[uIdx].xferLenL = 0x16;
//	_dmaDesc[uIdx].ctrlA = (0 << 7) | 
//	_dmaDesc[uIdx].
}

