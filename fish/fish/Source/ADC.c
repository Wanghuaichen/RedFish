#include "ADC.h"
void ADCInitialize(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);						//ADC1ʱ�ӿ���
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//��������ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						//������ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//ת������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 					//����λ�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;									//ɨ���ͨ����
	ADC_Init(ADC1,&ADC_InitStructure);										//ADC1�������Ի�
	ADC_RegularChannelConfig(ADC1,ADC_Channel_17,1,ADC_SampleTime_1Cycles5);//ADC1ͨ��3˳��1
	ADC_Cmd(ADC1,ENABLE);
}
u16 ADC1GetConvertValue(void)
{
	u16 ADCValue; 
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);					//
	ADCValue=ADC_GetConversionValue(ADC1);
	return ADCValue;
}
u32 GetRandomValue(int CountLimit)
{
	u32 RandomSeed=0;
	u8 i=0;
	for(i=0;i<4;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
		RandomSeed|=(ADC_GetConversionValue(ADC1)&0xFF)<<(i*8);
	}
	srand(RandomSeed);
	return rand()%CountLimit;
}
