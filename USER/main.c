#include "sys.h"
#include "delay.h"
#include "ads1256.h"
#include "lcd.h"
#include "stdio.h"

unsigned char   medleng = 0;          //���뻺������ݸ���
unsigned char str2[] = {"--AD����ϵͳ--"};
unsigned char str3[] = {"��ǰ����ͨ����1"};
unsigned char buf[10];
	char buff[10];

/**********************************************************/
// ��������main
// ����  ��������
// ����  ����
// ���  ����
/*********************************************************/
int main(void)
{  
	unsigned long results = 0;	                     
	delay_init();//��ʱʱ�ӳ�ʼ�� 
	SPI_ADS1256_Init();								  //SPI��ʼ��
	LcdInit();
	ADS1256_GPIO_init();							  //�˿ڳ�ʼ��
	ADS1256_Init();
	LcdSetPos(1, 1);
	LcdDisStr(str2);
	LcdSetPos(2, 1);
	LcdDisStr(str3);
	
	while(1)
	{
		
//	  	ADS_sum(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);	//ͨ��0�Եصĵ�ѹֵ
		
		results = (Filter(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM));
//		sprintf(buff, "����ֵ��%ld", results);
//		LcdSetPos(4, 1);
//		LcdDisStr(buff);
//		results = (float)(results* 2 * 2.5 / 8388607 * 100000000);

//		sprintf(buf, "��ѹ:%.8f", results);
//		LcdSetPos(4, 1);
//		LcdDisStr(buf);
		
//		ADS_sum(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);	//ͨ��1�Եصĵ�ѹֵ
	
//		ADS_sum(ADS1256_MUXP_AIN2 | ADS1256_MUXN_AINCOM);	//ͨ��2�Եصĵ�ѹֵ

//		ADS_sum(ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM);	//ͨ��3�Եصĵ�ѹֵ

//		ADS_sum(ADS1256_MUXP_AIN4 | ADS1256_MUXN_AINCOM);	//ͨ��4�Եصĵ�ѹֵ

//		ADS_sum(ADS1256_MUXP_AIN5 | ADS1256_MUXN_AINCOM);	//ͨ��5�Եصĵ�ѹֵ
	
//  	ADS_sum(ADS1256_MUXP_AIN6 | ADS1256_MUXN_AINCOM);	//ͨ��6�Եصĵ�ѹֵ

//		ADS_sum(ADS1256_MUXP_AIN7 | ADS1256_MUXN_AINCOM);	//ͨ��7�Եصĵ�ѹֵ		

//		delay_ms(500);
	}
}

