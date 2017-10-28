#ifndef NVICH
#define NVICH
#include "Fish.h"
extern u8 fRescue;
extern u8 fLedFlash;
extern void NVICInitialize(void);
extern void ReceiveData(u8 *Receive);
extern void EXTIInitialize(void);
#endif
