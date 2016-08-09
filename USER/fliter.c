#include "fliter.h"
#include "delay.h"
#include  <lcd.h>
#include "ads1256.h"
#include <stdio.h>


/**********************************************************/
// ����������ֵ�˲�����
// ����  ����ȡǰ9�βɼ������ݣ�ȥ����3����ȥ����3����Ȼ���м��
// ����  ��3��������ƽ��ֵ�����㷨�ɾ����ܵ��˵��������ݣ�����Ӱ��ɼ��ٶȡ�
// ����  ��9�����ݵ�����
// ���  ���м�3�����ݵ�ƽ��ֵ
/*********************************************************/
unsigned long MED_Filter(unsigned char addr)  //xad - ADCת��ֵ   
{   
	unsigned long MED_buffer[MEDIAN_LEN];   //��ֵ�˲����黺��
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
			
			if( MED_buffer[i] > MED_buffer[i+1]) // ��ѯ���ĵ�ǰԪ��>ADֵ,�򽻻����ǵ�ֵ   
			{
				xd = MED_buffer[i]; 
				MED_buffer[i] = MED_buffer[i+1];
				MED_buffer[i+1] = xd;
			} 
		}				
	}
		
	xxd = MED_buffer[MEDIAN - 1] + MED_buffer[MEDIAN] + MED_buffer[MEDIAN + 1];
	xd = xxd/3;
	return (xd); //��ֵ     
}

/**********************************************************/
// ����������Ȩ�˲�����
// ����  ��
// ����  ��
// ����  ������ɼ���ѹֵ
// ���  �������˲��������
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
// �����������ƣ��������˲�����
// ����  ��
// ����  ��
// ����  ������ɼ���ѹֵ
// ���  �������˲��������
/*********************************************************/

unsigned long Filter(unsigned char addr) 
{ 
	char i=0;
	static unsigned char buff[9];
	static unsigned long value_buf[N+1] = {0};
	unsigned long sum=0; 
	

	sprintf(buff, "����ֵ��%ld", value_buf[N]);
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
	sprintf(buff, "��ѹ��%ld", sum);
	LcdSetPos(4, 1);
	LcdDisStr(buff); 
	return (unsigned long)(sum/N); 
} 

























