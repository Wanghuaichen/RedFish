#include "Spi.h"
void SPIInitialize(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;				//PA5 SCK PA7 MOSI
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;					//复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;					//10M
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;							//PA4 NSS(CSN)
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;					//10M
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;							//PA6 MISO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;						//输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;							//PB1 CE
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;					//推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//SPI主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//SPI数通讯位数8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS位软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//时钟分频16分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//MSB格式发送
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE); 
}
u8 SpiRW(u8 Data) 
{ 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 		//当SPI发送缓冲器非空时等待
	SPI_I2S_SendData(SPI1, Data);											//通过 SPI2发送一字节数据 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);		//当SPI接收缓冲器为空时等待
	return SPI_I2S_ReceiveData(SPI1);										//返回SPI接收到的的数据
}
