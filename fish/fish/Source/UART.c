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
	USART_InitTypeDef USART_InitStructure;               			//串口设置恢复默认参数
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);			//RCC时钟设置
	USART1Data.fTX=1;
	
    USART_InitStructure.USART_BaudRate = 115200;                    //波特率115200 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 	//字长8位 
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1位停止字节 
    USART_InitStructure.USART_Parity = USART_Parity_No;				//无奇偶校验 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//打开Rx接收和Tx发送功能 
	USART_Init(USART1, &USART_InitStructure);						//初始化
	//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);					//串口中断开启
	//USART_ITConfig(USART1, , ENABLE);								//串口接收中断
    USART_Cmd(USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //PA9TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //TX初始化 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                     	//PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //RX初始化 
	
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
void USART1_IRQHandler(void)                            		//串口1中断 
{ 
	char RX_dat;                                                       	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//判断发生接收中断 
	{
		USART_ClearITPendingBit(USART1,    USART_IT_RXNE);		//清除中断标志 
		RX_dat=USART_ReceiveData(USART1);						//接收数据 
		USART_SendData(USART1, RX_dat);				 			//发送数据
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)		//判断发生发送中断 
	{
		
		USART_ClearITPendingBit(USART1,    USART_IT_TC);		//清除中断标志
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
