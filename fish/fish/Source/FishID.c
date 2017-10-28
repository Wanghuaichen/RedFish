#include "FishID.h"
u8 FishID[5];

void GetFishID(void)
{
	FishID[4]=*(u8*)(0x1ffff7e8)+*(u8*)(0x1ffff7ed)+*(u8*)(0x1ffff7f2);
	FishID[3]=*(u8*)(0x1ffff7e9)+*(u8*)(0x1ffff7ee)+*(u8*)(0x1ffff7f3);
	FishID[2]=*(u8*)(0x1ffff7ea)+*(u8*)(0x1ffff7ef);
	FishID[1]=*(u8*)(0x1ffff7eb)+*(u8*)(0x1ffff7f0);
	FishID[0]=*(u8*)(0x1ffff7ec)+*(u8*)(0x1ffff7f1);
}
