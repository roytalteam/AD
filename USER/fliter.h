#ifndef __FLITER_H
#define __FLITER_H

#define MEDIAN_LEN  5                     //中直滤波的总长度，一般选取奇数   
#define MEDIAN      2                     //中值在滤波数组中的位置
#define N           50					  //加权滤波的总长度

unsigned long MED_Filter(unsigned char addr); //中值滤波函数
unsigned long AD_Filter(unsigned char addr); //加权滤波函数
unsigned long Filter(unsigned char addr);   //滑动滤波函数


#endif






















