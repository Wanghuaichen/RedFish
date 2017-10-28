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

u8 FLASH_ProgramBytes(u32 Address, u8 *Buffer, u16 ByteCount)		//Flashд����8λ�ֽ�
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;						//Flash״̬�ֽ�
    u16 i = 0;														//ѭ������
    while((i<ByteCount) && (FLASHStatus == FLASH_COMPLETE))			//ѭ���ֽڴ���
    {
        FLASHStatus = FLASH_ProgramHalfWord(Address, *(u16*)Buffer);//д��һ��16λ����
        if(FLASHStatus!=FLASH_COMPLETE)								//д�벻�ɹ�
			return 1;												//����1
		i = i+2;													//�ֽ���+2
        Address = Address + 2;										//��ַ+2
        Buffer = Buffer + 2;										//Ҫд��������ֽ�+2
    }
	return 0;														//�ɹ�����0
}

u8 FLASH_ProgramWords(u32 Address, u32 *Buffer, u16 WordCount)		//Flashд����32λ��
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;						//Flash״̬�ֽ�
    u16 i = 0;														//ѭ������
    while((i<(WordCount)) && (FLASHStatus == FLASH_COMPLETE))		//ѭ���ִ���
    {
        FLASHStatus = FLASH_ProgramWord(Address, *Buffer);			//д��һ����
		if(FLASHStatus!= FLASH_COMPLETE)							//�����ж�
			return 1;												//����1
        i = i+1;													//ѭ������+1
        Address = Address + 4;										//��ַ+4
        Buffer = Buffer + 1;										//Ҫд�������+1
    }
	return 0;														//�ɹ�����0
}

u8 FlashWriteBytes(u32 Address, u8 *Buffer, u16 ByteCount)			//Flash д����ֽ�
{
	u32 PageAddress;												//32λҳ��ַ
	u16 PageOffset;													//16λҳƫ��
	u8 PageData[FLASH_PAGE_SIZE];									//ҳ����1024���ֽ�
	u16 i;															//ѭ������
	FLASH_Status FLASHStatus = FLASH_COMPLETE;						//Flash״̬
	
	FLASH_Unlock();													//����Flash
	PageAddress=Address & (~(FLASH_PAGE_SIZE -1 )) ;				//ҳ��ַ
    PageOffset=Address & (FLASH_PAGE_SIZE -1); 						//ҳƫ��
	
	FlashReadBytes(PageAddress,PageData,FLASH_PAGE_SIZE);			//��ȡ��ҳ1024�ֽڵ�����
	
	for(i=0;i<ByteCount;i++)										//ѭ���ж�
	{
		if(PageData[PageOffset+i]!=0xFF)							//���Ҫд��Ĳ��ֲ�ΪFF����Ҫ����
			break;
	}
	if(i<ByteCount)													//�ж���Ҫ����
	{
		FLASHStatus = FLASH_ErasePage(PageAddress);					//����ҳ
		if(FLASHStatus!= FLASH_COMPLETE)							//�����ж�
			return 1;
		for(i=0;i<ByteCount;i++)									//�ı��ȡֵ
		{ 
			PageData[PageOffset+i]=Buffer[i]; 
		}
		i=FLASH_ProgramBytes(PageAddress,PageData,FLASH_PAGE_SIZE);	//д���������� 
		if(i==1)
			return 1;												//д��ʧ��
	}	
	else
	{
		i=FLASH_ProgramBytes(Address,Buffer,ByteCount);
		if(i==1)
			return 1;												//д��ʧ��
	}
    FLASH_Lock();													//Flash����
	return 0;
}
u8 FlashErasePage(u32 PageAddress,u16 PageCount)					//Flashɾ�����ҳ
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
