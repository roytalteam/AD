#ifndef __LCD_H
#define __LCD_H

#include"sys.h"
#include"delay.h"

#define  LcdClear()  LcdWriteCmd(0x01)  //�ú궨�嶨��һ�������ʾ�ĺ�����


void LcdIOInitOut(void);           		//�����ж˿ڳ�ʼ��Ϊ���ģʽ�ĺ�����
void LcdIOInitIn(void);	          		//���������ų�ʼ��Ϊ��������ĺ�����
void WaitBusy(void);      	 			//�ȴ�12864��æ״̬�����ĺ�����
void LcdWriteCmd(u8 cmd);    			//д�������
void LcdWriteDat(u8 dat);  				//д���ݺ�����
void LcdInit(void);            			//��ʼ�� 12864 ��Ҫ�õ��� STM 32 �����š�
void LcdDisStr(u8 *s);    				//��ʾ�ַ����ĺ�����
void LcdSetPos(int x, int y);			//���ù��λ��

#endif
