#include "UART.h"
typedef struct
{
	u8 *TXData;
	u16 TXLength;
	u16 TXCount;
	u8	fTX;
	u8 *RXDAta;
	u16 RXLength;
}USARTData;
USARTData USART1Data;
void UARTInitialize(void)
{
	USART_InitTypeDef USART_InitStructure;               			//�������ûָ�Ĭ�ϲ���
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);			//RCCʱ������
	USART1Data.fTX=1;
	
    USART_InitStructure.USART_BaudRate = 115200;                    //������115200 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//�ֳ�8λ 
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1λֹͣ�ֽ� 
    USART_InitStructure.USART_Parity = USART_Parity_No;				//����żУ�� 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//��Rx���պ�Tx���͹��� 
	USART_Init(USART1, &USART_InitStructure);						//��ʼ��
	//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);					//�����жϿ���
	//USART_ITConfig(USART1, , ENABLE);								//���ڽ����ж�
    USART_Cmd(USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //PA9TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //TX��ʼ�� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                     	//PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //RX��ʼ�� 
	
}
void USART1Send(u8 *Send,u16 Length)
{
	if(USART1Data.fTX==1)
	{
		USART1Data.TXLength=Length;
		USART1Data.TXData=(u8 *)malloc(sizeof(u8)*USART1Data.TXLength);
		for(USART1Data.TXCount=0;USART1Data.TXCount<USART1Data.TXLength;USART1Data.TXCount++)
		{
			*(USART1Data.TXData+USART1Data.TXCount)=*(Send+USART1Data.TXCount);
		}
		USART_SendData(USART1,(u16)*(USART1Data.TXData));
		USART1Data.TXCount=1;
		USART1Data.fTX=0;
	}
}
const u8 Ascii[11]="0123456789";
void SendDec(int16_t Dec)
{
	u8 toSend[7];
	if(Dec>=0)
		toSend[0]=Ascii[0];
	else
	{
		toSend[0]='-';
		Dec=-Dec;
	}
	toSend[1]=Ascii[Dec/10000];
	toSend[2]=Ascii[Dec%10000/1000];
	toSend[3]=Ascii[Dec%1000/100];
	toSend[4]=Ascii[Dec%100/10];
	toSend[5]=Ascii[Dec%10];
	toSend[6]='\n';
	USART1Send(toSend,7);
}
void SendAngle(float Pitch,float Roll)
{
	u8 toSend[16];
	if(Pitch>=0)
	toSend[0]=Ascii[0];
	else
	{
		toSend[0]='-';
		Pitch=-Pitch;
	}
	toSend[1]=Ascii[(u8)Pitch/100];
	toSend[2]=Ascii[(u8)Pitch%100/10];
	toSend[3]=Ascii[(u8)Pitch%10];
	toSend[4]='.';
	toSend[5]=Ascii[(u8)(Pitch*10)%10];
	toSend[6]=Ascii[(u8)(Pitch*100)%10];
	toSend[7]=',';
	if(Roll>=0)
	toSend[8]=Ascii[0];
	else
	{
		toSend[8]='-';
		Roll=-Roll;
	}
	toSend[9]=Ascii[(u8)Roll/100];
	toSend[10]=Ascii[(u8)Roll%100/10];
	toSend[11]=Ascii[(u8)Roll%10];
	toSend[12]='.';
	toSend[13]=Ascii[(u8)(Roll*10)%10];
	toSend[14]=Ascii[(u8)(Roll*100)%10];
	toSend[15]='\n';
	USART1Send(toSend,16);
}
void USART1_IRQHandler(void)                            		//����1�ж� 
{ 
	char RX_dat;                                                       	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//�жϷ��������ж� 
	{
		USART_ClearITPendingBit(USART1,    USART_IT_RXNE);		//����жϱ�־ 
		RX_dat=USART_ReceiveData(USART1);						//�������� 
		USART_SendData(USART1, RX_dat);				 			//��������
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)		//�жϷ��������ж� 
	{
		
		USART_ClearITPendingBit(USART1,    USART_IT_TC);		//����жϱ�־
		if(USART1Data.fTX==0)
		{
			if(USART1Data.TXCount==USART1Data.TXLength)
			{
				USART1Data.fTX=1;
				free(USART1Data.TXData);
			}
			else
			{
				USART_SendData(USART1,(u16)*(USART1Data.TXData+USART1Data.TXCount));
				USART1Data.TXCount++;
			}
		}
	
	} 
}
