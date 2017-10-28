#include "GPIO.h"
void GPIOInitialize(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;						//LEDÀ¶
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14;					
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);						
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;						//GPIOA3 AD
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
