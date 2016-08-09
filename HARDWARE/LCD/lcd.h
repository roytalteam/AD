#ifndef __LCD_H
#define __LCD_H

#include"sys.h"
#include"delay.h"

#define  LcdClear()  LcdWriteCmd(0x01)  //用宏定义定义一个清除显示的函数。


void LcdIOInitOut(void);           		//把所有端口初始化为输出模式的函数。
void LcdIOInitIn(void);	          		//把数据引脚初始化为浮空输入的函数。
void WaitBusy(void);      	 			//等待12864的忙状态结束的函数。
void LcdWriteCmd(u8 cmd);    			//写命令函数。
void LcdWriteDat(u8 dat);  				//写数据函数。
void LcdInit(void);            			//初始化 12864 和要用到的 STM 32 的引脚。
void LcdDisStr(u8 *s);    				//显示字符串的函数。
void LcdSetPos(int x, int y);			//设置光标位置

#endif
