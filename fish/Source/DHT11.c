#include "DHT11.h"
//unsigned char  humidity, temperature, check_sum ; 
u8 tdata[4]={0x00,0x00,0x00,0x00};
u8 sbuf,check;

                    //全局变量定义区
//***************************************************************************/
//      //uchar       i;
//      uchar    U8FLAG,k;
//      uchar    U8count,U8temp;
//      uchar    U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
//      uchar    U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
//      uchar    U8comdata;
//      uint   ReceiveHighByte;
//      uint   ReceiveLowByte;
//***************Global defination for DHT11 end****// 
void DHT11_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//浮动输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void DHT11_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//Configure pin as input 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
unsigned char StartDHT11(void)
{
   //unsigned char flag;

   //delay_ms(18);  //触发开始,总线拉低要大于18ms
  
   DHT11_Out = 1;	//释放总线
   delay_us(25);  //wait 20-40uS 等待DHT11的低电平响应信号
   DHT11_PortIN();//改为输入
   //delay_us(5);   //
   //if(!PINC.3)
   if(!DHT11_In)
   {
	   //while(DHT11_In)//DHT11如果响应的话会拉低总线80us
	   //{
	   	   while(!DHT11_In);//低电平的响应信号，80us
		   while(DHT11_In);//紧接着是80us的高电平数据准备信号
		   //拉低后DHT11会拉高总线80us,接着会开始传数据	
		   return 1;
	   //}   
   }
//   if(DHT11_In)
//   {
//      //while((!PINC.3) && flag ++); //等待低电平80uS
//         
//      while((!DHT11_In) && flag ++);
//      //while((PINC.3) && flag ++);  //等待高电平80uS
//         
//	  while((DHT11_In) && flag ++);
//      return(1);
//   }
	return 0;
}
void com(void)
{
	u8 i,tt;
	//Flag = 0;
	tt = 0;
	for(i = 0;i<8;i++)
	{
		sbuf <<= 1;
		delay_us(20);//接收到响应后会出现50us的低电平表示发送数据的开始，所以这里小延时一下
		while(!DHT11_In);//等到高电平的出现，高电平的时间表示的是数据位的0和1
		delay_us(25);//数据为0的信号时间为26-28us，1则为70us，这里超时检测
		tt = 100;
		while(tt++);
		//tt = 200;
		//while(tt++);
		if(DHT11_In)//如果还为高
		{
			 sbuf |= 0x01;
			 //while(DHT11_In && ++Flag);//等待它变为低电平信号 ,这里不能一直都等，因为数据传送完了会拉高总线的
			 delay_us(30);//这里的延时足够了，40+10+30>70了
		}
		else //如果变低
		{
			sbuf &= 0xfe;
		}
	}
}
u8 ReadDHT11(void)
{
	u8 sum;
	if(StartDHT11())	//判断是否已经响应
	{
		com();
		tdata[0]=sbuf;
		com();
		tdata[1]=sbuf;
		com();
		tdata[2]=sbuf;		
		com();
		tdata[3]=sbuf;
		com();
		check = sbuf;
		sum = (tdata[0]+tdata[1]+tdata[2]+tdata[3]);
		//sum = sum - 0x80;
	}
	delay_ms(1);
	DHT11_PortOUT();
	DHT11_Out = 0;
	if(check == sum)
	return(1);
	else	
	return 0;
}
