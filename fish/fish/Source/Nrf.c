#include "Nrf.h"

u8	RandomChannel=12;										//默认随机频道为12 通道0
u8	fChannelReadyJump=1;									//是否更新随机频道1是 0否

u8	NrfGetData[NRFDataLength];								//nrf24l01接收到的数据
u8	GetLength;
u8	NrfSendData[NRFDataLength];								//nrf24l01需要发送的数据
u8  TX_ADDRESS[TXAdrLength]= {0xD0,0x20,0xE0,0xA0,0x30};	//本地地址
u8  RX_ADDRESS[RXAdrLength]= {0xD0,0x20,0xE0,0xA0,0x30};	//接收地址
u8  SendPacket[28];
const u8 Channels[13]={2,32,70,5,35,68,8,39,65,11,41,62,0};
/* Nrf写寄存器 */
u8 NrfWriteReg(u8 reg, u8 value)
{
	u8 status;
	SPINSSL;					//选通器件
	status = SpiRW(reg);		//写寄存器地址
	SpiRW(value);				//写数据
	SPINSSH;					//禁止该器件
	return 	status;
}

/* Nrf读寄存器 */
u8 NrfReadReg(u8 reg)
{
	uint8_t RegValue;
	SPINSSL;					//选通器件
	SpiRW(reg);					//写寄存器地址
	RegValue = SpiRW(0);		//读取该寄存器返回数据
	SPINSSH;					//禁止该器件
    return 	RegValue;
}

/* Nrf写缓冲区 */
u8 NrfWriteBuffer(u8 reg, u8 *pBuf, u8 Length)
{
	u8 i;
	u8 status;
	SPINSSL;				        //选通器件
	status = SpiRW(reg);			//写寄存器地址
	for(i=0; i<Length; i++)
	{
		SpiRW(pBuf[i]);				//写数据
	}
	SPINSSH;						//禁止该器件
    return 	status;	
}

/* Nrf读缓冲区 */
u8 NrfReadBuffer(u8 reg, u8 *pBuf, u8 Length)
{
	u8 i;
	u8 status;
	SPINSSL;						//选通器件
	status = SpiRW(reg);			//写寄存器地址
	for(i=0; i<Length; i++)
	{
		pBuf[i] = SpiRW(0);			//读取返回数据 	
	}
	SPINSSH;						//禁止该器件
    return 	status;
}

/* Nrf写数据包 */
void NrfSendPacket(uint8_t * tx_buf, uint8_t len)
{	
	NRFCEL;		 //StandBy I模式	
	NrfWriteBuffer(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TXAdrLength); // 装载接收端地址
	NrfWriteBuffer(WR_TX_PLOAD, tx_buf, len); 			 // 装载数据	
	NRFCEH;		 //置高CE，激发数据发送
}

void NrfSendPacketAP(uint8_t * tx_buf, uint8_t len)
{	
	NRFCEL;		 //StandBy I模式	
	NrfWriteBuffer(0xa8, tx_buf, len); 			 // 装载数据
	NRFCEH;		 //置高CE
}
u8 NrfCheck(void)
{ 
	u8 buf1[5]; 
	u8 i; 
	/*写入5个字节的地址. */ 
	NrfWriteBuffer(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,5); 
	/*读出写入的地址 */ 
	NrfReadBuffer(TX_ADDR,buf1,5); 
	/*比较*/ 
	for(i=0;i<5;i++) 
	{ 
		if(buf1[i]!=TX_ADDRESS[i]) 
			break; 
	} 
	if(i==5)
		return 0x1; //MCU与NRF成功连接 
	else
		return 0x0; //MCU与NRF不正常连接 
}
void NrfInterrupt(void)											//此程序主发送 发送完成后的应答中得到数据
{
	u8 sta = NrfReadReg(NRF_READ_REG + NRFRegSTATUS);
	fLedFlash=1;
	//GPIOC->ODR ^= (1<<13);
	if(sta & (1<<RX_DR))										//Nrf接收中断
	{
		GetLength = NrfReadReg(R_RX_PL_WID);					//判断接收长度
		if(GetLength<=33&&GetLength>=5)							//长度小于33
		{
			NrfReadBuffer(RD_RX_PLOAD,NrfGetData,GetLength);	//读取接收缓冲区数据
			//GPIOC->ODR ^= (1<<13);
			AnalyseData();
		}
		else 
		{
			NrfWriteReg(FLUSH_RX,0xff);							//清空缓冲区
		}
	}

	if(sta & (1<<TX_DS))										//发送完成中断
	{
		
	}
	if(sta & (1<<MAX_RT))
	{
		static u16 max_cnt = 0;
		max_cnt++;
		if(sta & 0x01)	//TX FIFO FULL
		{
			NrfWriteReg(FLUSH_TX,0xff);
		}
	}
	NrfWriteReg(NRF_WRITE_REG + NRFRegSTATUS, 0x70);					//清除中断
}

//打包数据
void PackData(u8 *PacketData,u8 PacketType,u8 Length)					//包类型678位<<5 8种 Length<28
{
	u8 i,Check;
	NrfSendData[0]=PacketHead;											//0		:包头
	PacketType%=8;														//包类型限制0-7
	NrfSendData[1]=PacketType<<5|(Length);								//1		:包类型和数据长
	memcpy(&NrfSendData[2],PacketData,Length);							//2-L+1	:复制数据
	Check=0;															//计算校验
	for(i=2;i<(Length+2);i++)
	{
		Check+=NrfSendData[i];											//循环加获得校验和
	}
	NrfSendData[Length+2]=~Check;										//L+2	:校验
	NrfSendData[Length+3]=PacketTail;									//L+2	:包尾
	NrfSendPacketAP(NrfSendData,Length+4);
}
void CheckID(u8 *PacketData,u8 Length)
{
	if(*PacketData==0x00)
	{
		SendPacket[0]=0x00;
		memcpy((void *)(SendPacket+1),(void *)FishID,5);
		PackData(SendPacket,0,6);
	}
}
void PacketWrite(u8 *PacketData,u8 Length)
{
	int i=0;
	int CPGPBuffer[13];
	if(*PacketData==0x00)
	{
		CPGMode=0;
		if(ReadyGetData==0)
		{
			Speed  = *(PacketData+1);
			Direct = (*(PacketData+2))&0x0F;
			if(((*(PacketData+2))>>4)==1)
			{
				Mode=1;
				ModeCount=0;
				ModeKeep=10;
				ModeTotal=50;
				ResetData();
			}
			ReadyGetData=1;
		}
		SendPacket[0]=0x00;
		SendPacket[1]=0xAA;
		PackData(SendPacket,2,2);
	}else if(*PacketData==0x01){
		CPGMode=1;
		for(i=0;i<26;i++){
			*((u8 *)(CPGPBuffer+i)) = *(PacketData+i+1);
		}
		for(i=0;i<13;i++){
			*((float *)(&CPGP+i)) = ((float)CPGPBuffer[i])/ScarlCPG; 
		}
	}else if(*PacketData==0xA0){
		ServoOffset[0] = ((int)(signed char)*(PacketData+1))*3;
		ServoOffset[1] = ((int)(signed char)*(PacketData+2))*3;
		ServoOffset[2] = ((int)(signed char)*(PacketData+3))*3;
		SendPacket[0]=0xA0;
		SendPacket[1]=0xAA;
		PackData(SendPacket,2,2);
	}
	else if(*PacketData==0xA3)
	{
		FishSetting.Offset0=ServoOffset[0];
		FishSetting.Offset1=ServoOffset[1];
		FishSetting.Offset2=ServoOffset[2];
		SaveParameter();
		SendPacket[0]=0xA3;
		SendPacket[1]=0xAA;
		PackData(SendPacket,2,2);
	}
}
void PacketRead(u8 *PacketData,u8 Length)
{
	u16 Voltage;
	if(*PacketData==0x10)
	{
		Voltage=GetRandomValue(12);
		//Voltage=ADC1GetConvertValue();
		//srand(Voltage);
		//Voltage=rand()%12;
		//Voltage=*(u16 *)(0x1FF80078);
		SendPacket[0]=0x10;
		SendPacket[1]= Voltage>>8;
		SendPacket[2]= Voltage&0xFF;
		PackData(SendPacket,3,3);
	}
}
void AnalyseData(void)
{
	u8 i,Check,Length,PacketType;
	if(NrfGetData[0]==0x21&&NrfGetData[GetLength-1]==0x52)
	{
		Check=0;
		Length=NrfGetData[1]&0x1F;
		PacketType=NrfGetData[1]>>5;
		for(i=2;i<Length+2;i++)
		{
			Check+=NrfGetData[i];
		}
		Check=~Check;
		if(Check==NrfGetData[GetLength-2])
		{
			switch(PacketType)
			{
				case 0:CheckID(NrfSendData+2,Length);break;
				//case 1:MPUReadData();PackData(MPUData,1,14);break;
				case 2:PacketWrite(NrfGetData+2,Length);break;
				case 3:PacketRead(NrfGetData+2,Length);break;
				default:PackData((u8 *)&("Good"),0,4);break;
			}
		}
	}
	else
	{
		PackData((u8 *)&("Bad"),4,3);
	}
}
void NrfInitialize(u8 model)
{
	NRFCEL;
	NrfWriteBuffer(NRF_WRITE_REG+RX_ADDR_P0,FishID,RXAdrLength);									//写RX节点地址 
	NrfWriteBuffer(NRF_WRITE_REG+TX_ADDR,FishID,TXAdrLength);										//写TX节点地址
	memcpy((void *)TX_ADDRESS,(void *)FishID,5);
	//TX_ADDRESS = FishID;
	NrfWriteReg(NRF_WRITE_REG+EN_AA,0x01); 															//使能通道0的自动应答 
	NrfWriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);														//使能通道0的接收地址
	NrfWriteReg(NRF_WRITE_REG+SETUP_AW,0x03);														//5位数据
	NrfWriteReg(NRF_WRITE_REG+SETUP_RETR,0x10);														//设置自动重发间隔时间:500us;最大自动重发次数:10次 
	NrfWriteReg(NRF_WRITE_REG+RF_CH,0x00);															//设置RF通道为CHANAL
	NrfWriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); 														//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	NrfWriteReg(NRF_WRITE_REG+NRFRegSTATUS,0x70); 													//设置TX发射参数,0db增益,1Mbps,低噪声增益开启
/////////////////////////////////////////////////////////
	if(model==1)																					//RX
	{
		NrfWriteReg(NRF_WRITE_REG+RX_PW_P0,NRFDataLength);									//选择通道0的有效数据宽度 
		NrfWriteReg(NRF_WRITE_REG + CONFIG, 0x0f);											// IRQ收发完成中断开启,16位CRC,主接收
	}
	else if(model==2)		//TX
	{
		NrfWriteReg(NRF_WRITE_REG+RX_PW_P0,NRFDataLength);								//选择通道0的有效数据宽度 
		NrfWriteReg(NRF_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断开启,16位CRC,主发送
	}
	else if(model==3)		//RX2
	{
		NrfWriteReg(FLUSH_TX,0xff);
		NrfWriteReg(FLUSH_RX,0xff);
		NrfWriteReg(NRF_WRITE_REG + CONFIG, 0x3f);   		 // IRQ收发完成中断开启,16位CRC,主接收
		
		SpiRW(0x50);
		SpiRW(0x73);
		NrfWriteReg(NRF_WRITE_REG+0x1c,0x01);
		NrfWriteReg(NRF_WRITE_REG+0x1d,0x07);
	}
	else								//TX2
	{
		NrfWriteReg(NRF_WRITE_REG + CONFIG, 0x0E);   		 // IRQ收发完成中断开启,16位CRC,主发送
		NrfWriteReg(FLUSH_TX,0xff);
		NrfWriteReg(FLUSH_RX,0xff);
		
		SpiRW(0x50);
		SpiRW(0x73);
		NrfWriteReg(NRF_WRITE_REG+0x1c,0x01);
		NrfWriteReg(NRF_WRITE_REG+0x1d,0x07);
	}
	NRFCEH;
}
void NrfJumpChannel(u8 Channel)
{
	if(Channel<12)
		NrfWriteReg(NRF_WRITE_REG+RF_CH,Channels[Channel]);
}
