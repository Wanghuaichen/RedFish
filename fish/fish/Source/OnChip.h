#ifndef OnChipH
#define OnChipH
#include "Fish.h"
#define ParameterAddress 0x0800F000
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE ((uint16_t)0x800)
#else
#define FLASH_PAGE_SIZE ((uint16_t)0x400)
#endif
extern void ParameterInitialize(void);
extern u8 FLASH_ProgramBytes(u32 Address, u8 *Buffer, u16 ByteCount);
extern u8 FlashWriteBytes(u32 Address, u8 *Buffer, u16 ByteCount);
extern u8 FlashWriteWords(u32 Address, u32 *Buffer, u16 WordCount);
extern void FlashReadBytes(u32 Address, u8 *Buffer, u16 ByteCount);
extern void FlashReadWords(u32 Address, u32 *Buffer, u16 WordCount);
extern void SaveParameter(void);
extern void ReadParameter(void);
extern u8 FlashErasePage(u32 PageAddress,u16 PageCount);
union FloatData
{
	float Save;
	u32   Buffer;
};
typedef struct Parameter
{
	int Version;
	int Offset0,
		Offset1,
		Offset2;
	u8 Password[8];
}FishParameter;
extern FishParameter FishSetting;
#endif
