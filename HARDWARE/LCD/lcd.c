#include "lcd.h"

/********** 以下是相关引脚定义。**************/

#define Data GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
                                     //定义12864使用的数据引脚。
#define RS GPIO_Pin_9
#define RW GPIO_Pin_10
#define EN GPIO_Pin_11           		//定义使能端使用的引脚。
#define PSB GPIO_Pin_12      		//定义并，串行数据模式的选择端口使用的引脚。
                					    
GPIO_InitTypeDef  GPIOStructure;  //定义用于定义所以引脚为输出的变量。

/*********************************************/

void LcdIOInitOut(void)           //把所有端口初始化为推挽输出模式的函数。
{
    GPIOStructure.GPIO_Mode = GPIO_Mode_Out_PP;//定义所有的引脚为推挽输出的变量初始化。
    GPIOStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOStructure.GPIO_Pin = Data|RS|RW|EN|PSB;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_Init(GPIOC,&GPIOStructure);	
}

/*********************************************/

void LcdIOInitIn(void)           //把数据引脚初始化为浮空输入的函数。
{
    GPIOStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//定义数据引脚为浮空输入的变量初始化。
    GPIOStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOStructure.GPIO_Pin = Data;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //把所有端口初始化为输入模式的函数。   
    GPIO_Init(GPIOC,&GPIOStructure);	
}

/*********************************************/

void WaitBusy(void)          	//等待12864的忙状态结束的函数。
{
    LcdIOInitIn();        		//把数据引脚定义为浮空输入；
	
    GPIO_ResetBits(GPIOC,RS);   //RS = 0.
    GPIO_SetBits(GPIOC,RW);     //RW = 1.
    GPIO_SetBits(GPIOC,EN);     //EN = 1.
	
    while(GPIO_ReadInputData(GPIOC) & 0x0080); //只要位7的值，位7是忙标志位。
	
    GPIO_ResetBits(GPIOC,EN);   //EN = 0;
    LcdIOInitOut();      		//把所有引脚定义为输出。
}

/*********************************************/

void LcdWriteCmd(u8 cmd)    	//写命令函数。
{
  	WaitBusy(); 
	
    GPIO_ResetBits(GPIOC,RS);   //RS = 0.
    GPIO_ResetBits(GPIOC,RW);   //RW = 0. 
    GPIO_SetBits(GPIOC,EN);     //EN = 1.
	
    GPIOC->ODR=((GPIOC->ODR & 0xff00)|cmd); //此处，只有直接操作寄存器才能
											//达到只改变输出数据寄存器ODR的低8位，其它位
											//不变的目的。
      
    delay_ms(2);
    GPIO_ResetBits(GPIOC,EN);   //EN = 0;
    delay_ms(2); 	
}

/*********************************************/

void LcdWriteDat(u8 dat)  		//写数据函数。
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

void LcdInit(void)   			//初始化 12864 和要用到的 STM 32 的引脚。
{
    LcdIOInitOut(); 	
    delay_init();   			//初始化延时函数的微妙计数基数。  
    GPIO_SetBits(GPIOC,PSB);    //令PSB=1，设置为并行数据模式。
	
    delay_ms(2); 
    LcdWriteCmd(0x30);  		//选择基本指令集，和，8位数据模式。
    delay_ms(2);
    LcdWriteCmd(0x0c);  		//开显示,无游标,不反白.
    delay_ms(2); 
    LcdWriteCmd(0x01);  		//清除显示，并将 DDRAM 的地址计数器 AC 设为 00H.
    delay_ms(2);
    LcdWriteCmd(0x06);  	    //设置，外部读写数据后,地址记数器 AC 会自动加 1。
    delay_ms(2);
    LcdWriteCmd(0x80);  		//将 DDRAM 地址计数器 AC 设为 0.
    delay_ms(2);	
}

/*********************************************/

void LcdDisStr(u8 *s)    		//显示字符串的函数。
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

void LcdSetPos(int x, int y) 	//设置光标位置
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
