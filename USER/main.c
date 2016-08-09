#include "sys.h"
#include "delay.h"
#include "ads1256.h"
#include "lcd.h"
#include "stdio.h"

unsigned char   medleng = 0;          //存入缓存的数据个数
unsigned char str2[] = {"--AD采样系统--"};
unsigned char str3[] = {"当前采样通道：1"};
unsigned char buf[10];
	char buff[10];

/**********************************************************/
// 函数名：main
// 描述  ：主函数
// 输入  ：无
// 输出  ：无
/*********************************************************/
int main(void)
{  
	unsigned long results = 0;	                     
	delay_init();//延时时钟初始化 
	SPI_ADS1256_Init();								  //SPI初始化
	LcdInit();
	ADS1256_GPIO_init();							  //端口初始化
	ADS1256_Init();
	LcdSetPos(1, 1);
	LcdDisStr(str2);
	LcdSetPos(2, 1);
	LcdDisStr(str3);
	
	while(1)
	{
		
//	  	ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//通道0对地的电压值
		
		results = (Filter(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM));
//		sprintf(buff, "采样值：%ld", results);
//		LcdSetPos(4, 1);
//		LcdDisStr(buff);
//		results = (float)(results* 2 * 2.5 / 8388607 * 100000000);

//		sprintf(buf, "电压:%.8f", results);
//		LcdSetPos(4, 1);
//		LcdDisStr(buf);
		
//		ADS_sum(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);	//通道1对地的电压值
	
//		ADS_sum(ADS1256_MUXP_AIN2 | ADS1256_MUXN_AINCOM);	//通道2对地的电压值

//		ADS_sum(ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM);	//通道3对地的电压值

//		ADS_sum(ADS1256_MUXP_AIN4 | ADS1256_MUXN_AINCOM);	//通道4对地的电压值

//		ADS_sum(ADS1256_MUXP_AIN5 | ADS1256_MUXN_AINCOM);	//通道5对地的电压值
	
//  	ADS_sum(ADS1256_MUXP_AIN6 | ADS1256_MUXN_AINCOM);	//通道6对地的电压值

//		ADS_sum(ADS1256_MUXP_AIN7 | ADS1256_MUXN_AINCOM);	//通道7对地的电压值		

//		delay_ms(500);
	}
}

