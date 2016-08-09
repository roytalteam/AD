#include "ads1256.h"
#include "delay.h"
#include "stm32f10x_spi.h"
#include "lcd.h"
#include "stdio.h"

u8 results1,results2,results3;

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_ADS1256_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);

  /*!< SPI_FLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  /*!< AFIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                    			//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						// 推免输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*!< Configure SPI_FLASH_SPI pins: DIN */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;							// 复用推免输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  /*!< Configure SPI_FLASH_SPI pins: DOUT */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                    			//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */

  /* SPI1 configuration */
  // 在SCLK下降沿，系统通过DIN向1256发送数据 
  // 在SCLK上升沿，系统通过DOUT 读取1256数据
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	  //两线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							  //主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						  //8位模式
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							  //SCLK空闲时位低电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							  //SCLK的下降沿采集数据
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								  //从机选择信号，软件模式，就是用GPIO选择从机
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	  //APB2或者APB1总线频率的1/2~1/256分频
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					  //数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7;								  //CRC值计算的多项式
  SPI_Init(SPI1, &SPI_InitStructure);


  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}


/*******************************************************************
函数名：初始化DAC接口
函数原型：void ADC_gpio_init()
参数说明：无
返回：  无
********************************************************************/
void ADS1256_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/////////CS  reset 引脚设置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;	//--推免输出模式
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz; 	//--GPIO翻转速度为50MHz

	GPIO_Init(GPIOA , &GPIO_InitStructure);	//--将配置写入对应的GPIO寄存器中	


//	GPIO_InitTypeDef GPIO_InitStructure;
    ///////// DRDY引脚设置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;	//--浮点输入模式
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz; 	//--GPIO翻转速度为50MHz
	GPIO_Init(GPIOA , &GPIO_InitStructure);	//--将配置写入对应的GPIO寄存器中

}

//-----------------------------------------------------------------//
//	功    能：ADS1256 写数据
//	入口参数: /
//	出口参数: /
//	全局变量: /
//	备    注: 向ADS1256中地址为regaddr的寄存器写入一个字节databyte
//-----------------------------------------------------------------//

void ADS1256WREG(unsigned char regaddr,unsigned char databyte)
{
    ADS_CS_LOW();
	while(ADS_DRDY);//当ADS1256_DRDY为低时才能写寄存器
	//向寄存器写入数据地址
    SPI_SendByte(ADS1256_CMD_WREG | (regaddr & 0xF));
    //写入数据的个数n-1
    SPI_SendByte(0);
    //向regaddr地址指向的寄存器写入数据databyte
  	delay_us(5);
    SPI_SendByte(databyte);
	ADS_CS_HIGH();
}

//-----------------------------------------------------------------//
//	功    能：ADS1256 读寄存器数据
//	入口参数: /
//	出口参数: /
//	全局变量: /
//	备    注: 从ADS1256中地址为regaddr的寄存器读出一个字节databyte
//-----------------------------------------------------------------//

unsigned char ADS1256RREG(unsigned char regaddr)
{
    //从ADS1256中地址为regaddr的寄存器读出一个字节
    unsigned char r=0;
    ADS_CS_LOW();
	while(ADS_DRDY);//当ADS1256_DRDY为低时才能写寄存器
	//写入地址
    SPI_SendByte(ADS1256_CMD_RREG | (regaddr & 0xF));
    //写入读取数据的个数n-1
    SPI_SendByte(0);
	  delay_us(10);  //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
    //读出regaddr地址指向的寄存器的数据
    r=SPI_SendByte(0);
	  ADS_CS_HIGH();

    return r;//返回数据
}

//-----------------------------------------------------------------//
//	功    能：ADS1256初始化子程序
//	入口参数: /
//	出口参数: /
//	全局变量: /
//	备    注: /
//-----------------------------------------------------------------//
void ADS1256_Init(void)
{     
	ADS_CS_LOW();
	RESET_HIGH();
	delay_us(100);
	SPI_SendByte(ADS1256_CMD_REST);
	delay_ms(10);
	while(ADS_DRDY);                                //当ADS1256_DRDY为低时才能写寄存器
	SPI_SendByte(ADS1256_CMD_SYNC);                 //同步命令
	SPI_SendByte(ADS1256_CMD_WAKEUP);               //同步唤醒
	
	while(ADS_DRDY);
	SPI_SendByte(ADS1256_CMD_WREG | ADS1256_STATUS);//连续写入4个寄存器
	SPI_SendByte(3);
	SPI_SendByte(0x04);                             //高位在前，使用内部校准，不使用缓存
	SPI_SendByte(ADS1256_MUXP_AIN2 | ADS1256_MUXN_AIN3);//端口输入A2为正，A3位负
	SPI_SendByte(ADS1256_GAIN_1);                   //放大倍数设置
	SPI_SendByte(ADS1256_DRATE_2_5SPS);             //采集速度设置
	delay_us(100);
	
	while(ADS_DRDY);                                //当ADS1256_DRDY为低时才能写寄存器
	SPI_SendByte(ADS1256_CMD_SELFCAL);              //偏移和增益自动校准
	
    ADS_CS_HIGH();
	delay_ms(100);
 
}

//-----------------------------------------------------------------//
//	功    能：
//	入口参数: /
//	出口参数: /
//	全局变量: /
//	备    注:
//-----------------------------------------------------------------//

unsigned long ADS1256ReadData(void)  
{
    unsigned char i=0;
    unsigned long sum=0;
		unsigned long r=0;
		ADS_CS_LOW();

		while(ADS_DRDY);               //当ADS1256_DRDY为低时才能写寄存器 	
		SPI_SendByte(ADS1256_CMD_SYNC);
		SPI_SendByte(ADS1256_CMD_WAKEUP);
		SPI_SendByte(ADS1256_CMD_RDATA);
		delay_us(10);                  //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
			for(i=0;i<3;i++)
			{
				sum = sum << 8;
				r = SPI_SendByte(0);  
				sum |= r;
			}
		ADS_CS_HIGH();		 
    return sum;
}

//-----------------------------------------------------------------//
//	功    能：读取ADS1256单路数据
//	入口参数: /
//	出口参数: /
//	全局变量: /
//	备    注: /
//-----------------------------------------------------------------//
unsigned long ADS_sum(unsigned char road)
{
//	long results=0;
	unsigned long Result_sum=0;
//	unsigned long fVoltage;
//	char buff[10];
	
	

	ADS1256WREG(ADS1256_MUX,road);		//设置通道
	SPI_SendByte(ADS1256_CMD_SELFCAL);    //偏移和增益自动校准

	Result_sum = ADS1256ReadData();//读取AD值，返回24位数据。
//	sprintf(buff, "采样值：%ld", Result_sum);
//	LcdSetPos(3, 1);
//	LcdDisStr(buff);

//	if(Result_sum < 0x800000)    //测得电压值为正的情况
//	{
//		fVoltage = ((Result_sum * 2 * 2.5) / 8388607)*100000000;

//		fVoltage = Result_sum * 985;  //954=2*2.5/2^19*100000000   2.5为基准电压  100000000为为了计算的准确性转换为整形计算。理论为954，实际修正为985
//		

//	 }
//	 else					 //测得电压值为负的情况
//	 {
//		buf[0] = '-';					       //取‘-’号
//		Result_sum = 0xfffff - Result_sum;	   //取反运算 
//		fVoltage = Result_sum * 985;  //954=2*2.5/2^19*100000000   2.5为基准电压  100000000为为了计算的准确性转换为整形计算
//	 }
	return Result_sum; 
//	ADS1256WREG(ADS1256_MUX,road);		//设置通道
//	results = ADS1256ReadData();        //读取AD值，返回24位数据。
//	results1 = (results >> 16) & 0x0000ff; //发送最高位	
//	results2 = (results >> 8) & 0x0000ff;  //发送中间位
//  results3 = results & 0x0000ff;		   //发送低位

}





