#include "Fish.h"
u8 Test=0;
int main()
{
	TIM_Cmd(TIM1, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000);      
	ParameterInitialize();     //������ʼ��
	ADCInitialize();
	Timer1Initialize();        //��ʱ��1��ʼ��
	Timer2Initialize();        
	Timer3Initialize(200);
	GPIOInitialize();        //led��ʼ��
	GetFishID();             //�õ����ID
	UARTInitialize();            //���ڳ�ʼ��
	//IICInitialize();
	//MPUInitialize();
	//HMCInitialize();
	NVICInitialize();               //�жϳ�ʼ��
	SPIInitialize();                //spi ��ʼ��
	NrfInitialize(3);                  //2.4G��ʼ��
	//DHT11_PortOUT();
	//DHT11_Out = 0;
	if(NrfCheck())
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	else
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//delay_ms(5000);
	while(1);
}
