#include "Timer.h"
void Timer1Initialize(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;     
//	TIM_OCInitTypeDef TIM_OCInitStructure;     
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE |
							RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
							
	
	//Ƶ���趨
  TIM_BaseInitStructure.TIM_Period = 1000-1;     
	TIM_BaseInitStructure.TIM_Prescaler = 24-1;     
	TIM_BaseInitStructure.TIM_ClockDivision = 0; 
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;     
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);     
	//����ARR��Ӱ�ӼĴ���
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	//CH1PWMƵ���趨
    /*
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;     
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     
	TIM_OCInitStructure.TIM_Pulse = 0;     
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	*/
	//����CH1Ӱ�ӼĴ���
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	//�����趨
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;     
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;     
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;     
	TIM_BDTRInitStructure.TIM_DeadTime = 0x00;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; 
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; 
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	//������ʱ��
	
    TIM_Cmd(TIM1, ENABLE);
}
void Timer2Initialize(void)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//�ر�Jtag
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	//��·PWM GPIO �˿���ӳ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
	
	//PWM GPIO�趨
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//|GPIO_Pin_3; //��·PWM���
	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11;		//��·PWM���
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;							//��·PWM���
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//APB��ʱ��2����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//Ƶ���趨
	TIM_TimeBaseStructure.TIM_Period = 40000;						//��������		20ms
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;						//pwmʱ�ӷ�Ƶ	2MHZ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;						
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 3000;								//��ʼռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//CH1
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//CH2
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//CH3
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	//CH4
	//TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	//TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//ARR
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//������ʱ��
	TIM_Cmd(TIM2, ENABLE);
	//github test
}
void Timer3Initialize(u16 us)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//�������ã�ʱ���ͱȽ�������ã���������ֻ�趨ʱ�����Բ���OC�Ƚ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=us;				//װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}
