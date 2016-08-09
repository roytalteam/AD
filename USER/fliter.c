#include "fliter.h"
#include "delay.h"
#include  <lcd.h>
#include "ads1256.h"
#include <stdio.h>


/**********************************************************/
// 函数名：中值滤波函数
// 描述  ：提取前9次采集的数据，去掉高3个，去掉低3个，然后中间的
// 描述  ：3个数据求平均值，该算法可尽可能的滤掉干扰数据，并不影响采集速度。
// 输入  ：9个数据的数组
// 输出  ：中间3个数据的平均值
/*********************************************************/
unsigned long MED_Filter(unsigned char addr)  //xad - ADC转换值   
{   
	unsigned long MED_buffer[MEDIAN_LEN];   //中值滤波数组缓存
	unsigned char i,j;
	unsigned long xd;
	unsigned long xxd;
	
	for(i=0; i<9; i++)
		{
			MED_buffer[i] = ADS_sum(addr);
		}
	
	for(i=0; i<MEDIAN_LEN; i++)     
	{   
		for(j=0; j<MEDIAN_LEN-i; j++)
		{
			
			if( MED_buffer[i] > MED_buffer[i+1]) // 轮询到的当前元素>AD值,则交换它们的值   
			{
				xd = MED_buffer[i]; 
				MED_buffer[i] = MED_buffer[i+1];
				MED_buffer[i+1] = xd;
			} 
		}				
	}
		
	xxd = MED_buffer[MEDIAN - 1] + MED_buffer[MEDIAN] + MED_buffer[MEDIAN + 1];
	xd = xxd/3;
	return (xd); //中值     
}

/**********************************************************/
// 函数名：加权滤波函数
// 描述  ：
// 描述  ：
// 输入  ：多组采集电压值
// 输出  ：经过滤波后的数据
/*********************************************************/
 
//unsigned long AD_Filter(unsigned char addr) 
//{ 
//	char coe[N] = {1,2,3,4,5,6,7,8,9,10,11,12}; 
//	char sum_coe = 1+2+3+4+5+6+7+8+9+10+11+12;
//	char count; 
//	unsigned long value_buf[N]; 
//	unsigned long sum = 0; 
//	
//	for(count=0; count<N; count++) 
//	{ 
//		value_buf[count] = ADS_sum(addr); 
//		delay_ms(5); 
//	} 
//	
//	for(count=0; count<N; count++)
//	{
//		sum += value_buf[count] * coe[count];
//	}	
//	 
//	return (unsigned long)(sum/sum_coe); 
//} 



/**********************************************************/
// 函数名：递推（滑动）滤波函数
// 描述  ：
// 描述  ：
// 输入  ：多组采集电压值
// 输出  ：经过滤波后的数据
/*********************************************************/

unsigned long Filter(unsigned char addr) 
{ 
	char i=0;
	static unsigned char buff[9];
	static unsigned long value_buf[N+1] = {0};
	unsigned long sum=0; 
	

	sprintf(buff, "采样值：%ld", value_buf[N]);
	LcdSetPos(3, 1);
	LcdDisStr(buff);
	
	for(i=0; i<N; i++)
	{
		value_buf[i] = value_buf[i+1];
		sum += value_buf[i];	
	}
    value_buf[N] = ADS_sum(addr); 
	
	sum = (unsigned long)sum/N;
	sum = (float)((float)sum* 2.0 * 2.5 / 8388607.0 * 100000000.0);
	sprintf(buff, "电压：%ld", sum);
	LcdSetPos(4, 1);
	LcdDisStr(buff); 
	return (unsigned long)(sum/N); 
} 

























