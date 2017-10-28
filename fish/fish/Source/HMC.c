#include "HMC.h"
u8 HMCData[6];

short M_X,M_Y,M_Z; //读取寄存器变量

/*
********************************************************************************
** 函数名称 ： void HMC5883L_Init(void)
** 函数功能 ： 初始化HMC5883L
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
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
** 函数名称 ： void Multiple_Read_HMC5883(void)
** 函数功能 ： 连续读取HMC5883L寄存器
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void HMCReadData(void)
{
    u8 i;  //连续读出HMC5883内部角度数据，地址范围0x3~0x5

	for(i=0;i<6;i++)
	  HMCData[i]=IICReadByte(MAG_ADDRESS,i+3); 

}
