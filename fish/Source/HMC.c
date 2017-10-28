#include "HMC.h"
u8 HMCData[6];

short M_X,M_Y,M_Z; //��ȡ�Ĵ�������

/*
********************************************************************************
** �������� �� void HMC5883L_Init(void)
** �������� �� ��ʼ��HMC5883L
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
void HMCInitialize(void)
{
	IICWriteByte(MAG_ADDRESS, ConfigRegA, SampleAveraging_8 << 5 | DataOutputRate_75HZ << 2 | NormalOperation);
	delay_ms(50);
	IICWriteByte(MAG_ADDRESS, ConfigRegB, magGain);
	delay_ms(50);
	IICWriteByte(MAG_ADDRESS, ModeRegister, ContinuousConversion);
	delay_ms(50);
}

/*
********************************************************************************
** �������� �� void Multiple_Read_HMC5883(void)
** �������� �� ������ȡHMC5883L�Ĵ���
** ��    ��	�� ��
** ��    ��	�� ��
** ��    ��	�� ��
********************************************************************************
*/
void HMCReadData(void)
{
    u8 i;  //��������HMC5883�ڲ��Ƕ����ݣ���ַ��Χ0x3~0x5

	for(i=0;i<6;i++)
	  HMCData[i]=IICReadByte(MAG_ADDRESS,i+3); 

}
