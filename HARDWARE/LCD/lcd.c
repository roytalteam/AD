#include "lcd.h"

/********** ������������Ŷ��塣**************/

#define Data GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
                                     //����12864ʹ�õ��������š�
#define RS GPIO_Pin_9
#define RW GPIO_Pin_10
#define EN GPIO_Pin_11           		//����ʹ�ܶ�ʹ�õ����š�
#define PSB GPIO_Pin_12      		//���岢����������ģʽ��ѡ��˿�ʹ�õ����š�
                					    
GPIO_InitTypeDef  GPIOStructure;  //�������ڶ�����������Ϊ����ı�����

/*********************************************/

void LcdIOInitOut(void)           //�����ж˿ڳ�ʼ��Ϊ�������ģʽ�ĺ�����
{
    GPIOStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������е�����Ϊ��������ı�����ʼ����
    GPIOStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOStructure.GPIO_Pin = Data|RS|RW|EN|PSB;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_Init(GPIOC,&GPIOStructure);	
}

/*********************************************/

void LcdIOInitIn(void)           //���������ų�ʼ��Ϊ��������ĺ�����
{
    GPIOStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//������������Ϊ��������ı�����ʼ����
    GPIOStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOStructure.GPIO_Pin = Data;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //�����ж˿ڳ�ʼ��Ϊ����ģʽ�ĺ�����   
    GPIO_Init(GPIOC,&GPIOStructure);	
}

/*********************************************/

void WaitBusy(void)          	//�ȴ�12864��æ״̬�����ĺ�����
{
    LcdIOInitIn();        		//���������Ŷ���Ϊ�������룻
	
    GPIO_ResetBits(GPIOC,RS);   //RS = 0.
    GPIO_SetBits(GPIOC,RW);     //RW = 1.
    GPIO_SetBits(GPIOC,EN);     //EN = 1.
	
    while(GPIO_ReadInputData(GPIOC) & 0x0080); //ֻҪλ7��ֵ��λ7��æ��־λ��
	
    GPIO_ResetBits(GPIOC,EN);   //EN = 0;
    LcdIOInitOut();      		//���������Ŷ���Ϊ�����
}

/*********************************************/

void LcdWriteCmd(u8 cmd)    	//д�������
{
  	WaitBusy(); 
	
    GPIO_ResetBits(GPIOC,RS);   //RS = 0.
    GPIO_ResetBits(GPIOC,RW);   //RW = 0. 
    GPIO_SetBits(GPIOC,EN);     //EN = 1.
	
    GPIOC->ODR=((GPIOC->ODR & 0xff00)|cmd); //�˴���ֻ��ֱ�Ӳ����Ĵ�������
											//�ﵽֻ�ı�������ݼĴ���ODR�ĵ�8λ������λ
											//�����Ŀ�ġ�
      
    delay_ms(2);
    GPIO_ResetBits(GPIOC,EN);   //EN = 0;
    delay_ms(2); 	
}

/*********************************************/

void LcdWriteDat(u8 dat)  		//д���ݺ�����
{
	WaitBusy(); 
	
    GPIO_SetBits(GPIOC,RS);  	//RS = 1. 
    GPIO_ResetBits(GPIOC,RW);   //RW = 0. 
    GPIO_SetBits(GPIOC,EN);     //EN = 1.
	
    GPIOC->ODR=((GPIOC->ODR & 0xff00)|dat);  
	
    delay_ms(2);
    GPIO_ResetBits(GPIOC,EN);  	//EN = 0;
    delay_ms(2); 
}

/*********************************************/

void LcdInit(void)   			//��ʼ�� 12864 ��Ҫ�õ��� STM 32 �����š�
{
    LcdIOInitOut(); 	
    delay_init();   			//��ʼ����ʱ������΢�����������  
    GPIO_SetBits(GPIOC,PSB);    //��PSB=1������Ϊ��������ģʽ��
	
    delay_ms(2); 
    LcdWriteCmd(0x30);  		//ѡ�����ָ����ͣ�8λ����ģʽ��
    delay_ms(2);
    LcdWriteCmd(0x0c);  		//����ʾ,���α�,������.
    delay_ms(2); 
    LcdWriteCmd(0x01);  		//�����ʾ������ DDRAM �ĵ�ַ������ AC ��Ϊ 00H.
    delay_ms(2);
    LcdWriteCmd(0x06);  	    //���ã��ⲿ��д���ݺ�,��ַ������ AC ���Զ��� 1��
    delay_ms(2);
    LcdWriteCmd(0x80);  		//�� DDRAM ��ַ������ AC ��Ϊ 0.
    delay_ms(2);	
}

/*********************************************/

void LcdDisStr(u8 *s)    		//��ʾ�ַ����ĺ�����
{
    while(*s != '\0')
    {
        LcdWriteDat(*s);
        s++;
        delay_ms(2);
    }
}

/*********************************************/


/*********************************************/

void LcdSetPos(int x, int y) 	//���ù��λ��
{
	switch(x)
	{
		case 1: LcdWriteCmd(0x80 + y -1); break;
		case 2: LcdWriteCmd(0x90 + y -1); break;
		case 3: LcdWriteCmd(0x88 + y -1); break;
		case 4: LcdWriteCmd(0x98 + y -1); break;
	}
}

/*********************************************/
