

#ifndef __DMA_F_H__
#define __DMA_F_H__

#include "TypeDef_F.h"

typedef struct _DMA_DESC{
  U8 srcAddrH;
  U8 srcAddrL;
  U8 dstAddrH;
  U8 dstAddrL;
  U8 xferLenV;
  U8 xferLenL;
  U8 ctrlA;
  U8 ctrlB;
}DMA_DESC_t;


void DmaInit(void);







#endif //__DMA_F_H__