#include "Spi.h"
void SPIInitialize(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;				//PA5 SCK PA7 MOSI
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;					//�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;					//10M
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;							//PA4 NSS(CSN)
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;					//�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;					//10M
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;							//PA6 MISO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;						//����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;							//PB1 CE
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;					//�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//SPI����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//SPI��ͨѶλ��8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSSλ�������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//ʱ�ӷ�Ƶ16��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//MSB��ʽ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE); 
}
u8 SpiRW(u8 Data) 
{ 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 		//��SPI���ͻ������ǿ�ʱ�ȴ�
	SPI_I2S_SendData(SPI1, Data);											//ͨ�� SPI2����һ�ֽ����� 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);		//��SPI���ջ�����Ϊ��ʱ�ȴ�
	return SPI_I2S_ReceiveData(SPI1);										//����SPI���յ��ĵ�����
}
