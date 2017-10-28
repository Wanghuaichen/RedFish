#include "Fish.h"
u8 Test=0;
int main()
{
	TIM_Cmd(TIM1, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000);      
	ParameterInitialize();     //参数初始化
	ADCInitialize();
	Timer1Initialize();        //定时器1初始化
	Timer2Initialize();        
	Timer3Initialize(200);
	GPIOInitialize();        //led初始化
	GetFishID();             //得到鱼的ID
	UARTInitialize();            //串口初始化
	//IICInitialize();
	//MPUInitialize();
	//HMCInitialize();
	NVICInitialize();               //中断初始化
	SPIInitialize();                //spi 初始化
	NrfInitialize(3);                  //2.4G初始化
	//DHT11_PortOUT();
	//DHT11_Out = 0;
	if(NrfCheck())
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	else
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//delay_ms(5000);
	while(1);
}
