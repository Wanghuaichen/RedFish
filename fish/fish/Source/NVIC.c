#include "NVIC.h"
u16 LEDCountA=0;
u8 fH=0;
u8 f1=0;
void NVICInitialize(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;					//��ʱ���ж�2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;			//�ж���ռ���ȼ�1	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//�ж���Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;					//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;					//��ʱ���ж�3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;			//�ж���ռ����3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;				//�ж���Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;					//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);									//��ʼ��
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//ͨ������Ϊ����1�ж� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 		//�ж���ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�ж���Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//���ж� 
	NVIC_Init(&NVIC_InitStructure);									//��ʼ�� 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//ͨ������Ϊ����1�ж� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 		//�ж���ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�ж���Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//���ж� 
	NVIC_Init(&NVIC_InitStructure);									//��ʼ�� 
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;						//PWM GPIO�趨
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,13);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;							//�ⲿ�ж�0
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;						//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	EXTIInitialize();
}
void EXTIInitialize(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit(EXTI_Line0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

	EXTI_InitStructure.EXTI_Line = EXTI_Line0; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
u8 c[6]="Meano";
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		NrfInterrupt();
	}
}
u8 fPlus=0;
u8 fRescue=0;
void PWMRange()
{
	if(fRescue)
	{
		if(fPlus==0)
		{
			TIM2->CCR4+=100;
			if((TIM2->CCR4)>=4400)
			{
				fPlus=1;
			}
		}
		else
		{
			TIM2->CCR4-=100;
			if((TIM2->CCR4)<=1600)
			{
				fPlus=0;
			}
		}
	}
}
void TIM2_IRQHandler(void)
{
	u8 i;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
		if(Mode==0)
		{
			if(CPGMode==0){
				CalculateSitaLevel(1,10,10,Speed,Direct);
				if(Speed==0&&Direct==7)
				{
					Sita[0]=0;
					Sita[1]=0;
					Sita[2]=0;
				}
			}else if(CPGMode==1){
				CalculateSitaf(CPGP.Miu,CPGP.F,CPGP.A,CPGP.B,CPGP.R,CPGP.X,CPGP.Fai);
			}
			ReadyGetData=0;
		}
		else if(Mode==1)
		{
			if(CPGMode==0){
				CalculateSitaLevel(1,10,10,Speed,Direct);
			}else if(CPGMode==1){
				CalculateSitaf(CPGP.Miu,CPGP.F,CPGP.A,CPGP.B,CPGP.R,CPGP.X,CPGP.Fai);
			}
			if(ModeCount==ModeKeep)
			{
				Mode=0;
				ReadyGetData=0;
			}
			if(ModeCount==0)
			{
				for(i=0;i<JointCount;i++)
				{
					XSet[i]=XTem[i][Direct];
				}
			}
			for(i=0;i<JointCount;i++)
			{
				Sita[i]=KSita*XSet[i];
			}
			ModeCount+=1;
		}
		TIM2->CCR1=3000+Sita[0]+ServoOffset[0];
		TIM2->CCR2=3000+Sita[1]+ServoOffset[1];
		TIM2->CCR3=3000+Sita[2]+ServoOffset[2];
	}
}
u8 fJump=0;
u16 JumpCount;
u8 fLedFlash=0;
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		if(fChannelReadyJump==1)							//����������յ�����ȷ��Ҫ����Ƶ��
		{
			fChannelReadyJump=0;
			RandomChannel=GetRandomValue(12);
		}
		else if(fChannelReadyJump>1)
		{
			fChannelReadyJump--;
		}
		JumpCount++;
		if(JumpCount%250==0)
		{
			if(fLedFlash==1)
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14);
				fLedFlash=0;
			}
			else if(Direct==7)
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14);
			}
			else if(Direct<7)
			{
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
				GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			}
			else if(Direct>7)
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);
				GPIO_SetBits(GPIOC,GPIO_Pin_14);
			}
			//GPIOC->ODR ^= (1<<13);
		}
		if(JumpCount==20000)
		{
			fJump=0;
			JumpCount=0;
		}
	}
}


