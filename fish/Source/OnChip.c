#include "OnChip.h"
FishParameter FishSetting;

void ParameterInitialize(void)
{
	ReadParameter();
	if(FishSetting.Version==0xFFFFFFFF)
	{
		FishSetting.Version=0;
		FishSetting.Offset0=0;
		FishSetting.Offset1=0;
		FishSetting.Offset2=0;
		FishSetting.Password[0]='M';
		FishSetting.Password[1]='e';
		FishSetting.Password[2]='a';
		FishSetting.Password[3]='n';
		FishSetting.Password[4]='o';
		FishSetting.Password[5]='C';
		FishSetting.Password[6]='a';
		FishSetting.Password[7]='t';
		SaveParameter();
	}
	else
	{
		ServoOffset[0]=FishSetting.Offset0;
		ServoOffset[1]=FishSetting.Offset1;
		ServoOffset[2]=FishSetting.Offset2;
	}
}
void SaveParameter(void)
{
	FlashWriteWords(ParameterAddress,(u32 *)&FishSetting,6);
}
void ReadParameter(void)
{
	FlashReadWords(ParameterAddress,(u32 *)&FishSetting,6);
}

u8 FLASH_ProgramBytes(u32 Address, u8 *Buffer, u16 ByteCount)		//Flash写入多个8位字节
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;						//Flash状态字节
    u16 i = 0;														//循环次数
    while((i<ByteCount) && (FLASHStatus == FLASH_COMPLETE))			//循环字节次数
    {
        FLASHStatus = FLASH_ProgramHalfWord(Address, *(u16*)Buffer);//写入一个16位半字
        if(FLASHStatus!=FLASH_COMPLETE)								//写入不成功
			return 1;												//返回1
		i = i+2;													//字节数+2
        Address = Address + 2;										//地址+2
        Buffer = Buffer + 2;										//要写入的数据字节+2
    }
	return 0;														//成功返回0
}

u8 FLASH_ProgramWords(u32 Address, u32 *Buffer, u16 WordCount)		//Flash写入多个32位字
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;						//Flash状态字节
    u16 i = 0;														//循环次数
    while((i<(WordCount)) && (FLASHStatus == FLASH_COMPLETE))		//循环字次数
    {
        FLASHStatus = FLASH_ProgramWord(Address, *Buffer);			//写入一个字
		if(FLASHStatus!= FLASH_COMPLETE)							//错误判断
			return 1;												//返回1
        i = i+1;													//循环次数+1
        Address = Address + 4;										//地址+4
        Buffer = Buffer + 1;										//要写入的字数+1
    }
	return 0;														//成功返回0
}

u8 FlashWriteBytes(u32 Address, u8 *Buffer, u16 ByteCount)			//Flash 写入多字节
{
	u32 PageAddress;												//32位页地址
	u16 PageOffset;													//16位页偏移
	u8 PageData[FLASH_PAGE_SIZE];									//页数据1024个字节
	u16 i;															//循环次数
	FLASH_Status FLASHStatus = FLASH_COMPLETE;						//Flash状态
	
	FLASH_Unlock();													//解锁Flash
	PageAddress=Address & (~(FLASH_PAGE_SIZE -1 )) ;				//页地址
    PageOffset=Address & (FLASH_PAGE_SIZE -1); 						//页偏移
	
	FlashReadBytes(PageAddress,PageData,FLASH_PAGE_SIZE);			//读取整页1024字节的数据
	
	for(i=0;i<ByteCount;i++)										//循环判断
	{
		if(PageData[PageOffset+i]!=0xFF)							//如果要写入的部分不为FF则需要擦除
			break;
	}
	if(i<ByteCount)													//判断需要擦除
	{
		FLASHStatus = FLASH_ErasePage(PageAddress);					//擦除页
		if(FLASHStatus!= FLASH_COMPLETE)							//错误判断
			return 1;
		for(i=0;i<ByteCount;i++)									//改变读取值
		{ 
			PageData[PageOffset+i]=Buffer[i]; 
		}
		i=FLASH_ProgramBytes(PageAddress,PageData,FLASH_PAGE_SIZE);	//写入整个扇区 
		if(i==1)
			return 1;												//写入失败
	}	
	else
	{
		i=FLASH_ProgramBytes(Address,Buffer,ByteCount);
		if(i==1)
			return 1;												//写入失败
	}
    FLASH_Lock();													//Flash锁定
	return 0;
}
u8 FlashErasePage(u32 PageAddress,u16 PageCount)					//Flash删除多个页
{
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	u16 i;
	FLASH_Unlock();
	for(i=0;i<PageCount;i++)
	{
		FLASHStatus=FLASH_ErasePage(PageAddress);
		PageAddress+=FLASH_PAGE_SIZE;
		if(FLASHStatus!=FLASH_COMPLETE)
			return 1;
	}
	FLASH_Lock();
	return 0;
}
u8 FlashWriteWords(u32 Address, u32 *Buffer, u16 WordCount) 
{
	return FlashWriteBytes(Address, (u8 *)Buffer, WordCount*4);
}

void FlashReadBytes(u32 Address, u8 *Buffer, u16 ByteCount) 
{
    u16 i = 0;
    while(i < ByteCount) 
    {
        *(Buffer + i) = *(__IO u8*) Address++;
        i++;
    }
}

void FlashReadWords(u32 Address, u32 *Buffer, u16 WordCount)
{
	FlashReadBytes(Address, (u8 *)Buffer,(WordCount*4));
}
