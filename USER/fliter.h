#ifndef __FLITER_H
#define __FLITER_H

#define MEDIAN_LEN  5                     //��ֱ�˲����ܳ��ȣ�һ��ѡȡ����   
#define MEDIAN      2                     //��ֵ���˲������е�λ��
#define N           50					  //��Ȩ�˲����ܳ���

unsigned long MED_Filter(unsigned char addr); //��ֵ�˲�����
unsigned long AD_Filter(unsigned char addr); //��Ȩ�˲�����
unsigned long Filter(unsigned char addr);   //�����˲�����


#endif






















